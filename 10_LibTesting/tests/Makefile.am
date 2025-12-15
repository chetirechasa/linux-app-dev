TESTS = check_buf
check_PROGRAMS = check_buf
check_buf_SOURCES = check_buf.c
check_buf_CFLAGS = $(CHECK_CFLAGS) -I$(top_srcdir)/src
check_buf_LDADD = $(CHECK_LIBS) $(top_builddir)/src/libgrowable-buf.la

if COVERAGE_ENABLED
check_buf_CFLAGS += --coverage
check_buf_LDFLAGS = --coverage
endif

clean-local:
	rm -f *.gcda *.gcno *.gcov

