#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define ERROR_ARGC 1
#define ERROR_OPEN_IN 2
#define ERROR_OPEN_OUT 3
#define ERROR_READ 4
#define ERROR_WRITE 5
#define ERROR_CLOSE_OUT 6
#define ERROR_CLOSE_IN 7
#define ERROR_UNLINK_IN 8
#define ERROR_FSTAT 9

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <infile> <outfile>\n", argv[0]);
        return ERROR_ARGC;
    }

    const char *infile = argv[1];
    const char *outfile = argv[2];
    int fd_in = -1, fd_out = -1;
    char *buffer = NULL;
    struct stat st;
    int exit_code = 0;

    // 1. Открываем исходный файл
    fd_in = open(infile, O_RDONLY);
    if (fd_in == -1) {
        perror("Error opening source file");
        exit_code = ERROR_OPEN_IN;
        goto cleanup;
    }

    // 2. Получаем информацию о файле
    if (fstat(fd_in, &st) == -1) {
        perror("Error getting file info");
        exit_code = ERROR_FSTAT;
        goto cleanup;
    }

    // 3. Выделяем память для содержимого файла
    buffer = malloc(st.st_size);
    if (buffer == NULL) {
        perror("Memory allocation failed");
        exit_code = ERROR_READ;
        goto cleanup;
    }

    // 4. Читаем содержимое файла
    ssize_t bytes_read = read(fd_in, buffer, st.st_size);
    if (bytes_read != st.st_size) {
        perror("Error reading file");
        exit_code = ERROR_READ;
        goto cleanup;
    }

    // 5. Закрываем исходный файл (данные уже в памяти)
    if (close(fd_in) == -1) {
        perror("Error closing source file");
        exit_code = ERROR_CLOSE_IN;
        goto cleanup;
    }
    fd_in = -1;

    // 6. Создаем/перезаписываем целевой файл
    fd_out = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out == -1) {
        perror("Error creating target file");
        exit_code = ERROR_OPEN_OUT;
        goto cleanup;
    }

    // 7. Записываем данные в целевой файл
    ssize_t bytes_written = write(fd_out, buffer, st.st_size);
    if (bytes_written != st.st_size) {
        perror("Error writing to target file");
        exit_code = ERROR_WRITE;
        goto cleanup_outfile;
    }

    // 8. Закрываем целевой файл
    if (close(fd_out) == -1) {
        perror("Error closing target file");
        exit_code = ERROR_CLOSE_OUT;
        goto cleanup_outfile;
    }
    fd_out = -1;

    // 9. Удаляем исходный файл (все прошло успешно)
    if (unlink(infile) == -1) {
        perror("Error removing source file");
        exit_code = ERROR_UNLINK_IN;
        goto cleanup_outfile;
    }

    free(buffer);
    return 0;

cleanup_outfile:
    if (fd_out != -1) {
        close(fd_out);
    }
    unlink(outfile);

cleanup:
    if (buffer) free(buffer);
    if (fd_in != -1) close(fd_in);
    return exit_code;
}
