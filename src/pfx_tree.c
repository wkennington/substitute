/*
 * pfx_tree.c: prefix tree for string searches
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

#include <string.h>
#include "pfx_tree.h"

struct pfx_tree_node {
	struct pfx_tree_node *children;
	size_t children_count, children_size;
	wchar_t c;
	void *data;
};

pfx_tree_t pfx_tree_init()
{
	pfx_tree_t tree = malloc(sizeof(struct pfx_tree_node));
	if (tree == NULL)
		return tree;

	tree->children_count = 0;
	tree->children_size = 7;
	tree->children = malloc(sizeof(struct pfx_tree_node *) * tree->children_size);
	tree->data = NULL;
	return tree;
}

void pfx_tree_destroy(pfx_tree_t tree)
{
	for (size_t i = 0; i < tree->num_children; ++i)
		pfx_tree_destroy(tree->children[i]);

	free(tree->children);
	free(tree);
}

bool pfx_tree_insert_safe(pfx_tree_t tree, const wchar_t key[], size_t key_size, void *value)
{
	while (key_size > 0) {
		if (tree->data != NULL)
			return false;
	}
	return true;
}

pfx_tree_iter_t pfx_tree_get_iter(pfx_tree_t tree)
{
	return NULL;
}

pfx_tree_iter_t pfx_tree_iter_next(pfx_tree_iter_t iter, wchar_t c)
{
	return NULL;
}

void *pfx_tree_iter_data(pfx_tree_iter_t iter)
{
	return NULL;
}
