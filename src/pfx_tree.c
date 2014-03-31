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

#include <stdlib.h>
#include <string.h>
#include "pfx_tree.h"

struct pfx_tree_node {
	/* Assume the children array is always sorted */
	struct pfx_tree_node **children;
	size_t children_count, children_size;
	wchar_t c;
	void *data;
};

/*
 * @return The index of the key if it exists,
 * 	otherwise the index to the right of the missing key
 */
static size_t find_child_idx(struct pfx_tree_node *node, wchar_t key,
		bool *exists)
{
	/* Left index is inclusive, right is exclusive */
	size_t left = 0, right = node->children_count;
	while (left < right) {
		size_t mid = (left+right-1)>>1;
		if (node->children[mid]->c == key) {
			*exists = true;
			return mid;
		}
		if (node->children[mid]->c < key)
			left = mid+1;
		else
			right = mid;
	}
	*exists = false;
	return left;
}

pfx_tree_t pfx_tree_init()
{
	pfx_tree_t tree = malloc(sizeof(struct pfx_tree_node));
	if (tree == NULL)
		return tree;

	tree->children_count = 0;
	tree->children_size = 7;
	tree->children = malloc(sizeof(struct pfx_tree_node *)*tree->children_size);
	tree->data = NULL;
	return tree;
}

void pfx_tree_destroy(pfx_tree_t tree)
{
	if (tree == NULL)
		return;

	for (size_t i = 0; i < tree->children_count; ++i)
		pfx_tree_destroy(tree->children[i]);

	free(tree->children);
	free(tree);
}

bool pfx_tree_insert_safe(pfx_tree_t tree, const wchar_t key[],
		size_t key_size, void *value)
{
	while (key_size > 0) {
		/* Guarantee we will reach the data of the key */
		if (tree->data != NULL)
			return false;

		bool exists;
		size_t idx = find_child_idx(tree, key[0], &exists);
		if (!exists) {
			/* Reallocate the array if too small */
			if (tree->children_count == tree->children_size) {
				tree->children_size <<= 1;
				struct pfx_tree_node **resized =
					realloc(tree->children, tree->children_size);
				if (resized == NULL) {
					tree->children_size >>= 1;
					return false;
				}
				tree->children = resized;
			}

			/* Insertion into children */
			memmove(tree->children+idx+1, tree->children+idx,
					tree->children_count-idx);
			++tree->children_count;
			tree->children[idx] = pfx_tree_init();
			if (tree->children[idx] == NULL)
				return false;
		}

		tree = tree->children[idx];
		++key;
		--key_size;
	}

	tree->data = value;
	return true;
}

pfx_tree_iter_t pfx_tree_get_iter(pfx_tree_t tree)
{
	return tree;
}

pfx_tree_iter_t pfx_tree_iter_next(pfx_tree_iter_t iter, wchar_t c)
{
	bool exists;
	size_t idx = find_child_idx(iter, c, &exists);
	if (!exists)
		return NULL;
	return iter->children[idx];
}

void *pfx_tree_iter_data(pfx_tree_iter_t iter)
{
	return iter->data;
}
