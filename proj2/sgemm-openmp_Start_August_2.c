#include <nmmintrin.h>
#include <stdio.h>
  void transpose(int n,int m, float* dst, float* src){
  int i,j;  //n is row and m is column of original
#pragma omp parallel for 
  for (i=0;i<n;i++){
    for (j=0;j<m;j++){
        dst[j+i*m]=src[i+j*n];
    }

  }

}
void sgemm( int m_a, int n_a, float *A, float *B, float *C ) {
  int Blockk=36;
  int Blockj=36;
  int Blocki=36;
  float* Apad=malloc(m_a*n_a*sizeof(float));
  transpose(m_a,n_a,Apad,A);
  A=Apad;
  float* Bpad=malloc(m_a*n_a*sizeof(float));
  transpose(m_a,n_a,Bpad,B);
  B=Bpad;            
  
  int KB=n_a/Blockk;
  int JB=m_a/Blockj;
  int IB=m_a/Blocki;
#pragma omp parallel
  {
   __m128 c0,a1, c1, a2, c2, a3, c3,a4 , c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16;
  __m128 b1, b2, b3, b4;
 float temp0,temp1,temp2,temp3,tempC[4];
  int ii=0;
  int jj=0;
  int kk=0;

  int kkma=0;
  int jjna=0;
  int jjma=0; 
  int iina=0;
#pragma omp for
           for(int l=0;l<JB;l++){
for( int j = 0; j </*m_a*/ Blockj; j+=4 ) {
          int jj=j+l*Blockj;
     //     jj=j;  
            jjma=jj*m_a;
             jjna=jj*n_a;
for (int w=0;w<IB;w++){

    for( int i = 0; i <Blocki/* m_a*/; i+=4 ) {
        int ii=i+w*Blocki;
//      ii=i;  
      iina=ii*n_a;

 c1=c2=c3=c4=c5=c6=c7=c8=c9=c10=c11=c12=c13=c14=c15=c16= _mm_setzero_ps();
 
   for( int k = 0; k < n_a; k+=4 ) {
      tempC[0]=0;
      tempC[1]=0;
      tempC[2]=0;
      tempC[3]=0;
      float* tempA=A+k+iina;
      float* tempB=B+k+jjna;
 
 b1 = _mm_loadu_ps(tempB);
 b2 = _mm_loadu_ps(tempB+n_a);
 b3 = _mm_loadu_ps(tempB+2*n_a);
 b4 = _mm_loadu_ps(tempB+3*n_a);
 

 /////////////////////////////////////////
 a1 = _mm_loadu_ps(tempA);
 a2 = _mm_loadu_ps(tempA+n_a);
 a3 = _mm_loadu_ps(tempA+2*n_a);
 a4 = _mm_loadu_ps(tempA+3*n_a);
  
 c1=_mm_add_ps(c1,  _mm_mul_ps(a1, b1));
 c2=_mm_add_ps(c2,  _mm_mul_ps(a2, b1));
 c3=_mm_add_ps(c3,  _mm_mul_ps(a3, b1));
 c4=_mm_add_ps(c4,  _mm_mul_ps(a4, b1));
  
 c5=_mm_add_ps(c5,  _mm_mul_ps(a1, b2));
 c6=_mm_add_ps(c6,  _mm_mul_ps(a2, b2));
 c7=_mm_add_ps(c7,  _mm_mul_ps(a3, b2));
 c8=_mm_add_ps(c8,  _mm_mul_ps(a4, b2));

 c9=_mm_add_ps(c9,  _mm_mul_ps(a1, b3));
 c10=_mm_add_ps(c10,  _mm_mul_ps(a2, b3));
 c11=_mm_add_ps(c11,  _mm_mul_ps(a3, b3));
 c12=_mm_add_ps(c12,  _mm_mul_ps(a4, b3));

 c13=_mm_add_ps(c13,  _mm_mul_ps(a1, b4));
 c14=_mm_add_ps(c14,  _mm_mul_ps(a2, b4));
 c15=_mm_add_ps(c15,  _mm_mul_ps(a3, b4));
 c16=_mm_add_ps(c16,  _mm_mul_ps(a4, b4));
 /////////////////////////////////////////
 

      }

   c0=  _mm_hadd_ps(c1,c1);
    c0=  _mm_hadd_ps(c0,c0); 
 
    tempC[0]=_mm_cvtss_f32(c0);
    c0=  _mm_hadd_ps(c2,c2);
    c0=  _mm_hadd_ps(c0,c0);
    tempC[1]= _mm_cvtss_f32(c0); 
     
    c0=  _mm_hadd_ps(c3,c3);
    c0=  _mm_hadd_ps(c0,c0);   
    
    tempC[2]=_mm_cvtss_f32(c0);

    c0=  _mm_hadd_ps(c4,c4);
    c0=  _mm_hadd_ps(c0,c0);   
    
    tempC[3]= _mm_cvtss_f32(c0);  
  C[ii+jjma]+=tempC[0];
  C[ii+1+jjma]+=tempC[1];
  C[ii+2+jjma]+=tempC[2];
  C[ii+3+jjma]+=tempC[3];
  ////////////////////////////////
   c0=  _mm_hadd_ps(c5,c5);
    c0=  _mm_hadd_ps(c0,c0); 
 
    tempC[0]=_mm_cvtss_f32(c0);
    c0=  _mm_hadd_ps(c6,c6);
    c0=  _mm_hadd_ps(c0,c0);
    tempC[1]= _mm_cvtss_f32(c0); 
     
    c0=  _mm_hadd_ps(c7,c7);
    c0=  _mm_hadd_ps(c0,c0);   
    
    tempC[2]=_mm_cvtss_f32(c0);

    c0=  _mm_hadd_ps(c8,c8);
    c0=  _mm_hadd_ps(c0,c0);   
    
    tempC[3]= _mm_cvtss_f32(c0);  
  C[ii+jjma+m_a]+=tempC[0];
  C[ii+1+jjma+m_a]+=tempC[1];
  C[ii+2+jjma+m_a]+=tempC[2];
  C[ii+3+jjma+m_a]+=tempC[3];
 
  ////////////////////////////////
   c0=  _mm_hadd_ps(c9,c9);
    c0=  _mm_hadd_ps(c0,c0); 
 
    tempC[0]=_mm_cvtss_f32(c0);
    c0=  _mm_hadd_ps(c10,c10);
    c0=  _mm_hadd_ps(c0,c0);
    tempC[1]= _mm_cvtss_f32(c0); 
     
    c0=  _mm_hadd_ps(c11,c11);
    c0=  _mm_hadd_ps(c0,c0);   
    
    tempC[2]=_mm_cvtss_f32(c0);

    c0=  _mm_hadd_ps(c12,c12);
    c0=  _mm_hadd_ps(c0,c0);   
    
    tempC[3]= _mm_cvtss_f32(c0);  
  C[ii+jjma+2*m_a]+=tempC[0];
  C[ii+1+jjma+2*m_a]+=tempC[1];
  C[ii+2+jjma+2*m_a]+=tempC[2];
  C[ii+3+jjma+2*m_a]+=tempC[3];
   ////////////////////////////////
   c0=  _mm_hadd_ps(c13,c13);
    c0=  _mm_hadd_ps(c0,c0); 
 
    tempC[0]=_mm_cvtss_f32(c0);
    c0=  _mm_hadd_ps(c14,c14);
    c0=  _mm_hadd_ps(c0,c0);
    tempC[1]= _mm_cvtss_f32(c0); 
     
    c0=  _mm_hadd_ps(c15,c15);
    c0=  _mm_hadd_ps(c0,c0);   
    
    tempC[2]=_mm_cvtss_f32(c0);

    c0=  _mm_hadd_ps(c16,c16);
    c0=  _mm_hadd_ps(c0,c0);   
    
    tempC[3]= _mm_cvtss_f32(c0);  
  C[ii+jjma+3*m_a]+=tempC[0];
  C[ii+1+jjma+3*m_a]+=tempC[1];
  C[ii+2+jjma+3*m_a]+=tempC[2];
  C[ii+3+jjma+3*m_a]+=tempC[3];
  


    }
  } 
  }                    }         }
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
