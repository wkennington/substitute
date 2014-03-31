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
#include <errno.h>

#include "util.h"

wchar_t *from_utf8(const char *str)
{
	size_t len = mbstowcs(NULL, str, 0);
	wchar_t *ret = malloc((len+1) * sizeof(wchar_t));
	if (ret == NULL)
		return NULL;

	if (mbstowcs(ret, str, len) != len) {
		free(ret);
		errno = EINVAL;
		return NULL;
	}
	return ret;
}

bool substitute_file(const char *dest, const char *src, pfx_tree_t substitutions)
{
	// TODO(wak): Implement
	errno = ENOSYS;
	return false;
}
