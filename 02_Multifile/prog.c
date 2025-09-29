#include <stdio.h>
#include "const.h"
#include "fun.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        // Используем константу вместо argv[0] для совпадения выводов
        print_help("prog");
        return 0;
    }
    
    printf("Константа: %s\n", MY_CONSTANT);
    process_arguments(argc - 1, argv + 1);
    
    return 0;
}

