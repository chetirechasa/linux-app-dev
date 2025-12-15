#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buf.h"

#define TEST(cond, msg) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "FAIL: %s (line %d)\n", msg, __LINE__); \
            return 1; \
        } else { \
            printf("OK: %s\n", msg); \
        } \
    } while(0)

int test_basic() {
    printf("=== Test basic operations ===\n");
    int *buf = NULL;
    
    TEST(buf_len(buf) == 0, "Initial length should be 0");
    TEST(buf_cap(buf) == 0, "Initial capacity should be 0");
    
    buf_push(buf, 42);
    TEST(buf_len(buf) == 1, "Length after push should be 1");
    TEST(buf[0] == 42, "First element should be 42");
    
    buf_push(buf, 13);
    TEST(buf_len(buf) == 2, "Length after second push should be 2");
    TEST(buf[1] == 13, "Second element should be 13");
    
    buf_pop(buf);
    TEST(buf_len(buf) == 1, "Length after pop should be 1");
    
    buf_clear(buf);
    TEST(buf_len(buf) == 0, "Length after clear should be 0");
    
    buf_free(buf);
    TEST(buf == NULL, "Buffer should be NULL after free");
    
    return 0;
}

int test_growth() {
    printf("=== Test buffer growth ===\n");
    int *buf = NULL;
    size_t initial_cap = buf_cap(buf);
    
    for (int i = 0; i < 100; i++) {
        buf_push(buf, i);
    }
    
    TEST(buf_len(buf) == 100, "Should have 100 elements");
    TEST(buf_cap(buf) > initial_cap, "Capacity should have grown");
    
    for (int i = 0; i < 100; i++) {
        if (buf[i] != i) {
            printf("FAIL: Element %d should be %d, got %d\n", i, i, buf[i]);
            return 1;
        }
    }
    
    printf("OK: All 100 elements correct\n");
    buf_free(buf);
    return 0;
}

int test_strings() {
    printf("=== Test string buffer ===\n");
    char **buf = NULL;
    
    buf_push(buf, "hello");
    buf_push(buf, "world");
    buf_push(buf, "test");
    
    TEST(buf_len(buf) == 3, "Should have 3 strings");
    TEST(strcmp(buf[0], "hello") == 0, "First string should be 'hello'");
    TEST(strcmp(buf[1], "world") == 0, "Second string should be 'world'");
    TEST(strcmp(buf[2], "test") == 0, "Third string should be 'test'");
    
    buf_free(buf);
    return 0;
}

int test_structs() {
    printf("=== Test struct buffer ===\n");
    typedef struct {
        int x;
        int y;
    } Point;
    
    Point *buf = NULL;
    
    buf_push(buf, ((Point){1, 2}));
    buf_push(buf, ((Point){3, 4}));
    
    TEST(buf_len(buf) == 2, "Should have 2 points");
    TEST(buf[0].x == 1, "First point x should be 1");
    TEST(buf[0].y == 2, "First point y should be 2");
    TEST(buf[1].x == 3, "Second point x should be 3");
    TEST(buf[1].y == 4, "Second point y should be 4");
    
    buf_free(buf);
    return 0;
}

int main() {
    int failed = 0;
    
    failed += test_basic();
    failed += test_growth();
    failed += test_strings();
    failed += test_structs();
    
    if (failed == 0) {
        printf("\n✅ All tests passed!\n");
        return 0;
    } else {
        printf("\n❌ %d test(s) failed\n", failed);
        return 1;
    }
}

