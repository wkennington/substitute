/*
 * util.c: core utility function for file substituting
 *
 * Copyright (c) 2014, William A. Kennington III
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "util.h"

#define MIN_BUF_SIZE 4096

wchar_t *from_utf8(const char *str)
{
	size_t len = mbstowcs(NULL, str, 0);
	wchar_t *ret = malloc((len+1) * sizeof(wchar_t));
	if (ret == NULL)
		return NULL;

	if (mbstowcs(ret, str, len+1) != len) {
		free(ret);
		errno = EINVAL;
		return NULL;
	}
	return ret;
}

static bool write_all(const void *ptr, size_t size, size_t count, FILE *stream)
{
	size_t written;
	while (count > 0 && (written = fwrite(ptr, size, count, stream)) > 0) {
		ptr += written;
		count -= written;
	}
	return count == 0;
}

static bool replace_until(char *dest, size_t dest_size,
		const char *src, size_t src_count, size_t stop_at,
		FILE *out, pfx_tree_t substitutions)
{
	size_t dest_offset = 0, tree_offset = 0;
	pfx_tree_iter_t iter = pfx_tree_get_iter(substitutions);

#define MAYBE_WRITE_AND_COPY(src_inc, buf, num) {                        \
		size_t len_ = num, src_inc_ = src_inc;                           \
		if (dest_offset + len_ > dest_size) {                            \
			if (!write_all(dest, sizeof(char), dest_offset, out))        \
				return false;                                            \
			dest_offset = 0;                                             \
		}                                                                \
		memcpy(dest + dest_offset, buf, len_ * sizeof(char));            \
		src += src_inc_;                                                 \
		src_count -= src_inc_;                                           \
		dest_offset += len_;                                             \
		tree_offset = 0;                                                 \
		iter = pfx_tree_get_iter(substitutions);                         \
	}

	while(src_count > stop_at) {
		pfx_tree_iter_t next = pfx_tree_iter_next(iter, src[tree_offset]);
		++tree_offset;
		if (next == NULL) {
			MAYBE_WRITE_AND_COPY(1, src, 1);
			continue;
		}
		iter = next;

		char *replacement = pfx_tree_iter_data(iter);
		if (replacement != NULL)
			MAYBE_WRITE_AND_COPY(tree_offset, replacement, strlen(replacement));
	}
	return write_all(dest, sizeof(char), dest_offset, out);
}

bool substitute_file(const char *dest_fn, const char *src_fn,
		pfx_tree_t substitutions, size_t longest_replacement)
{
	FILE *in = fopen(src_fn, "r"), *out = fopen(dest_fn, "w");
	bool ret = false;
	if (in == NULL || out == NULL)
		goto substitute_cleanup;

	{
		size_t height = pfx_tree_height(substitutions);
		size_t sbuf_size = (height+1) * 5, offset = 0;
		size_t obuf_size = (longest_replacement+1) * 5;
		if (sbuf_size < MIN_BUF_SIZE)
			sbuf_size = MIN_BUF_SIZE;
		char sbuf[sbuf_size], obuf[obuf_size];
		ssize_t in_bytes, replaced;

		while((in_bytes = fread(sbuf + offset, sizeof(char),
						sbuf_size - offset, in)) > 0) {
			offset += in_bytes;
			if (offset < height)
				continue;

			if (!replace_until(obuf, obuf_size, sbuf, offset, height,
						out, substitutions))
				goto substitute_cleanup;

			memmove(sbuf, sbuf + offset - height, height);
			offset = height;
		}
		if (in_bytes == -1)
			goto substitute_cleanup;
		if (!replace_until(obuf, obuf_size, sbuf, offset, 0,
					out, substitutions))
			goto substitute_cleanup;
	}

	ret = true;
substitute_cleanup:
	if (in != NULL)
		fclose(in);
	if (out != NULL)
		fclose(out);
	return ret;
}
