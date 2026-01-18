bin_PROGRAMS = login-gen
login_gen_SOURCES = main.c
login_gen_LDADD = $(top_builddir)/lib/liblogingen.la
login_gen_DEPENDENCIES = $(top_builddir)/lib/liblogingen.la
AM_CFLAGS = -I$(top_builddir)/lib -D'LOCALE_PATH="$(prefix)/share/locale"'