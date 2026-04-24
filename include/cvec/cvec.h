#ifndef VECTOR_LIBRARY_H
#define VECTOR_LIBRARY_H
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#define VEC_INIT_CAP 2

typedef enum {
    CVEC_SUCCESS = 0,
    CVEC_ERROR_BAD_ALLOC,

} VecStatus;

typedef struct {
    size_t capacity;
    size_t length;
} VecHeader;

#define Vec(T) T*

[[nodiscard]] VecStatus vector_grow_impl(void** data, size_t element_size);
[[nodiscard]] VecStatus vector_reserve_impl(void** data, size_t element_size, size_t capacity);
VecStatus vector_shrink_impl(void** data, size_t element_size);
void vector_remove_impl(void** data, size_t element_size, size_t index);

#define v_header(v) ((VecHeader *)(v) - 1)
#define v_len(v) ((v) ? v_header(v)->length : 0)
#define v_cap(v) ((v) ? v_header(v)->capacity : 0)

#endif

#define v_reserve(v, cap) ({ vector_reserve_impl((void**)&(v), sizeof(*(v)), cap); })

#define v_from_args(type, ...) \
    ({ \
        type _tmp[] = { __VA_ARGS__ }; \
        size_t _n = sizeof(_tmp) / sizeof(_tmp[0]); \
        Vec(type) _v = NULL; \
        VecStatus _status = v_reserve(_v, _n); \
        if (_status != CVEC_SUCCESS) { \
            _v = NULL; \
        } else { \
            bool _ok = true; \
            for (size_t _i = 0; _i < _n; _i++) { \
                if (v_push(_v, _tmp[_i]) != CVEC_SUCCESS) { \
                    v_free(_v); \
                    _ok = false; \
                    break; \
                } \
            } \
            if (!_ok) _v = NULL; \
        } \
        _v; \
    })

#define v_with_cap(type, cap) \
    ({ \
        Vec(type) _v = NULL; \
        VecStatus _status = v_reserve(_v, (cap)); \
        (_status == CVEC_SUCCESS) ? _v : NULL; \
    })

#define v_free(v) \
    do { \
        if (v) { \
            free(v_header(v)); \
            (v) = NULL; \
        } \
    } while(0)

#define v_push(v, elem) \
    ({ \
        VecStatus _status = CVEC_SUCCESS; \
        if (v_len(v) >= v_cap(v)) { \
            _status = vector_grow_impl((void**)&(v), sizeof(*(v))); \
        } \
        if (_status == CVEC_SUCCESS) (v)[v_header(v)->length++] = (elem); \
        \
        _status; \
    })

#define v_pop(v) \
    do { \
        if (v_len(v) > 0) { \
            v_header(v)->length--; \
        } \
    } while (0)

#define v_free_with_dtor(v, dtor_func) \
    do { \
        if (v) { \
            for (size_t _i = 0; _i < v_len(v); _i++) { \
                dtor_func(&(v)[_i]); \
            } \
            v_free(v); \
        } \
    } while (0)

#define v_at(v, i) \
    (*({ \
        assert((v) != NULL && "Vector is NULL"); \
        size_t _i = (i); \
        assert(_i < v_header(v)->length && "Out of bounds"); \
        &(v)[_i]; \
    }))

#define v_foreach(T, item, v) \
    for (T *item = (v), *_end = (v) + v_len(v); item < _end; item++)

#define v_shrink(v) ({ vector_shrink_impl((void**)&(v), sizeof(*(v))); })

#define v_remove(v, i) vector_remove_impl((void**)&(v), sizeof(*(v)), (i))

#define v_remove_with_dtor(v, i, dtor) \
    do { \
        size_t _i = i; \
        assert(_i < v_len(v) && "Index out of range"); \
        dtor(&(v)[(_i)]); \
        vector_remove_impl((void**) &(v), sizeof(*(v)), (_i)); \
    } while (0)