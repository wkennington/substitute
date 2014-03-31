/*
 * pfx_tree.c: Test cases for the prefix tree implementation
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
#include <check.h>
#include <wchar.h>

#include "../src/pfx_tree.h"

START_TEST(test_init)
{
	pfx_tree_t tree = pfx_tree_init();
	ck_assert(tree != NULL);
	pfx_tree_destroy(tree);
}
END_TEST

static char *get_str(pfx_tree_t tree, wchar_t *str)
{
	pfx_tree_iter_t iter = pfx_tree_get_iter(tree);
	for (size_t i = 0; i < wcslen(str); ++i) {
		iter = pfx_tree_iter_next(iter, str[i]);
		if (iter == NULL)
			return NULL;

		char *data = pfx_tree_iter_data(iter);
		if (data != NULL)
			return data;
	}
	return NULL;
}

START_TEST(test_one)
{
	wchar_t s[] = L"hello";
	pfx_tree_t tree = pfx_tree_init();
	ck_assert(pfx_tree_insert_safe(tree, s, wcslen(s), "data"));
	ck_assert_str_eq(get_str(tree, s), "data");
	pfx_tree_destroy(tree);
}
END_TEST

START_TEST(test_multi)
{
	wchar_t s1[] = L"hello", s2[] = L"world", s3[] = L"hi";
	pfx_tree_t tree = pfx_tree_init();
	ck_assert(pfx_tree_insert_safe(tree, s1, wcslen(s1), "data1"));
	ck_assert(pfx_tree_insert_safe(tree, s2, wcslen(s2), "data2"));
	ck_assert(pfx_tree_insert_safe(tree, s3, wcslen(s3), "data3"));
	ck_assert_str_eq(get_str(tree, s1), "data1");
	ck_assert_str_eq(get_str(tree, s2), "data2");
	ck_assert_str_eq(get_str(tree, s3), "data3");
	pfx_tree_destroy(tree);
}
END_TEST

START_TEST(test_same_prefix_forward)
{
	wchar_t s1[] = L"hello", s2[] = L"hello world";
	pfx_tree_t tree = pfx_tree_init();
	ck_assert(pfx_tree_insert_safe(tree, s1, wcslen(s1), "data1"));
	ck_assert(!pfx_tree_insert_safe(tree, s2, wcslen(s2), "data2"));
	ck_assert_str_eq(get_str(tree, s1), "data1");
	ck_assert(get_str(tree, s2) == NULL);
	pfx_tree_destroy(tree);
}
END_TEST

START_TEST(test_same_prefix_backward)
{
	wchar_t s1[] = L"hello", s2[] = L"hello world";
	pfx_tree_t tree = pfx_tree_init();
	ck_assert(pfx_tree_insert_safe(tree, s2, wcslen(s2), "data2"));
	ck_assert(!pfx_tree_insert_safe(tree, s1, wcslen(s1), "data1"));
	ck_assert_str_eq(get_str(tree, s2), "data2");
	ck_assert(get_str(tree, s1) == NULL);
	pfx_tree_destroy(tree);
}
END_TEST

Suite *pfx_tree_suite()
{
	Suite *s = suite_create("PFX_Tree");

	TCase *tc_init = tcase_create("Init");
	tcase_add_test(tc_init, test_init);
	suite_add_tcase(s, tc_init);

	TCase *tc_one = tcase_create("One");
	tcase_add_test(tc_one, test_one);
	suite_add_tcase(s, tc_one);

	TCase *tc_multi = tcase_create("Multi");
	tcase_add_test(tc_multi, test_multi);
	suite_add_tcase(s, tc_multi);

	TCase *tc_same_prefix_forward = tcase_create("Same Prefix Forward");
	tcase_add_test(tc_same_prefix_forward, test_same_prefix_forward);
	suite_add_tcase(s, tc_same_prefix_forward);

	TCase *tc_same_prefix_backward = tcase_create("Same Prefix Backward");
	tcase_add_test(tc_same_prefix_backward, test_same_prefix_backward);
	suite_add_tcase(s, tc_same_prefix_backward);

	return s;
}

int main(int argc, char *argv[])
{
	int nr_failed;
	SRunner *sr = srunner_create(pfx_tree_suite());
	srunner_run_all(sr, CK_NORMAL);
	nr_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (nr_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
