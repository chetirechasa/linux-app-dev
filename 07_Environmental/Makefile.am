bin_PROGRAMS = rhasher
rhasher_SOURCES = rhasher.c
rhasher_CFLAGS = $(RHASH_CFLAGS)
rhasher_LDADD = $(RHASH_LIBS)

if USE_READLINE
rhasher_CFLAGS += $(READLINE_CFLAGS)
rhasher_LDADD += $(READLINE_LIBS)
endif

TESTS = test_rhasher.sh
EXTRA_DIST = $(TESTS)

# Очистка
CLEANFILES = *~

