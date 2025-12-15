#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buf.h"

START_TEST(test_buf_basic) {
    int *buf = NULL;
    
    ck_assert_int_eq(buf_len(buf), 0);
    ck_assert_int_eq(buf_cap(buf), 0);
    
    buf_push(buf, 42);
    ck_assert_int_eq(buf_len(buf), 1);
    ck_assert_int_eq(buf[0], 42);
    
    buf_push(buf, 13);
    ck_assert_int_eq(buf_len(buf), 2);
    ck_assert_int_eq(buf[1], 13);
    
    buf_pop(buf);
    ck_assert_int_eq(buf_len(buf), 1);
    
    buf_clear(buf);
    ck_assert_int_eq(buf_len(buf), 0);
    
    buf_free(buf);
    ck_assert_ptr_null(buf);
}
END_TEST

START_TEST(test_buf_growth) {
    int *buf = NULL;
    size_t initial_cap = buf_cap(buf);
    
    // Push enough elements to trigger growth
    for (int i = 0; i < 100; i++) {
        buf_push(buf, i);
    }
    
    ck_assert_int_eq(buf_len(buf), 100);
    ck_assert_int_gt(buf_cap(buf), initial_cap);
    
    for (int i = 0; i < 100; i++) {
        ck_assert_int_eq(buf[i], i);
    }
    
    buf_free(buf);
}
END_TEST

START_TEST(test_buf_strings) {
    char **buf = NULL;
    
    buf_push(buf, "hello");
    buf_push(buf, "world");
    buf_push(buf, "test");
    
    ck_assert_int_eq(buf_len(buf), 3);
    ck_assert_str_eq(buf[0], "hello");
    ck_assert_str_eq(buf[1], "world");
    ck_assert_str_eq(buf[2], "test");
    
    buf_free(buf);
}
END_TEST

START_TEST(test_buf_structs) {
    typedef struct {
        int x;
        int y;
    } Point;
    
    Point *buf = NULL;
    
    buf_push(buf, ((Point){1, 2}));
    buf_push(buf, ((Point){3, 4}));
    
    ck_assert_int_eq(buf_len(buf), 2);
    ck_assert_int_eq(buf[0].x, 1);
    ck_assert_int_eq(buf[0].y, 2);
    ck_assert_int_eq(buf[1].x, 3);
    ck_assert_int_eq(buf[1].y, 4);
    
    buf_free(buf);
}
END_TEST

Suite *buf_suite(void) {
    Suite *s;
    TCase *tc_core;
    
    s = suite_create("GrowableBuf");
    
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_buf_basic);
    tcase_add_test(tc_core, test_buf_growth);
    tcase_add_test(tc_core, test_buf_strings);
    tcase_add_test(tc_core, test_buf_structs);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;
    
    s = buf_suite();
    sr = srunner_create(s);
    
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

