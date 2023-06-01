/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* gemver.c: this file is part of PolyBench/C */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
#define POLYBENCH_USE_SCALAR_LB
#include <polybench.h>

#define MINI_DATASET
#define DATA_TYPE_IS_INT
/* Include benchmark-specific header. */
#include "gemver.h"

  /* Retrieve problem size. */
  int n = N;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta[1];
  DATA_TYPE betata;
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N, N, n, n);
  POLYBENCH_1D_ARRAY_DECL(u1, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(v1, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(u2, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(v2, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(w, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(x, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(y, DATA_TYPE, N, n);
  POLYBENCH_1D_ARRAY_DECL(z, DATA_TYPE, N, n);


/* Array initialization. */
static
void init_array ()
{
  int i, j;

  alpha = 1;
  beta[0] = 1;

  DATA_TYPE fn = (DATA_TYPE)n;

  for (i = 0; i < n; i++)
    {
      u1[i] = i;
      u2[i] = ((i+1)/fn)/2;
      v1[i] = ((i+1)/fn)/4;
      v2[i] = ((i+1)/fn)/6;
      y[i] = ((i+1)/fn)/8;
      z[i] = ((i+1)/fn)/9;
      x[i] = 0;
      w[i] = 0;
      for (j = 0; j < n; j++)
        A[i][j] = (DATA_TYPE) (i*j % n) / n;
    }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array()
{
  int i;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("w");
  for (i = 0; i < n; i++) {
    if (i % 20 == 0) fprintf (POLYBENCH_DUMP_TARGET, "\n");
    fprintf (POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, w[i]);
  }
  POLYBENCH_DUMP_END("w");
  POLYBENCH_DUMP_FINISH;
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel()
{
  int i, j, s;

#pragma scop
//loop_begin();
  for (i = 0; i < _PB_N; i++)
  {
    // int sum = 0;
    for (j = 0; j < _PB_N; j++)
    {
      //please_map_me();
      // s = A[i][j];
      A[i][j] += u1[i] * v1[j] + u2[i] * v2[j];
      // A[i][j] += sum; 
    }
    
  }

  for (i = 0; i < _PB_N; i++)
  {
    for (j = 0; j < _PB_N; j++)
    {
      //please_map_me();
      x[i] += betata * A[j][i] * y[j];
    }
  }
//loop_begin();
  for (i = 0; i < _PB_N; i++)
  {
    //please_map_me();
    s = x[i];
    s = s + z[i];
    x[i] = s;
  }
loop_begin();
  for (i = 0; i < _PB_N; i++)
  {
    for (j = 0; j < _PB_N; j++)
    {
      //please_map_me();
      s = w[i];
      s = s +  alpha * A[i][j] * x[j];
      w[i] = s;
    }
  }
loop_end();
#pragma endscop
}


int main(int argc, char** argv)
{

  /* Initialize array(s). */
  init_array ();

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel();

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array());

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(A);
  POLYBENCH_FREE_ARRAY(u1);
  POLYBENCH_FREE_ARRAY(v1);
  POLYBENCH_FREE_ARRAY(u2);
  POLYBENCH_FREE_ARRAY(v2);
  POLYBENCH_FREE_ARRAY(w);
  POLYBENCH_FREE_ARRAY(x);
  POLYBENCH_FREE_ARRAY(y);
  POLYBENCH_FREE_ARRAY(z);

  return 0;
}
