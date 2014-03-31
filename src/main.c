/*
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

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include "pfx_tree.h"

static const char opts[] = "hr:";
static const struct option long_opts[] = {
	{
		.name = "help",
		.has_arg = no_argument,
		.flag = NULL,
		.val = 'h'
	},
	{
		.name = "replace",
		.has_arg = required_argument,
		.flag = NULL,
		.val = 'r'
	},
	NULL
};

static bool get_two_subopts(int argc, char *argv[], char **opt1, char **opt2)
{
	if (optind >= argc || argv[optind][0] == '-') {
		fprintf(stderr, "Not enough arguments for that option\n");
		return false;
	}

	*opt1 = optarg;
	*opt2 = argv[optind];
	++optind;
	return true;
}

static wchar_t *from_char(const char *str)
{
	size_t len = strlen(str);
	wchar_t *ret = malloc((len+1) * sizeof(wchar_t));

	if (ret == NULL)
		return NULL;

	for (size_t i = 0; i < len; ++i)
		ret[i] = str[i];
	return ret;
}

int main(int argc, char *argv[])
{
	int opt_ret, main_ret = EXIT_FAILURE;
	pfx_tree_t substitutions;

	substitutions = pfx_tree_init();
	if (substitutions == NULL) {
		fprintf(stderr, "Failed to allocate the subsitution tree\n");
		goto main_cleanup;
	}

	while ((opt_ret = getopt_long(argc, argv, opts, long_opts, NULL)) != -1) {
		char *opt1, *opt2;
		switch (opt_ret) {
			case 'r':
				if (!get_two_subopts(argc, argv, &opt1, &opt2))
					goto main_print_help;

				wchar_t *key_str = from_char(opt1);
				if (key_str == NULL) {
					fprintf(stderr,
							"Failed to allocate memory for replacement keys\n");
					goto main_cleanup;
				}

				pfx_tree_insert_safe(substitutions,
						key_str, strlen(opt1), opt2);
				free(key_str);
				break;
			case 'h':
			default:
				goto main_print_help;
		}
	}
	argv += optind;
	argc -= optind;

	if (argc != 2) {
		fprintf(stderr, "You must pass a SRC and DEST file\n");
		goto main_print_help;
	}

	// TODO(wak): Perform replace action in the file

	main_ret = EXIT_SUCCESS;
	goto main_cleanup;

main_print_help:
	fprintf(stderr, "Usage: substitute [OPTION] SRC DEST\n");
	fprintf(stderr, "Example: substitute -r foo bar in.txt out.txt\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "  -h, --help                          "
			"Displays this help text\n");
	fprintf(stderr, "  -r, --replace=NEEDLE REPLACEMENT    "
			"Replaces the NEEDLE in the source text with REPLACEMENT\n");
main_cleanup:
	pfx_tree_destroy(substitutions);
	return main_ret;
}
