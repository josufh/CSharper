#include "gc.h"

#include <stdlib.h>

#ifdef GC_VERBOSE
#include <stdio.h>
#endif

static void **gc_ptrs = NULL;
static size_t gc_count = 0;
static size_t gc_capacity = 0;

#ifdef GC_VERBOSE
static size_t  *gc_sizes       = NULL;
static size_t   gc_total_bytes = 0;
static size_t   gc_max_alloc   = 0;
#endif

static void GC_Register(void *ptr, size_t size) {
    if (!ptr) return;
    if (gc_count == gc_capacity) {
        size_t new_cap = gc_capacity == 0 ? 16 : gc_capacity * 2;
        void **new_buf = realloc(gc_ptrs, new_cap * sizeof(void*));
        if (!new_buf) {
            free(ptr);
            abort();
        }
        gc_ptrs = new_buf;

#ifdef GC_VERBOSE
        size_t *new_sizes = realloc(gc_sizes, new_cap * sizeof(size_t));
        if (!new_sizes) {
            free(ptr);
            abort();
        }
        gc_sizes = new_sizes;
#endif

        gc_capacity = new_cap;
    }
    gc_ptrs[gc_count] = ptr;

#ifdef GC_VERBOSE
    gc_sizes[gc_count] = size;
    gc_total_bytes += size;
    if (size > gc_max_alloc)
        gc_max_alloc = size;
#endif

    gc_count++;
    (void)size;
}

void *GC_Alloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        exit(69);
    }
    GC_Register(ptr, size);
    return ptr;
}

static void GC_FreeAll(void) {
    for (size_t i = 0; i < gc_count; i++) {
        free(gc_ptrs[i]);
    }

#ifdef GC_VERBOSE
    fprintf(stderr, "[GC] allocations: %zu\n", gc_count);
    fprintf(stderr, "[GC] total bytes: %zu\n", gc_total_bytes);
    fprintf(stderr, "[GC] max single allocation: %zu\n", gc_max_alloc);
    if (gc_count > 0) {
        fprintf(stderr, "[GC] average allocation: %.2f\n",
                (double)gc_total_bytes / (double)gc_count);
    }
    free(gc_sizes);
    gc_sizes = NULL;
    gc_total_bytes = 0;
    gc_max_alloc   = 0;
#endif

    free(gc_ptrs);
    gc_ptrs = NULL;
    gc_count = 0;
    gc_capacity = 0;
}

__attribute__((destructor))
static void GC_Cleanup(void) {
    GC_FreeAll();
}