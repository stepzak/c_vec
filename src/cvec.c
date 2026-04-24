#include "../include/cvec/cvec.h"
#include "unlikely.h"

#include <stdlib.h>
#include <string.h>

[[nodiscard]] VecStatus vector_reserve_impl(void** data, size_t element_size, size_t capacity) {
    size_t new_size = sizeof(VecHeader) + capacity * element_size;
    void* new_ptr = realloc(*data ? v_header(*data) : NULL, new_size);
    if (unlikely(!new_ptr)) return CVEC_ERROR_BAD_ALLOC;

    VecHeader* h = new_ptr;
    if (!*data) h->length = 0;
    h->capacity = capacity;
    *data = (void*)(h + 1);
    return CVEC_SUCCESS;
}

[[nodiscard]] VecStatus vector_grow_impl(void** data, size_t element_size) {
    size_t new_cap = *data == NULL ? VEC_INIT_CAP : v_header(*data)->capacity * 2;
    return vector_reserve_impl(data, element_size, new_cap);
}

void vector_remove_impl(void** data, size_t element_size, size_t index) {
    if (unlikely(data == NULL || *data == NULL)) return;
    VecHeader* h = v_header(*data);
    assert(index < h->length && "Index out of range");
    if (index < h->length - 1) {
        char* base = (char*) *data;
        char* dest = base + index * element_size;
        char* src = base + (index + 1) * element_size;

        size_t n = (h->length - index - 1) * element_size;

        memmove((void*) dest, (void*) src, n);
    }
    h->length--;
}

VecStatus vector_shrink_impl(void** data, size_t element_size) {
    if (!data || !*data) return CVEC_SUCCESS;

    size_t len = v_header(*data)->length;
    if (len == 0) {
        free(v_header(*data));
        *data = NULL;
        return CVEC_SUCCESS;
    }

    size_t new_cap = len;
    size_t new_size = sizeof(VecHeader) + (new_cap * element_size);

    void* new_ptr = realloc(v_header(*data), new_size);
    if (unlikely(!new_ptr)) return CVEC_ERROR_BAD_ALLOC;

    VecHeader* h = new_ptr;
    h->capacity = new_cap;

    *data = (void*)(h + 1);
    return CVEC_SUCCESS;
}
