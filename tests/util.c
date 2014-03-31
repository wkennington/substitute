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

#include "../src/util.h"
#include "common.h"

START_TEST(test_utf8_simple)
{
	// TODO(wak): Implement
	ck_assert(0);
}
END_TEST

START_TEST(test_utf8_complex)
{
	// TODO(wak): Implement
	ck_assert(0);
}
END_TEST

START_TEST(test_utf8_invalid)
{
	// TODO(wak): Implement
	ck_assert(0);
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

Suite *substitute_suite()
{
	Suite *s = suite_create("Substitute");
	return s;
}

SRunner *srunner_generate()
{
	SRunner *sr = srunner_create(utf8_suite());
	srunner_add_suite(sr, substitute_suite());
	return sr;
}
