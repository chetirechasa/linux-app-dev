#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>

#define _(STRING) gettext(STRING)
#define N_(STRING) (STRING)

int main() {
    setlocale(LC_ALL, "");
    bindtextdomain("guess-game", LOCALEDIR);
    textdomain("guess-game");
    
    int low = 1;
    int high = 100;
    int guess;
    char response[100];
    int attempts = 0;
    
    printf(_("Think of a number between 1 and 100.\n"));
    printf(_("I'll try to guess it. Answer 'y' for yes, 'n' for no.\n\n"));
    
    do {
        attempts++;
        guess = (low + high) / 2;
        
        if (low == high) {
            printf(_("Your number is %d! I guessed it in %d attempts!\n"), 
                   low, attempts);
            break;
        }
        
        printf(_("Is your number greater than %d? [y/n]: "), guess);
        
        if (fgets(response, sizeof(response), stdin) == NULL) {
            fprintf(stderr, _("Error: input closed.\n"));
            return 1;
        }
        response[strcspn(response, "\n")] = 0;
        
        if (strcmp(response, _("y")) == 0) {
            low = guess + 1;
        } else if (strcmp(response, _("n")) == 0) {
            high = guess;
        } else {
            printf(_("Error: please answer 'y' or 'n' only.\n"));
            attempts--;
            continue;
        }
        
    } while (1);
    
    return 0;
}
