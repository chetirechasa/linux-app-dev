# Simple Makefile for the project
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Isrc -g

# Check if check library exists
CHECK_EXISTS := $(shell pkg-config --exists check && echo yes)

ifeq ($(CHECK_EXISTS),yes)
    # Try to get libs with pkg-config
    CHECK_LIBS := $(shell pkg-config --libs check 2>/dev/null || echo "-lcheck -lm -lpthread -lrt")
    CHECK_CFLAGS := $(shell pkg-config --cflags check 2>/dev/null)
else
    CHECK_LIBS := 
    CHECK_CFLAGS := 
endif

# Source files
SRC_DIR = src
TEST_DIR = tests
SRC_FILES = $(SRC_DIR)/buf.c
HEADER_FILES = $(SRC_DIR)/buf.h

# Targets
TARGET_LIB = libgrowable-buf.a
TARGET_TEST = test_buf

.PHONY: all lib test clean

all: lib

lib: $(TARGET_LIB)

$(TARGET_LIB): $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) -c $(SRC_FILES) -o buf.o
	ar rcs $@ buf.o
	@echo "Library built: $@"

test: $(TARGET_TEST)
	./$(TARGET_TEST)

# Try multiple linking options
check_test: $(SRC_FILES) $(TEST_DIR)/check_buf.c
	@echo "Trying to compile with Check..."
	@if $(CC) $(CFLAGS) $(CHECK_CFLAGS) $(TEST_DIR)/check_buf.c $(SRC_FILES) -o $(TARGET_TEST) $(CHECK_LIBS) 2>/dev/null; then \
		echo "Success with pkg-config flags"; \
	elif $(CC) $(CFLAGS) $(TEST_DIR)/check_buf.c $(SRC_FILES) -o $(TARGET_TEST) -lcheck -lm -lpthread -lrt 2>/dev/null; then \
		echo "Success with standard check library"; \
	else \
		echo "Check not found or not working. Falling back to simple test..."; \
		$(MAKE) simple_test; \
	fi

# Simple test without Check
simple_test: $(SRC_FILES) $(TEST_DIR)/simple_test.c
	@echo "Compiling simple test..."
	$(CC) $(CFLAGS) $(TEST_DIR)/simple_test.c $(SRC_FILES) -o $(TARGET_TEST)
	@echo "Simple test compiled. Note: Check framework not available."

$(TARGET_TEST):
ifeq ($(CHECK_EXISTS),yes)
	@$(MAKE) check_test
else
	@$(MAKE) simple_test
endif

# Coverage
coverage: CFLAGS += --coverage
coverage: clean $(TARGET_TEST)
	./$(TARGET_TEST) || true
	@echo "Generating coverage report..."
	gcov -b -c buf.c
	@if command -v lcov >/dev/null 2>&1; then \
		lcov --capture --directory . --output-file coverage.info; \
		lcov --remove coverage.info '/usr/*' --output-file coverage.info; \
		lcov --list coverage.info; \
		genhtml coverage.info --output-directory coverage 2>/dev/null || echo "genhtml not available"; \
	fi

clean:
	rm -f *.o *.a $(TARGET_TEST) *.gcda *.gcno *.gcov coverage.info
	rm -rf coverage

