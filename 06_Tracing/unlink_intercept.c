#define _GNU_SOURCE
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>

int unlink(const char *pathname) {
    static int (*original_unlink)(const char *) = NULL;
    if (!original_unlink) {
        original_unlink = dlsym(RTLD_NEXT, "unlink");
    }

    if (strstr(pathname, "PROTECT") != NULL) {
        printf("File '%s' is protected! Deletion blocked.\n", pathname);
        return 0;
    }

    return original_unlink(pathname);
}
