#include "gc.h"

#include <stdlib.h>

static void **gc_ptrs = NULL;
static size_t gc_count = 0;
static size_t gc_capacity = 0;

static void GC_Register(void *ptr) {
    if (!ptr) return;
    if (gc_count == gc_capacity) {
        size_t new_cap = gc_capacity == 0 ? 16 : gc_capacity * 2;
        void **new_buf = realloc(gc_ptrs, new_cap * sizeof(void*));
        if (!new_buf) {
            free(ptr);
            abort();
        }
        gc_ptrs = new_buf;
        gc_capacity = new_cap;
    }
    gc_ptrs[gc_count++] = ptr;
}

void *GC_Alloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        exit(69);
    }
    GC_Register(ptr);
    return ptr;
}

static void GC_FreeAll(void) {
    for (size_t i = 0; i < gc_count; i++) {
        free(gc_ptrs[i]);
    }
    free(gc_ptrs);
    gc_ptrs = NULL;
    gc_count = 0;
    gc_capacity = 0;
}

__attribute__((destructor))
static void GC_Cleanup(void) {
    GC_FreeAll();
}