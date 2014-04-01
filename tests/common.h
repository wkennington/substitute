/*
 * common.h: Common test case code
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

#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <check.h>

#define TCASE_ADD_(suite, name, func)  \
	TCase *tc = tcase_create(name);    \
	tcase_add_test(tc, func);          \
	suite_add_tcase(suite, tc);

#define TCASE_ADD(suite, name, func) { TCASE_ADD_(suite, name, func) }
#define TCASE_ADD_CF(suite, name, func, setup, teardown) { \
	TCASE_ADD_(suite, name, func)                          \
	tcase_add_checked_fixture(tc, setup, teardown);        \
}

SRunner *srunner_generate();

int main(int argc, char *argv[])
{
	int nr_failed;
	SRunner *sr = srunner_generate();
	srunner_set_fork_status(sr, CK_FORK);
	srunner_run_all(sr, CK_NORMAL);
	nr_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (nr_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif // COMMON_H
