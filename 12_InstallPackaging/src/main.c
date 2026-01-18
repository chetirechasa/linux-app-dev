/** @mainpage login-gen
 * 
 * Util to generate system login from user's name in Latin.
 * 
 * To generate a login, pass your name to program input.
 */

#include "../lib/logingen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>
#include <config.h>

/**
 * @file login-gen.c
 * @brief Command-line utility for generating system logins.
 *
 * This program provides a CLI interface to the login generator library
 * (`liblogingen`). It validates user input, supports localization via
 * GNU gettext, and prints a generated system login to standard output.
 *
 * The generated login has the following format:
 * @code
 * system-<username_in_lowercase>-<random_4_digit_number>
 * @endcode
 *
 * The username must consist of Latin letters only (`A–Z`, `a–z`).
 */

#define _(STRING) gettext(STRING)

static void print_help(void) {
    printf(_("Usage: login-gen [OPTIONS] USERNAME\n"));
    printf(_("Generate system login in format: system-<username>-<random_number>\n\n"));
    printf(_("Options:\n"));
    printf(_("  -h, --help    Show this help message\n"));
    printf(_("  -v, --version Show version information\n\n"));
    printf(_("Username must contain only latin letters\n"));
    printf(_("Example: login-gen Ivan\n"));
}

static void print_version(void) {
    printf("login-gen 1.0.0\n");
    printf(_("System login generator\n"));
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "");
    bindtextdomain(PACKAGE, LOCALE_PATH);
    textdomain(PACKAGE);
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            print_version();
            return 0;
        }
    }
    
    if (argc != 2 || argv[1][0] == '-') {
        fprintf(stderr, _("Error: Username is required\n"));
        print_help();
        return 1;
    }
    
    char* username = argv[1];
    
    if (!is_latin_only(username)) {
        fprintf(stderr, _("Error: Username must contain only Latin letters\n"));
        return 1;
    }
    
    char* login = generate_login(username);
    if (!login) {
        fprintf(stderr, _("Error: Failed to generate login\n"));
        return 1;
    }
    
    printf("%s\n", login);
    
    free(login);
    return 0;
}