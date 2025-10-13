#include <stdio.h>
#include <stdlib.h>

// Функция для вывода справки
void print_help(const char* prog_name) {
    printf("Использование: %s [START] STOP [STEP]\n", prog_name);
    printf("Генерирует арифметическую прогрессию аналогично Python range()\n\n");
    printf("Без параметров    - выводит эту справку\n");
    printf("С одним параметром N - [0, 1, ..., N-1]\n");
    printf("С двумя параметрами M, N - [M, M+1, ..., N-1]\n");
    printf("С тремя параметрами M, N, S - [M, M+S, M+2S, ..., N-1]\n");
}

int main(int argc, char* argv[]) {
    int start = 0, stop = 0, step = 1;
    
    // Обработка аргументов
    switch (argc) {
        case 1:
            print_help(argv[0]);
            return 0;
        
        case 2:
            stop = atoi(argv[1]);
            break;
        
        case 3:
            start = atoi(argv[1]);
            stop = atoi(argv[2]);
            break;
        
        case 4:
            start = atoi(argv[1]);
            stop = atoi(argv[2]);
            step = atoi(argv[3]);
            break;
        
        default:
            fprintf(stderr, "Ошибка: слишком много аргументов\n");
            print_help(argv[0]);
            return 1;
    }
    
    // Проверка корректности шага
    if (step == 0) {
        fprintf(stderr, "Ошибка: шаг не может быть равен 0\n");
        return 1;
    }
    
    // Генерация прогрессии
    if (step > 0) {
        for (int i = start; i < stop; i += step) {
            printf("%d\n", i);
        }
    } else {
        for (int i = start; i > stop; i += step) {
            printf("%d\n", i);
        }
    }
    
    return 0;
}

