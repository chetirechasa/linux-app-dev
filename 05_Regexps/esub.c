#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

// Максимальное количество карманов (9 карманов + полное совпадение)
#define MAX_MATCHES 10
// Максимальная длина ошибки
#define ERROR_BUF_SIZE 256

// Функция для обработки строки замены
// Возвращает новую строку или NULL при ошибке
char* process_replacement(const char* replacement, 
                         regmatch_t matches[], 
                         const char* original_string,
                         int* error) {
    *error = 0;
    
    // Вычисляем длину результирующей строки
    size_t new_len = 0;
    const char* p = replacement;
    
    while (*p) {
        if (*p == '\\') {
            p++;  // Пропускаем обратный слеш
            
            if (*p == '\0') {
                *error = 1;  // Одиночный обратный слеш в конце
                return NULL;
            }
            
            if (*p == '\\') {
                // Экранированный обратный слеш
                new_len++;
                p++;
            } else if (isdigit(*p)) {
                int num = *p - '0';  // Номер кармана
                p++;
                
                if (num >= MAX_MATCHES) {
                    *error = 2;  // Номер кармана больше 9
                    return NULL;
                }
                
                // Проверяем, существует ли карман
                if (matches[num].rm_so == -1) {
                    *error = 3;  // Карман не существует
                    return NULL;
                }
                
                // Добавляем длину кармана
                new_len += matches[num].rm_eo - matches[num].rm_so;
            } else {
                *error = 4;  // Неизвестная escape-последовательность
                return NULL;
            }
        } else {
            new_len++;
            p++;
        }
    }
    
    // Выделяем память для новой строки
    char* result = malloc(new_len + 1);
    if (!result) {
        *error = 5;  // Ошибка выделения памяти
        return NULL;
    }
    
    // Собираем результирующую строку
    char* dest = result;
    p = replacement;
    
    while (*p) {
        if (*p == '\\') {
            p++;  // Пропускаем обратный слеш
            
            if (*p == '\\') {
                *dest++ = '\\';
                p++;
            } else if (isdigit(*p)) {
                int num = *p - '0';
                p++;
                
                // Копируем содержимое кармана
                int start = matches[num].rm_so;
                int end = matches[num].rm_eo;
                int len = end - start;
                
                if (len > 0) {
                    strncpy(dest, original_string + start, len);
                    dest += len;
                }
            }
        } else {
            *dest++ = *p++;
        }
    }
    
    *dest = '\0';
    return result;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Использование: %s <regexp> <substitution> <string>\n", argv[0]);
        fprintf(stderr, "Пример: %s '([0-9]+)' 'ЧИСЛО: \\\\1' 'Тест 123 тест'\n", argv[0]);
        return 1;
    }
    
    const char* regex_pattern = argv[1];
    const char* replacement = argv[2];
    const char* input_string = argv[3];
    
    regex_t regex;
    int ret;
    char error_buf[ERROR_BUF_SIZE];
    
    // Компилируем регулярное выражение с поддержкой расширенных регулярных выражений
    ret = regcomp(&regex, regex_pattern, REG_EXTENDED);
    if (ret != 0) {
        regerror(ret, &regex, error_buf, sizeof(error_buf));
        fprintf(stderr, "Ошибка компиляции регулярного выражения: %s\n", error_buf);
        return 2;
    }
    
    // Выполняем поиск совпадений
    regmatch_t matches[MAX_MATCHES];
    ret = regexec(&regex, input_string, MAX_MATCHES, matches, 0);
    
    if (ret == REG_NOMATCH) {
        // Совпадений нет - выводим исходную строку
        printf("%s\n", input_string);
        regfree(&regex);
        return 0;
    } else if (ret != 0) {
        // Ошибка при выполнении регулярного выражения
        regerror(ret, &regex, error_buf, sizeof(error_buf));
        fprintf(stderr, "Ошибка выполнения регулярного выражения: %s\n", error_buf);
        regfree(&regex);
        return 3;
    }
    
    // Обрабатываем строку замены
    int error = 0;
    char* new_replacement = process_replacement(replacement, matches, input_string, &error);
    
    if (error) {
        switch (error) {
            case 1:
                fprintf(stderr, "Ошибка: одиночный обратный слеш в строке замены\n");
                break;
            case 2:
                fprintf(stderr, "Ошибка: ссылка на карман с номером больше 9\n");
                break;
            case 3:
                fprintf(stderr, "Ошибка: ссылка на несуществующий карман\n");
                break;
            case 4:
                fprintf(stderr, "Ошибка: неизвестная escape-последовательность\n");
                break;
            case 5:
                fprintf(stderr, "Ошибка выделения памяти\n");
                break;
            default:
                fprintf(stderr, "Неизвестная ошибка при обработке строки замены\n");
        }
        regfree(&regex);
        return 4;
    }
    
    // Выводим результат замены
    if (matches[0].rm_so > 0) {
        // Часть до совпадения
        fwrite(input_string, 1, matches[0].rm_so, stdout);
    }
    
    // Замена
    printf("%s", new_replacement);
    
    if (matches[0].rm_eo < (int)strlen(input_string)) {
        // Часть после совпадения
        printf("%s", input_string + matches[0].rm_eo);
    }
    
    printf("\n");
    
    // Освобождаем память
    free(new_replacement);
    regfree(&regex);
    
    return 0;
}

