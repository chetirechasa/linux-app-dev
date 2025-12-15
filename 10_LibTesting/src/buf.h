/* growable-buf - A growable buffer for C99 */
#ifndef GROWABLE_BUF_H
#define GROWABLE_BUF_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>  /* Добавляем для offsetof */

#define buf__hdr(b) ((BufHdr *)((char *)(b) - offsetof(BufHdr, buf)))

typedef struct {
    size_t len;
    size_t cap;
    char buf[];
} BufHdr;

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0)
#define buf_end(b) ((b) + buf_len(b))
#define buf_sizeof(b) (buf_len(b) * sizeof(*(b)))

#define buf_free(b) \
    do { \
        if (b) { \
            free(buf__hdr(b)); \
            (b) = NULL; \
        } \
    } while (0)

#define buf_fit(b, n) \
    do { \
        if ((n) > buf_cap(b)) { \
            buf__grow((b), (n), sizeof(*(b))); \
        } \
    } while (0)

#define buf_push(b, ...) \
    do { \
        buf_fit((b), 1 + buf_len(b)); \
        (b)[buf__hdr(b)->len] = (__VA_ARGS__); \
        buf__hdr(b)->len++; \
    } while (0)

#define buf_pop(b) \
    do { \
        if (buf_len(b) > 0) { \
            buf__hdr(b)->len--; \
        } \
    } while (0)

#define buf_clear(b) \
    do { \
        if (b) { \
            buf__hdr(b)->len = 0; \
        } \
    } while (0)

void *buf__grow(const void *buf, size_t new_len, size_t elem_size);

#endif /* GROWABLE_BUF_H */

