#include "gemm.h"

//Define compute data type
#define TYPE int

//Specify row/column sizes
#define row_size 32
#define col_size 32
#define N row_size*col_size

TYPE m1[N]; TYPE m2[N]; TYPE prod[N];
void kernel(){
    int i, j, k;
    int k_col, i_col;
    TYPE mult;

    outer:for(i=0;i<row_size;i++) {
        middle:for(j=0;j<col_size;j++) {
            i_col = i * col_size;
            TYPE sum = 0;
            #pragma unroll 4
            for(k=0;k<row_size;k++) {
            please_map_me();
                k_col = k * col_size;
                mult = m1[i_col + k] * m2[k_col + j];
                sum += mult;
            }
            prod[i_col + j]  = sum;
        }
    }
}