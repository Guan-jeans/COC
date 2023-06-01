#include <string.h>
#include <stdio.h>

#define E 96
#define P 14

int fin[E];
int fint[E];

void kernel(){
    int RNULL = 4; // number of NULL rows
    int CNULL = 1; // column index of the first NULL
    // P(P+1)/2 >= E > P(P-1)/2
    // RNULL(RNULL+1)/2 >= P(P+1)/2-E > RNULL(RNULL-1)/2
    // CNULL = RNULL(RNULL+1)/2 + E - P(P+1)/2
    int RN = P - RNULL + (CNULL>0); // valid row number
    #ifdef CGRA_COMPILER
    loop_begin();
    #endif
    int idx = 0; // input index
    for(int j = 0; j < P; j++){
    	for(int i = 0; i < RN; i++){
    		if(j + i < P && (i < RN-1 || j < CNULL)){
    			fint[i*P - i*(i-1)/2 + j] = fin[idx];
    			idx++;
    		}
    	}
    }
    #ifdef CGRA_COMPILER
    loop_end();
    #endif
}