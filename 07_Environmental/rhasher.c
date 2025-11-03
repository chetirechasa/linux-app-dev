#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <rhash.h>

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#define MAX_HASH_SIZE 128

typedef enum {
    OUTPUT_HEX,
    OUTPUT_BASE64
} output_format_t;

typedef struct {
    const char* name;
    int id;
} hash_algo_t;

const hash_algo_t algorithms[] = {
    {"MD5", RHASH_MD5},
    {"SHA1", RHASH_SHA1},
    {"TTH", RHASH_TTH},
    {NULL, 0}
};

const hash_algo_t* find_algorithm(const char* name) {
    for (const hash_algo_t* algo = algorithms; algo->name != NULL; algo++) {
        if (strcasecmp(algo->name, name) == 0) {
            return algo;
        }
    }
    return NULL;
}

void calculate_hash(const hash_algo_t* algo, const char* data, size_t len, output_format_t format) {
    unsigned char digest[MAX_HASH_SIZE];
    char result[MAX_HASH_SIZE * 2];
    
    if (rhash_msg(algo->id, data, len, digest) < 0) {
        fprintf(stderr, "Error calculating hash\n");
        return;
    }
    
    if (format == OUTPUT_HEX) {
        rhash_print_bytes(result, digest, rhash_get_digest_size(algo->id), RHPR_HEX | RHPR_UPPERCASE);
    } else {
        rhash_print_bytes(result, digest, rhash_get_digest_size(algo->id), RHPR_BASE64);
    }
    
    printf("%s\n", result);
}

void process_command(char* command) {
    char* algo_name = strtok(command, " \t\n");
    if (!algo_name) return;
    
    char* target = strtok(NULL, " \t\n");
    if (!target) {
        fprintf(stderr, "Error: missing target\n");
        return;
    }
    
    const hash_algo_t* algo = find_algorithm(algo_name);
    if (!algo) {
        fprintf(stderr, "Error: unknown algorithm '%s'\n", algo_name);
        return;
    }
    
    output_format_t format = isupper((unsigned char)algo_name[0]) ? OUTPUT_HEX : OUTPUT_BASE64;
    
    if (target[0] == '"') {
        size_t len = strlen(target);
        if (target[len-1] == '"') {
            target[len-1] = '\0';
            target++;
        }
        calculate_hash(algo, target, strlen(target), format);
    } else {
        FILE* file = fopen(target, "rb");
        if (!file) {
            fprintf(stderr, "Error: cannot open file '%s'\n", target);
            return;
        }
        
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        char* buffer = malloc(file_size);
        if (!buffer) {
            fclose(file);
            fprintf(stderr, "Error: memory allocation failed\n");
            return;
        }
        
        if (fread(buffer, 1, file_size, file) != (size_t)file_size) {
            fprintf(stderr, "Error: reading file failed\n");
            free(buffer);
            fclose(file);
            return;
        }
        
        calculate_hash(algo, buffer, file_size, format);
        free(buffer);
        fclose(file);
    }
}

int main() {
    rhash_library_init();
    char* line = NULL;
    
    while (1) {
#ifdef USE_READLINE
        line = readline("rhasher> ");
        if (!line) break;
        if (*line) add_history(line);
#else
        size_t len = 0;
        ssize_t read = getline(&line, &len, stdin);
        if (read == -1) {
            free(line);
            break;
        }
#endif
        
        process_command(line);
        
#ifdef USE_READLINE
        free(line);
#else
        free(line);
        line = NULL;
#endif
    }
    
    return 0;
}
