/*
 * pfx_tree.h: prefix tree for string searches
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

#ifndef PFX_TREE_H
#define PFX_TREE_H

typedef struct pfx_tree_node *pfx_tree_t;
typedef struct pfx_tree_node *pfx_tree_iter_t;

pfx_tree_t pfx_tree_init();
void pfx_tree_destroy(pfx_tree_t tree);
bool pfx_tree_insert_safe(pfx_tree_t tree, wchar_t key[], void *value);
pfx_tree_iter_t pfx_tree_get_iter(pfx_tree_t tree);

pfx_tree_iter_t pfx_tree_iter_next(pfx_tree_iter_t iter, wchar_t c);
void *pfx_tree_iter_data(pfx_tree_iter_t iter);

#endif // PFX_TREE_H
