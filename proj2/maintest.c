 #include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <cblas.h>
     typedef struct rect block;
     struct rect{
     int startR;
       int startC;
       int endR;
       int endC;
     };
/* Your function must have the following signature: */
void transpose(int n,int m, float* dst, float* src){
  int i,j;
  for (i=0;i<n;i++){
    for (j=0;j<m;j++){
        dst[j+i*m]=src[i+j*n];
    }

  }

}
void sgemm1( int m, int n, float *A, float *B, float *C,block* blockA,\
    block* blockB );

/* The benchmarking program */

int main( int argc, char **argv )
{
 
block blockA;
blockA.startR=1;
blockA.startC=0;
blockA.endR=3;
blockA.endC=2;
block blockB;  
blockB.startR=0;
blockB.startC=1;
blockB.endR=2;
blockB.endC=3; 
  float* A=malloc(4*2*sizeof(float));
  float* B=malloc(4*2*sizeof(float));
  float* C=malloc(4*4*sizeof(float));
  for (int i=0;i<16;i++)
    C[i]=0.0001;
  for (int i=0;i<8;i++){
    A[i]=1.1;
   // B[i]=1;
  }
  A[0]=0.0;
  A[3]=0.0;
  A[4]=0.0;
  A[7]=0.0;
  // transpose(8,2,B,A);
 // sgemm();
 for (int i=0;i<2;i++){
   for (int j=0;j<4;j++)
    printf("%f ",A[i*4+j]);
   printf("\n");
 } printf("\n");
 /*for (int i=0;i<16;i++){
  // for (int j=0;j<8;j++)
    printf("%f ",B[i]);
 } printf("\n");*/
 sgemm1(4,2,A ,A,C,&blockA,&blockB);
 
 for (int i=0;i<4;i++){
   for (int j=0;j<4;j++)
    printf("%f ",C[4*i+j]);
   printf("\n\n");
 } printf("\n"); 
    free(A);
    free(B);
    free(C);
}
