#include <stdio.h>

#define N 1024

int a[N];
int b[N];

int kernel() {
    int i;
    for (i = 0; i < N; i++) {
        a[i] = i;
    }

//    int a_2 = a[i];
//    int a_1 = a[i + 1];
    for (i = 0; i < N-2; i++) {
        //DFGLoop: loop
#ifdef CGRA_COMPILER
please_map_me();
#endif 
        b[i] = a[i] * 10 + a[i + 1] * 20 + a[i+ 2] * 3;
        /*int a_0 = a[i+2];
        b[i] = a_2 * 1 + a_1 * 2 + a_0 * 3;
        a_2 = a_1;
        a_1 = a_0;
        */
    }

    return 0;
}
