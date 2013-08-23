#include <nmmintrin.h>
#include <stdio.h>
#include <omp.h>

#define STEPN 4
#define  STEPM 8

void transposeA(int n,int m, int npad, int mpad, float* dst, float* src)
{
#pragma omp parallel for  
  for (int j = 0; j < n; ++j)
    {
      for (int i = 0; i < m; ++i)
	{
	  dst[j+i*npad] = src[i+j*m];
	}

      for (int i = m; i < mpad; ++i)
	{
	  dst[j+i*npad] = 0;
	}
    }

#pragma omp parallel for
  for (int i = 0; i < mpad; ++i)
    for (int j = n; j < npad; ++j)
      {
	dst[j+i*npad] = 0;
      }
      
}

void transposeB(int n, int m, int npad, int mpad, float* dst, float* src)
{
#pragma omp parallel for
  for (int i = 0; i < n; ++i)
    {
      for (int j = 0; j < m; ++j)
	{
	  dst[i+j*npad] = src[j+i*m];
	}
      
      for (int j = m; j < mpad; ++j)
	{
	  dst[i+j*npad] = 0;
	}
    }
             
#pragma omp parallel for
  for (int j = 0; j < mpad; ++j)
    for (int i = n; i < npad; ++i)
      {
	dst[i+j*npad] = 0;
      }
}

void move(int m, int mpad, float *C, float *backup)
{
#pragma omp parallel for
  for (int j = 0; j < m; ++j)
    {
      int i;

      for ( i = 0; i+4 < m; i+=4)
	{
	  _mm_storeu_ps(backup+i+j*m, _mm_loadu_ps(C+i+j*mpad));
	}
      for (; i < m; ++i)
	{
	  backup[i+j*m] = C[i+j*mpad];
	}
    }
}

