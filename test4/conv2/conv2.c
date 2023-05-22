#include <stdio.h>

// volatile int * N;
#define N 1024
//int *a;
int a[N];
int b[N];// = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
//int *b;
// int *m;

int kernel() {
    int i;
    for (i = 0; i < N; i++) {
        a[i] = i;
    }

    for (i = 0; i < N-1; i++) {
        //DFGLoop: loop
#ifdef CGRA_COMPILER
please_map_me();
#endif 
        b[i] = a[i] * 10 + a[i + 1] * 20; //+ a[i+ 2] * 3;
    }

    return 0;
}
