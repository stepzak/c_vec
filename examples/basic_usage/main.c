#include <stdio.h>

#include <cvec/cvec.h>

int main() {
    Vec(int) vec = v_from_args(int, -1, -2, -3, -4);
    for (size_t i = 0; i < 10; i++) {
        VecStatus s = v_push(vec, i);
        if (s != CVEC_SUCCESS) {
            fprintf(stderr, "Error in v_push()\n");
            return 1;
        }
    }
    v_foreach(int, it, vec) {
        printf("%d\n", *it);
    }
    v_pop(vec);
    printf("Len: %zu\n", v_len(vec));
    printf("%d\n", vec[0]);
    v_free(vec);
    return 0;
}