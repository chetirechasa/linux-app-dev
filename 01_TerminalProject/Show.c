#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define DEFAULT_STRINGS_LIMIT 128
#define BUFFER_SIZE 1024
#define WINDOW_OFFSET 2
#define ESC_KEY 27

typedef struct {
    char **lines;
    size_t count;
    size_t capacity;
} FileContent;

typedef struct {
    size_t vertical_offset;  // Первая видимая строка
    size_t horizontal_offset; // Смещение по горизонтали
} ScrollState;

void cleanup_file_content(FileContent *content) {
    if (content == NULL) return;
    
    for (size_t i = 0; i < content->count; i++) {
        free(content->lines[i]);
    }
    free(content->lines);
}

int read_file(const char *filename, FileContent *content) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return 0;
    }

    content->capacity = DEFAULT_STRINGS_LIMIT;
    content->count = 0;
    content->lines = calloc(sizeof(char *), content->capacity);
    
    if (content->lines == NULL) {
        fclose(file);
        return 0;
    }

    char buffer[BUFFER_SIZE];
    
    while (fgets(buffer, sizeof(buffer), file)) {
        size_t length = strlen(buffer);
        
        // Убираем символ новой строки
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
            length--;
        }

        char *line = malloc(length + 1);
        if (line == NULL) {
            fclose(file);
            return 0;
        }
        strcpy(line, buffer);

        // Увеличиваем емкость при необходимости
        if (content->count >= content->capacity) {
            content->capacity *= 2;
            char **new_lines = realloc(content->lines, sizeof(char *) * content->capacity);
            if (new_lines == NULL) {
                free(line);
                fclose(file);
                return 0;
            }
            content->lines = new_lines;
        }

        content->lines[content->count] = line;
        content->count++;
    }

    fclose(file);
    return 1;
}

void render_window(WINDOW *window, const FileContent *content, const ScrollState *scroll) {
    int max_y, max_x;
    getmaxyx(window, max_y, max_x);
    
    werase(window);
    
    size_t max_line_length = 0;
    
    for (int i = 0; i < max_y && scroll->vertical_offset + i < content->count; i++) {
        const char *line = content->lines[scroll->vertical_offset + i];
        size_t line_length = strlen(line);
        
        if (line_length > max_line_length) {
            max_line_length = line_length;
        }
        
        const char *display_start = line;
        if (line_length > scroll->horizontal_offset) {
            display_start = line + scroll->horizontal_offset;
        } else {
            display_start = "";
        }
        
        mvwaddnstr(window, i, 0, display_start, max_x);
    }
    
    box(window, 0, 0);
    wrefresh(window);
}

void handle_input(int key, ScrollState *scroll, const FileContent *content, WINDOW *window) {
    int max_y, max_x;
    getmaxyx(window, max_y, max_x);
    
    switch (key) {
        case ' ':
        case KEY_DOWN:
            if (scroll->vertical_offset + max_y < content->count) {
                scroll->vertical_offset++;
            }
            break;
            
        case KEY_UP:
            if (scroll->vertical_offset > 0) {
                scroll->vertical_offset--;
            }
            break;
            
        case KEY_NPAGE: // Page Down
            if (scroll->vertical_offset + max_y * 2 < content->count) {
                scroll->vertical_offset += max_y;
            } else if (content->count > max_y) {
                scroll->vertical_offset = content->count - max_y;
            }
            break;
            
        case KEY_PPAGE: // Page Up
            if (scroll->vertical_offset > max_y) {
                scroll->vertical_offset -= max_y;
            } else {
                scroll->vertical_offset = 0;
            }
            break;
            
        case KEY_RIGHT:
            // Для горизонтальной прокрутки нужно найти максимальную длину строки
            // среди видимых строк
            size_t max_line_length = 0;
            for (int i = 0; i < max_y && scroll->vertical_offset + i < content->count; i++) {
                size_t length = strlen(content->lines[scroll->vertical_offset + i]);
                if (length > max_line_length) {
                    max_line_length = length;
                }
            }
            
            if (scroll->horizontal_offset + max_x < max_line_length) {
                scroll->horizontal_offset++;
            }
            break;
            
        case KEY_LEFT:
            if (scroll->horizontal_offset > 0) {
                scroll->horizontal_offset--;
            }
            break;
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FileContent content = {0};
    if (!read_file(argv[1], &content)) {
        return EXIT_FAILURE;
    }

    // Инициализация ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    
    // Вывод имени файла
    printw("File: %s", argv[1]);
    refresh();
    
    // Создание окна для содержимого
    WINDOW *content_window = newwin(
        LINES - 2 * WINDOW_OFFSET,
        COLS - 2 * WINDOW_OFFSET,
        WINDOW_OFFSET,
        WINDOW_OFFSET
    );
    
    keypad(content_window, TRUE);
    box(content_window, 0, 0);
    wrefresh(content_window);
    
    ScrollState scroll = {0};
    int running = 1;
    
    while (running) {
        render_window(content_window, &content, &scroll);
        
        int key = wgetch(content_window);
        
        if (key == ESC_KEY) {
            running = 0;
        } else {
            handle_input(key, &scroll, &content, content_window);
        }
    }
    
    // Очистка
    delwin(content_window);
    endwin();
    cleanup_file_content(&content);
    
    return EXIT_SUCCESS;
}