void sgemm( int m_a, int n_a, float *A, float *B, float *C ) {

  int mpad = (m_a%STEPM? (m_a/STEPM+1)*STEPM:m_a);
  int npad =(n_a%STEPN ? (n_a/STEPN+1)*STEPN:n_a);
  int mbackup = m_a;
  // padding and transpose happen all together
  float* Apad=malloc(mpad*npad*sizeof(float));
  transposeA(n_a, m_a, npad, mpad, Apad, A);
  A=Apad;
  float* Bpad=malloc(npad*mpad*sizeof(float));
  transposeB(n_a, m_a, npad, mpad, Bpad, B);
  B=Bpad;            
 
  float* Cpad = malloc(mpad*mpad*sizeof(float));
  float* backup = C;
  C = Cpad;
  // we are not worried about freeing original A\B\C so we just 
  // overide them with padded ones so that the below code use A\B\C.
  m_a = mpad;
  n_a = npad;
  // calibrate the block size to match the loop strike size
  int Blocki=STEPM;
  int Blockj=STEPM;

  int IB=m_a/Blocki;
  int JB=m_a/Blockj;
#pragma omp parallel 
  {
    __m128 a1, a2, a3, a4,a5,a6,a7,a8, c0;
    __m128 c11, c12, c13, c14,c15,c16,c17,c18;
    __m128 c21, c22, c23, c24,c25,c26,c27,c28;
    __m128 c31, c32, c33, c34,c35,c36,c37,c38;
    __m128 c41, c42, c43, c44,c45,c46,c47,c48;

    __m128 b1, b2, b3, b4,b5,b6,b7,b8;
    float temp0,temp1,temp2,temp3,temp4, temp5, temp6, temp7, temp8;
   // Below is just variables adjusted by blocking in for loops
    int jj=0;
    int kk=0;

    int kkma=0;
    int jjna=0;
    int jjma=0; 
    int iina=0;

#pragma omp for 
    for( int j = 0; j < m_a; j+=8 ) {
      jj=j;
      jjma=jj*m_a;
      jjna=jj*n_a;

       int ii=0;

  for (int l=0;l<IB;l++){
      for( int i = 0; i < Blocki; i+=4 ) {
	ii=i+l*Blocki; // adjusted for blocking

	iina=ii*n_a;   // precomputed for the products in inner loop

    c31=c32=c33=c34=c35=c36=c37=c38\
     =c41=c42=c43=c44=c45=c46=c47=c48\
     =c11=c12=c13=c14=c15=c16=c17=c18\
     =c21=c22=c23=c24=c25=c26=c27=c28\
     = _mm_setzero_ps();
 
	for( int k = 0; k < n_a; k+=4 ) {
	  float* tempA=A+k+iina;
	  float* tempB=B+k+jjna;
            // use 8 B's in one iteration and this affect index j
	  b1 = _mm_loadu_ps(tempB);
	  b2 = _mm_loadu_ps(tempB+n_a);
	  b3 = _mm_loadu_ps(tempB+2*n_a);
	  b4 = _mm_loadu_ps(tempB+3*n_a);
	  b5 = _mm_loadu_ps(tempB+4*n_a);
	  b6 = _mm_loadu_ps(tempB+5*n_a);
	  b7 = _mm_loadu_ps(tempB+6*n_a);
	  b8 = _mm_loadu_ps(tempB+7*n_a);
	  
    /////////////////////////////////////////
    // use 4 a's. Affect index i
	  a1 = _mm_loadu_ps(tempA);
	  a2 = _mm_loadu_ps(tempA+n_a);
	  a3 = _mm_loadu_ps(tempA+n_a*2);
	  a4 = _mm_loadu_ps(tempA+n_a*3);
    // below are multiplications between a's and corresponding b's
	  c11=_mm_add_ps(c11,  _mm_mul_ps(a1, b1));
	  c21 = _mm_add_ps(c21, _mm_mul_ps(a2, b1));
    c31=_mm_add_ps(c31,  _mm_mul_ps(a3, b1));
	  c41 = _mm_add_ps(c41, _mm_mul_ps(a4, b1));
    
	  c12 = _mm_add_ps(c12, _mm_mul_ps(a1, b2));
	  c22 = _mm_add_ps(c22, _mm_mul_ps(a2, b2));
    c32 = _mm_add_ps(c32, _mm_mul_ps(a3, b2));
	  c42 = _mm_add_ps(c42, _mm_mul_ps(a4, b2));
   
	  c13= _mm_add_ps(c13,  _mm_mul_ps(a1, b3));
	  c23 = _mm_add_ps(c23, _mm_mul_ps(a2, b3));
    c33= _mm_add_ps(c33,  _mm_mul_ps(a3, b3));
	  c43 = _mm_add_ps(c43, _mm_mul_ps(a4, b3));

	
	  c14 = _mm_add_ps(c14, _mm_mul_ps(a1, b4));
	  c24 = _mm_add_ps(c24, _mm_mul_ps(a2, b4));
	  c34 = _mm_add_ps(c34, _mm_mul_ps(a3, b4));
	  c44 = _mm_add_ps(c44, _mm_mul_ps(a4, b4));
    /////////////////////////////////////////
    c15 = _mm_add_ps(c15, _mm_mul_ps(a1, b5));
	  c25 = _mm_add_ps(c25, _mm_mul_ps(a2, b5));
	  c35 = _mm_add_ps(c35, _mm_mul_ps(a3, b5));
	  c45 = _mm_add_ps(c45, _mm_mul_ps(a4, b5));
 
    c16 = _mm_add_ps(c16, _mm_mul_ps(a1, b6));
	  c26 = _mm_add_ps(c26, _mm_mul_ps(a2, b6));
	  c36 = _mm_add_ps(c36, _mm_mul_ps(a3, b6));
	  c46 = _mm_add_ps(c46, _mm_mul_ps(a4, b6));

 
    /////////////////////////////////////////
    c17 = _mm_add_ps(c17, _mm_mul_ps(a1, b7));
	  c27 = _mm_add_ps(c27, _mm_mul_ps(a2, b7));
	  c37 = _mm_add_ps(c37, _mm_mul_ps(a3, b7));
	  c47 = _mm_add_ps(c47, _mm_mul_ps(a4, b7));
 
    c18 = _mm_add_ps(c18, _mm_mul_ps(a1, b8));
	  c28 = _mm_add_ps(c28, _mm_mul_ps(a2, b8));
	  c38 = _mm_add_ps(c38, _mm_mul_ps(a3, b8));
	  c48 = _mm_add_ps(c48, _mm_mul_ps(a4, b8));   
     
    }
  // below sums 4 numbers in a c and extract it to save to one matrix entry
	c0=  _mm_hadd_ps(c11,c11);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c12,c12);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+m_a] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c13,c13);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+m_a*2] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c14,c14);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+m_a*3] = _mm_cvtss_f32(c0);

  c0=  _mm_hadd_ps(c15,c15);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+m_a*4] = _mm_cvtss_f32(c0);
 
 	c0=  _mm_hadd_ps(c16,c16);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+m_a*5] = _mm_cvtss_f32(c0);
 
  	c0=  _mm_hadd_ps(c17,c17);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+m_a*6] = _mm_cvtss_f32(c0);
  
   	c0=  _mm_hadd_ps(c18,c18);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+m_a*7] = _mm_cvtss_f32(c0);
  

  //
	c0=  _mm_hadd_ps(c21,c21);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+1] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c22,c22);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+1+m_a] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c23,c23);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+1+m_a*2] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c24,c24);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+1+m_a*3] = _mm_cvtss_f32(c0);
  
	c0=  _mm_hadd_ps(c25,c25);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+1+m_a*4] = _mm_cvtss_f32(c0);
 
	c0=  _mm_hadd_ps(c26,c26);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+1+m_a*5] = _mm_cvtss_f32(c0);
 
  
	c0=  _mm_hadd_ps(c27,c27);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+1+m_a*6] = _mm_cvtss_f32(c0);
                        
   
	c0=  _mm_hadd_ps(c28,c28);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+1+m_a*7] = _mm_cvtss_f32(c0);
                        
   
  //

	c0=  _mm_hadd_ps(c31,c31);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+2] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c32,c32);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+2+m_a] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c33,c33);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+2+m_a*2] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c34,c34);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+2+m_a*3] = _mm_cvtss_f32(c0);
  
   c0=  _mm_hadd_ps(c35,c35);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+2+m_a*4] = _mm_cvtss_f32(c0);
  
    c0=  _mm_hadd_ps(c36,c36);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+2+m_a*5] = _mm_cvtss_f32(c0);
  
   
   c0=  _mm_hadd_ps(c37,c37);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+2+m_a*6] = _mm_cvtss_f32(c0);
  
    c0=  _mm_hadd_ps(c38,c38);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+2+m_a*7] = _mm_cvtss_f32(c0);
                   
  
  //
	c0=  _mm_hadd_ps(c41,c41);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+3] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c42,c42);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+3+m_a] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c43,c43);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+3+m_a*2] = _mm_cvtss_f32(c0);

	c0=  _mm_hadd_ps(c44,c44);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+3+m_a*3] = _mm_cvtss_f32(c0);
  
   	c0=  _mm_hadd_ps(c45,c45);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+3+m_a*4] = _mm_cvtss_f32(c0);
	
   	c0=  _mm_hadd_ps(c46,c46);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+3+m_a*5] = _mm_cvtss_f32(c0);
	
  
   	c0=  _mm_hadd_ps(c47,c47);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+3+m_a*6] = _mm_cvtss_f32(c0);
	
   	c0=  _mm_hadd_ps(c48,c48);
	c0=  _mm_hadd_ps(c0,c0); 
	C[ii+jjma+3+m_a*7] = _mm_cvtss_f32(c0);
	                          
   
      }    }
    } 
    }
  // copy the result to the original matrix
  move(mbackup, mpad, C, backup);
  // these are padded matrix already
  free(A);
  free(B);
  free(C);
}             
