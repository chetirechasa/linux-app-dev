#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([login-gen], [1.0.0], [chipichipi@chapachapa.ru])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])

DX_INIT_DOXYGEN(login-gen)

AM_INIT_AUTOMAKE([foreign subdir-objects])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
LT_INIT

ALL_LINGUAS="ru"

# Checks for libraries.

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.21])

# Checks for header files.
AC_CHECK_HEADERS([libintl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

PKG_CHECK_MODULES([CHECK], [check])

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([setlocale])

AC_CONFIG_FILES([
    Makefile
    Doxyfile
    po/Makefile.in
    src/Makefile
    tests/Makefile
    lib/Makefile
    manual/Makefile
])
AC_OUTPUT
