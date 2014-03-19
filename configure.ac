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

AM_INIT_AUTOMAKE([1.11 foreign -Wall -Werror silent-rules subdir-objects check-news])
AM_SILENT_RULES([yes])

LT_PREREQ([2.2])
LT_INIT

AC_PROG_CC_C99

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])
AC_OUTPUT
