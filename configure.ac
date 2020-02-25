AC_PREREQ([2.64])
AC_INIT([substitute],
        [0.1.0],
        [https://github.com/wkennington/substitute],
        [substitute],
        [william@wkennington.com])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 foreign -Wall -Wno-portability -Werror silent-rules subdir-objects dist-xz tar-ustar])
AM_MAINTAINER_MODE([enable])

# Make sure the default CFLAGS of `-O2 -g` don't override CODE_COVERAGE_CFLAGS
# It is important that this comes before AC_PROG_C{C,XX}, as we are attempting
# to stop them from populating default CFLAGS and CXXFLAGS.
AS_IF([test "x$enable_tests" = "xno"], [enable_code_coverage=no])
AS_IF([test "x$enable_code_coverage" != "xno"], [
    AS_IF([test "x${CXXFLAGS+set}" != "xset"], [
        AC_SUBST(CXXFLAGS, [""])
    ])
    AS_IF([test "x${CFLAGS+set}" != "xset"], [
        AC_SUBST(CFLAGS, [""])
    ])
])

AC_PROG_CC_C99
AM_PROG_CC_C_O

LT_PREREQ([2.2])
LT_INIT

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([CHECK], [check >= 0.9.4], [], [
   AS_IF([test "x$enable_tests" = "xyes"], [AC_MSG_ERROR([CHECK required])])
])

# Make it possible for users to choose if they want test support
# explicitly or not at all
AC_ARG_ENABLE([tests], AC_HELP_STRING([--disable-tests],
                                      [Build test cases]))

# Check for valgrind
AS_IF([test "x$enable_tests" = "xno"], [enable_valgrind=no])
m4_foreach([vgtool], [valgrind_tool_list],
    [AX_VALGRIND_DFLT(vgtool, [off])])
AX_VALGRIND_DFLT([memcheck], [on])
AX_VALGRIND_CHECK
AM_EXTRA_RECURSIVE_TARGETS([check-valgrind])
m4_foreach([vgtool], [valgrind_tool_list],
    [AM_EXTRA_RECURSIVE_TARGETS([check-valgrind-]vgtool)])

# Code coverage
AX_CODE_COVERAGE
AS_IF([test "x$CODE_COVERAGE_ENABLED" = "xyes"], [
    AX_APPEND_COMPILE_FLAGS([-DHAVE_GCOV], [CODE_COVERAGE_CPPFLAGS])
])
m4_ifdef([_AX_CODE_COVERAGE_RULES], [AC_SUBST([CODE_COVERAGE_RULES], [])], [])
AX_ADD_AM_MACRO_STATIC([])

# Append -Werror after doing autoconf compiler checks
# Otherwise some perfectly valid checks can fail and cause our
# final configuratin to be broken.
AC_ARG_ENABLE([werror], AC_HELP_STRING([--disable-werror], [Whether to automatically add -Werror CFLAGS]))
AC_SUBST([WERROR], [$enable_werror])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    tests/Makefile
])
AC_OUTPUT
