#include <stdio.h>

#define N 1024

int a[N];
int b[N];

int kernel() {
    int i;
    for (i = 0; i < N; i++) {
        a[i] = i;
    }

    #ifdef CGRA_COMPILER
    loop_begin();
    #endif

    for (i = 0; i < N-2; i++) {
        b[i] = a[i] * 10 + a[i + 1] * 20 + a[i+ 2] * 3;
    }

    #ifdef CGRA_COMPILER
    loop_end();
    #endif

    return 0;
}
