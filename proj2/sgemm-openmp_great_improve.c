#include <nmmintrin.h>
#include <stdio.h>

 void sgemm( int m_a, int n_a, float *A, float *B, float *C ) {
  int Blockk=9;
  int Blockj=9;
  int KB=n_a/Blockk;
  int JB=m_a/Blockj;
  
#pragma omp parallel
  {          
  int ii=0;
  int jj=0;
  int kk=0;
#pragma omp for
 
  
   for( int i = 0; i < m_a; i++ ) {
 // for (int g=0;g<KB;g++){
    for (int w=0;w<JB;w++){

         for( int k = 0; k < n_a; k++ ) {
    //        kk=k+g*Blockk;
        for( int j = 0; j < Blockj; j++ ) {
         jj=j+w*Blockj;   
      //   ii=i+l*Blocki;
  //     for (int u=0;u<JB;u++){
   //       jj=j+u*Blockj;
	
 C[i+jj*m_a] += A[i+k*m_a] * B[jj+k*m_a];
      }
    }
  }          }        
  //                  }
    }//openmp
}






























/*typedef struct rect block;
struct rect{
int startR;
int startC;
int endR;
int endC;
};

void sgemm1( int m_a, int n_a, float *A, float *B, float *C, block* blockA,\
    block* blockB) {
 int startAR=blockA->startR;
 int startAC=blockA->startC;
 int AsizeR=blockA->endR-startAR;
 int AsizeC=blockA->endC-startAC;

 int startBR=blockB->startR;
 int startBC=blockB->startC;
 int BsizeR=blockB->endR-startBR;
 int BsizeC=blockB->endC-startBC;
 if (AsizeR!=BsizeC||AsizeC!=BsizeR)
   fprintf(stderr, "The DIM is not right for A\\B\n");

for( int j = 0;j  < AsizeR; j++ ) {
   int jj=j+startBC;
   for( int k = 0; k < AsizeC; k++ ) {
  int kA=k+startAC;
  int kB=k+startBR;
 
   
   for( int i = 0; i < AsizeR; i++ ) {
     int ii=i+startAR;
     fprintf(stderr,"***\n");
     fprintf(stderr,"A[ii:startAR]:%d:%d:%dx used\n",ii,kA,(int)A[ii+kA*m_a]);
 
     fprintf(stderr,"B[jj:startAR]:%d:%d:%dxis used\n",jj,kB,(int)B[jj+kB*m_a]);
	
       fprintf(stderr,"C[ii:jj]:%d:%d:%dxis used\n",ii,jj,(int)C[ii+jj*m_a]);
   
       float temp=  A[ii+kA*m_a] * B[jj+kB*n_a];
       fprintf(stderr,"temp: %d\n",(int)temp);
       C[ii+jj*m_a] +=temp;
   
      fprintf(stderr,"C[ii:jj]:%d:%d:%dis used\n",ii,jj,(int)C[ii+jj*m_a]);
      fprintf(stderr,"**\n");
   }
    }
  }
}


 
void sgemm(int m_a,int n_a,float* A, float * B, float * C){
    block blockA;
    blockA.startR=0;
    blockA.startC=0;
    blockA.endR=m_a/2;
    blockA.endC=n_a;

     block blockB;
    blockB.startR=0;
    blockB.startC=0;
    blockB.endR=n_a;
    blockB.endC=m_a/2;

    sgemm1(m_a,n_a,A,B,C,&blockA,&blockB);
   fprintf(stderr,"****** 1st finish********\n");
    //////////////////////////////////
//        block blockA;
    blockA.startR=0;
    blockA.startC=0;
    blockA.endR=m_a/2;
    blockA.endC=n_a;

//     block blockB;
    blockB.startR=0;
    blockB.startC=m_a/2;
    blockB.endR=n_a;
    blockB.endC=m_a;

    sgemm1(m_a,n_a,A,B,C,&blockA,&blockB); 
    
   fprintf(stderr,"****** 2nd finish********\n"); 
   ///////////////////////////////////
 //      block blockA;
    blockA.startR=m_a/2;
    blockA.startC=0;
    blockA.endR=m_a;
    blockA.endC=n_a;

 //    block blockB;
    blockB.startR=0;
    blockB.startC=0;
    blockB.endR=n_a;
    blockB.endC=m_a/2;

    sgemm1(m_a,n_a,A,B,C,&blockA,&blockB); 



 
   fprintf(stderr,"****** 3rd finish********\n"); 

   /////////////////////////////////

  //     block blockA;
    blockA.startR=m_a/2;
    blockA.startC=0;
    blockA.endR=m_a;
    blockA.endC=n_a;

  //   block blockB;
    blockB.startR=0;
    blockB.startC=m_a/2;
    blockB.endR=n_a;
    blockB.endC=m_a;

    sgemm1(m_a,n_a,A,B,C,&blockA,&blockB); 
   
   fprintf(stderr,"****** 4th finish********\n"); 

    


} */                                      
