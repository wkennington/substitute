@VALGRIND_CHECK_RULES@

TESTS = check_pfx_tree check_util
check_PROGRAMS = check_pfx_tree check_util

check_pfx_tree_SOURCES = pfx_tree.c ../src/pfx_tree.c
check_pfx_tree_CFLAGS = $(AM_CFLAGS) $(CHECK_CFLAGS)
check_pfx_tree_LDADD = $(LDADD) $(CHECK_LIBS)

check_util_SOURCES = util.c ../src/pfx_tree.c ../src/util.c
check_util_CFLAGS = $(AM_CFLAGS) $(CHECK_CFLAGS)
check_util_LDADD = $(LDADD) $(CHECK_LIBS)
