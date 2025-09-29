#include <stdio.h>
#include "fun.h"

void print_help(const char* prog_name) {
    printf("Использование: %s [аргументы]\n", prog_name);
    printf("Программа демонстрирует работу с библиотеками.\n");
}

void process_arguments(int argc, char* argv[]) {
    printf("Обработка %d аргументов:\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("  %d: %s\n", i + 1, argv[i]);
    }
}

