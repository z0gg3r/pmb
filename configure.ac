#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([pmb], [0.0.3], [zocker@10zen.eu])
AC_CONFIG_SRCDIR([src/pmb.c])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([curl], [curl_version_info], [has_libcurl=1], [has_libcurl=0])
AC_CHECK_LIB([sqlite3], [sqlite3_version], [has_sqlite3=1], [has_sqlite3=0])

if test "$has_sqlite3" = "0"; then
	AC_MSG_ERROR([Please install libsqlite3])
fi

if test "$has_libcurl" = "0"; then
	AC_MSG_ERROR([Please install libcurl])
fi

# Checks for header files.
AC_CHECK_HEADERS([string.h getopt.h limits.h stdio.h stdlib.h time.h sys/stat.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
