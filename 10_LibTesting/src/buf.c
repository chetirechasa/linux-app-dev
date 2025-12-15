/* growable-buf - A growable buffer for C99 */
#include "buf.h"
#include <stdlib.h>
#include <string.h>

void *buf__grow(const void *buf, size_t new_len, size_t elem_size) {
    size_t new_cap = buf_cap(buf) * 2;
    if (new_cap < new_len) {
        new_cap = new_len;
    }
    if (new_cap < 16) {
        new_cap = 16;
    }
    
    size_t new_size = offsetof(BufHdr, buf) + new_cap * elem_size;
    BufHdr *new_hdr;
    
    if (buf) {
        new_hdr = realloc(buf__hdr(buf), new_size);
    } else {
        new_hdr = malloc(new_size);
        new_hdr->len = 0;
    }
    
    if (!new_hdr) {
        return NULL;
    }
    
    new_hdr->cap = new_cap;
    return new_hdr->buf;
}

