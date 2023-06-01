#include <string.h>
#include <stdio.h>

#define E 96
#define P 14

int fin[E];
int fint[E];

#define N 64

int W[N];

void kernel(){
    int maxCnt = (E+N-1)/N;
    int step = 2;

    #ifdef CGRA_COMPILER
    loop_begin();
    #endif

    for(int i = 0; i < N; i++){
    	int sum = 0;
    	for(int j = 0; j < maxCnt; j++){
    		int idx = i+j*N;
    		if(idx < E){
    			sum += fint[idx];
    		}
    	}
    	W[i] = sum >> step; // round(sum >> step);
    }

    #ifdef CGRA_COMPILER
    loop_end();
    #endif
}
