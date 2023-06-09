#include <string.h>
#include <stdio.h>


#define SIZE  1024
int A[SIZE], B[SIZE], C[SIZE];

__attribute__((noinline))
void kernel(){
    #ifdef CGRA_COMPILER
    loop_begin();
    #endif

    for (int i=0;i<SIZE; i++){
      C[i] = A[i]+B[i];
    }

    #ifdef CGRA_COMPILER
    loop_end();
    #endif
}

int main(){

int i,j;

for (i=0;i<SIZE; i++){
      A[i] = i * 2 + 5;
      B[i] = i * 3;
      C[i] = 0;
    }
    
kernel();

for (i=0;i<SIZE; i++) printf("%d\n", C[i]);
    

return 0;

}
