CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -ldl

all: move libprotect.so

move: move.c
	$(CC) $(CFLAGS) -o move move.c

libprotect.so: unlink_intercept.c
	$(CC) $(CFLAGS) -shared -fPIC -o libprotect.so unlink_intercept.c $(LDFLAGS)

test: move libprotect.so
	@echo "=== Testing error injection ==="
	@echo "Test 1: Error opening source file"
	@strace -e fault=openat:error=ENOENT:when=1 ./move no_such_file out 2>/dev/null; test $$? -eq 2 && echo "PASS" || echo "FAIL"
	
	@echo "Test 2: Error creating target file"
	@touch test_infile
	@strace -e fault=openat:error=EACCES:when=2 ./move test_infile /root/out 2>/dev/null; test $$? -eq 3 && echo "PASS" || echo "FAIL"
	@rm -f test_infile
	
	@echo "=== Testing LD_PRELOAD protection ==="
	@touch PROTECTed_test.txt
	@LD_PRELOAD=./libprotect.so ./move PROTECTed_test.txt new_location.txt 2>/dev/null
	@test -f PROTECTed_test.txt && echo "PROTECTION PASS" || echo "PROTECTION FAIL"
	@rm -f PROTECTed_test.txt new_location.txt

clean:
	rm -f move libprotect.so

.PHONY: all test clean

