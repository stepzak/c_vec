#include "../include/cvec/cvec.h"
#include "unlikely.h"

#include <stdlib.h>

[[nodiscard]] VecStatus vector_grow_impl(void** data, size_t element_size) {
    size_t new_cap = *data == NULL ? VEC_INIT_CAP : v_header(*data)->capacity * 2;
    size_t new_size = sizeof(VecHeader) + new_cap * element_size;

    void* new_ptr = realloc(*data ? v_header(*data) : NULL, new_size);
    if (unlikely(!new_ptr)) return CVEC_ERROR_BAD_ALLOC;

    VecHeader* h = new_ptr;
    if (!*data) h->length = 0;
    h->capacity = new_cap;

    *data = (void*)(h + 1);
    return CVEC_SUCCESS;
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
