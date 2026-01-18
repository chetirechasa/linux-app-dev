/**
 * @file logingen.h
 *
 * This header declares functions for validating user names,
 * transforming them, and generating system logins in a unified format.
 *
 * The generated login has the following form:
 * @code
 * system-<lowercase_username>-<random_4_digit_number>
 * @endcode
 *
 * Only Latin letters (`A–Z`, `a–z`) are allowed in user names.
 */

#ifndef LOGINGEN_H
#define LOGINGEN_H

#include <stddef.h>

int is_latin_only(const char* name);

char* to_lowercase(const char* name);

char* generate_login(const char* name);

void logingen_init(void);

#endif /* LOGINGEN_H */
