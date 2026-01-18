/**
 * @file logingen.c
 * @brief Implementation of system login generation utilities.
 *
 * This file contains helper functions for validating user names,
 * transforming them, and generating a system login string
 * in the format:
 *
 * @code
 * system-<username>-<random_number>
 * @endcode
 *
 * The generated login uses only lowercase Latin letters and
 * a random numeric suffix.
 */

#include "logingen.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <config.h>

int is_latin_only(const char* name) {
    if (!name) return 0;
    
    for (size_t i = 0; name[i] != '\0'; i++) {
        char c = name[i];
        // check latin letters only
        if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
            return 0;
        }
    }
    return 1;
}

char* to_lowercase(const char* name) {
    if (!name) return NULL;
    
    size_t len = strlen(name);
    char* result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        char c = name[i];
        if (c >= 'A' && c <= 'Z') {
            result[i] = c + ('a' - 'A');
        } else {
            result[i] = c; // each case to lower case
        }
    }
    result[len] = '\0';
    
    return result;
}

void logingen_init(void) {
    static int initialized = 0;
    if (!initialized) {
        srand(time(NULL));
        initialized = 1;
    }
}

char* generate_login(const char* name) {
    if (!name || !*name) return NULL;
    
    if (!is_latin_only(name)) {
        return NULL;
    }
    
    logingen_init();
    
    char* lower_name = to_lowercase(name);
    if (!lower_name) return NULL;
    
    int random_num = rand() % 10000;
    size_t name_len = strlen(lower_name);
    size_t total_len = 7 + name_len + 1 + 4 + 1;
    char* login = malloc(total_len);
    if (!login) {
        free(lower_name);
        return NULL;
    }
    
    snprintf(login, total_len, "system-%s-%04d", lower_name, random_num);
    
    free(lower_name);
    return login;
}
