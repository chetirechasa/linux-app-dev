#include <check.h>
#include <../lib/logingen.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

START_TEST(test_is_latin_only) {
    ck_assert_int_eq(is_latin_only("Ivan"), 1);
    ck_assert_int_eq(is_latin_only("ALICE"), 1);
    ck_assert_int_eq(is_latin_only("ivan123"), 0);  // содержит цифры
    ck_assert_int_eq(is_latin_only("ivan-roman"), 0); // содержит дефис
    ck_assert_int_eq(is_latin_only("ivan roman"), 0); // содержит пробел
    ck_assert_int_eq(is_latin_only("Иван"), 0);     // содержит кириллицу
    ck_assert_int_eq(is_latin_only(""), 1);         // пустая строка
    ck_assert_int_eq(is_latin_only(NULL), 0);       // NULL
}
END_TEST

START_TEST(test_to_lowercase) {
    char* result = to_lowercase("Ivan");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "ivan");
    free(result);
    
    result = to_lowercase("ALICE");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "alice");
    free(result);
    
    result = to_lowercase("ivan");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "ivan");
    free(result);
    
    result = to_lowercase("");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "");
    free(result);
    
    result = to_lowercase(NULL);
    ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_generate_login_format) {
    srand(12345);
    
    char* login = generate_login("Ivan");
    ck_assert_ptr_nonnull(login);
    
    ck_assert(strncmp(login, "system-ivan-", 12) == 0);
    
    // проверяем, что после дефиса идет 4 цифры
    char* number_part = login + 12; // после "system-ivan-"
    ck_assert_int_eq(strlen(number_part), 4);
    
    for (int i = 0; i < 4; i++) {
        ck_assert(isdigit(number_part[i]));
    }
    
    free(login);
}
END_TEST

START_TEST(test_generate_login_invalid) {
    ck_assert_ptr_null(generate_login(NULL));
    ck_assert_ptr_null(generate_login(""));
    ck_assert_ptr_null(generate_login("Иван"));
}
END_TEST

START_TEST(test_init) {
    logingen_init();
    ck_assert(1);
}
END_TEST

Suite* logingen_suite(void) {
    Suite* s = suite_create("LoginGen");
    
    TCase* tc_core = tcase_create("Core Functions");
    tcase_add_test(tc_core, test_is_latin_only);
    tcase_add_test(tc_core, test_to_lowercase);
    tcase_add_test(tc_core, test_init);
    tcase_add_test(tc_core, test_generate_login_format);
    tcase_add_test(tc_core, test_generate_login_invalid);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void) {
    int number_failed;
    Suite* s = logingen_suite();
    SRunner* sr = srunner_create(s);
    
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    
    return (number_failed == 0) ? 0 : 1;
}