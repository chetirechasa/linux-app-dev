AC_INIT([growable-buf], [1.0.0], [])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_PROG_CC

# Инициализация libtool
LT_INIT

# Check for pkg-config
PKG_CHECK_MODULES([CHECK], [check >= 0.9.0])

# Check for coverage support
AC_ARG_ENABLE([coverage],
    [AS_HELP_STRING([--enable-coverage],
        [enable code coverage support])],
    [enable_coverage=$enableval],
    [enable_coverage=no])

AM_CONDITIONAL([COVERAGE_ENABLED], [test "x$enable_coverage" = "xyes"])

if test "x$enable_coverage" = "xyes"; then
    CFLAGS="$CFLAGS --coverage"
    LDFLAGS="$LDFLAGS --coverage"
fi

AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_OUTPUT

