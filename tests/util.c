/*
 * util.c: Test cases for util functions
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

#include <locale.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "../src/util.h"
#include "common.h"

#define BUF_SIZE 4096
#define IN_FILE "util/in"

static void check_string(const char *in, const wchar_t *expected)
{
	wchar_t *ostr = from_utf8(in);
	ck_assert(ostr != NULL);
	ck_assert_int_eq(wcslen(ostr), wcslen(expected));
	ck_assert_int_eq(memcmp(expected, ostr,
				(wcslen(expected)+1) * sizeof(wchar_t)), 0);
}

START_TEST(test_utf8_simple)
{
	check_string("hello", L"hello");
}
END_TEST

START_TEST(test_utf8_complex)
{
	setlocale(LC_ALL, "en_US.UTF-8");
	check_string(u8"\u00A2world", L"\u00A2world");
}
END_TEST

START_TEST(test_utf8_invalid)
{
	ck_assert(from_utf8((char []){ 198, 0 }) == NULL);
	ck_assert_int_eq(errno, EINVAL);
}
END_TEST

Suite *utf8_suite()
{
	Suite *s = suite_create("UTF-8");
	TCASE_ADD(s, "Simple", test_utf8_simple);
	TCASE_ADD(s, "Complex", test_utf8_complex);
	TCASE_ADD(s, "Invalid", test_utf8_invalid);
	return s;
}

static char in[] = "substitute_XXXXXX", out[] = "substitute_XXXXXX";
static int in_fd, out_fd;

static void tmp_destroy()
{
	unlink(in);
	unlink(out);
	close(in_fd);
	close(out_fd);
}

static void tmp_init()
{
	out_fd = mkstemp(out);
	ck_assert_int_ne(out_fd, -1);
	atexit(tmp_destroy);
}

static void assert_file_eq(int fd1, int fd2)
{
	uint8_t buf1[BUF_SIZE], buf2[BUF_SIZE];
	bool init = true;
	size_t off1 = 0, off2 = 0;

	while (true) {
#define READ(num) {                                                            \
			ssize_t bytes##num = read(fd##num, buf##num + off##num,            \
					BUF_SIZE - off##num);                                      \
			ck_assert_int_ne(bytes##num, -1);                                  \
			if (!init && bytes##num + off##num != 0)                           \
				break;                                                         \
			off##num += bytes##num;                                            \
		}
		READ(1);
		READ(2);
		init = false;

		size_t min = off1 < off2 ? off1 : off2;
		ck_assert_int_eq(memcmp(buf1, buf2, min), 0);

		off1 -= min;
		memmove(buf1, buf1 + min, off1);
		off2 -= min;
		memmove(buf2, buf2 + min, off2);
	}
	ck_assert_int_eq(off1, 0);
	ck_assert_int_eq(off2, 0);
}

struct subs {
	wchar_t *key;
	char *val;
};

static void substitute_tester(const char *expected_fn,
		const struct subs *substitutes)
{
	pfx_tree_t tree = pfx_tree_init();
	while (substitutes[0].key != NULL) {
		ck_assert(pfx_tree_insert_safe(tree, substitutes[0].key,
					wcslen(substitutes[0].key), substitutes[0].val));
		++substitutes;
	}
	ck_assert(substitute_file(out, IN_FILE, tree));
	int expected_fd = open(expected_fn, 0);
	ck_assert_int_ne(expected_fd, -1);
	assert_file_eq(out_fd, expected_fd);
	close(expected_fd);
	pfx_tree_destroy(tree);
}

START_TEST(test_substitute_none)
{
	substitute_tester(IN_FILE, (struct subs []) {
			{ .key = NULL, .val = NULL },
	});
}
END_TEST

START_TEST(test_substitute_single)
{
	substitute_tester("util/single.out", (struct subs []) {
			{ .key = L"id", .val = "hello" },
			{ .key = NULL, .val = NULL },
	});
}
END_TEST

START_TEST(test_substitute_multi)
{
	substitute_tester("util/multi.out", (struct subs []) {
			{ .key = L"id", .val = "hello" },
			{ .key = L"ipsum", .val = "world" },
			{ .key = L"mattis", .val = "foobar" },
			{ .key = NULL, .val = NULL },
	});
}
END_TEST

START_TEST(test_substitute_bad_input)
{
	pfx_tree_t tree = pfx_tree_init();
	struct stat before, after;
	ck_assert_int_eq(stat(out, &before), 0);
	ck_assert(!substitute_file(out, "does/not/exist", tree));
	ck_assert_int_eq(stat(out, &after), 0);
	ck_assert(before.st_ctime == after.st_ctime);
	ck_assert(before.st_mtime == after.st_mtime);
	pfx_tree_destroy(tree);
}
END_TEST

Suite *substitute_suite()
{
	Suite *s = suite_create("Substitute");
	TCASE_ADD_CF(s, "None", test_substitute_none, tmp_init, NULL);
	TCASE_ADD_CF(s, "Single", test_substitute_single, tmp_init, NULL);
	TCASE_ADD_CF(s, "Multi", test_substitute_multi, tmp_init, NULL);
	TCASE_ADD_CF(s, "Bad Input", test_substitute_bad_input,
			tmp_init, NULL);
	return s;
}

SRunner *srunner_generate()
{
	SRunner *sr = srunner_create(utf8_suite());
	srunner_add_suite(sr, substitute_suite());
	return sr;
}
