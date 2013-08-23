#include <nmmintrin.h>
#include <stdio.h>

int roundcol;
int roundrow;

float *paddingA(float *X, int m_a, int n_a)
{
  int i,j;

  roundcol = m_a%12 ? (m_a/12+1)*12 : m_a;
  roundrow = n_a%4  ? (n_a/4+1)*4 : n_a;

  if (roundcol == m_a && roundrow == n_a)
    {
      return 0;
    }
  else
    {
      int rc = roundcol;
      int rr = roundrow;

      float *pad = malloc(rc * rr *sizeof(float));
      __m128 temp;

      for (j = 0; j < n_a; ++j)
{
 for (i = 0; i < m_a-4; i += 4)
   {
     temp = _mm_loadu_ps(X+i+j*m_a);
     _mm_storeu_ps(pad+i+j*rc, temp);
   }

 for (; i < m_a; ++i)
   {
     pad[i+j*rc] = X[i+j*m_a];
   }
 /*
   for (i = 0; i < m_a; ++i)
   {
   pad[i+j*rc] = X[i+j*m_a];
   }*/
 for (i = m_a; i < roundcol; ++i)
   {
     pad[i+j*rc] = 0;
   }
}

      for (j = n_a; j < rr; ++j)
{
 for (i = 0; i < rc; ++i)
   {
     pad[i+j*rc] = 0;
   }
}

      return pad;
    }
  
}

float *paddingB(float *X, int m_a, int n_a)
{
  int rc = roundrow;
  int i, j;
  float *pad = malloc(rc * m_a * sizeof(float));

  for (j = 0; j < n_a; ++j)
    {
      for (i = 0; i < m_a; ++i)
{
 pad[i+j*m_a] = X[i+j*m_a];
}
    }

  for (j = n_a; j < rc; ++j)
    {
      for (i = 0; i < m_a; ++i)
{
 pad[i+j*m_a] = 0;
}
    }

  return pad;
}

float *paddingC(int m_a)
{
  int rc = roundcol;
  int i, j;
  float *pad = calloc(rc * m_a, sizeof(float));

  return pad;
}

void move(float *src, float *dst, int m_a)
{
  int i, j;
  __m128 temp;
  int rc = roundcol;

  for (j = 0; j < m_a; ++j)
    {
      for (i = 0; i < m_a-4; i+=4)
{
 temp = _mm_loadu_ps(src+i+j*rc);
 _mm_storeu_ps(dst+i+j*m_a, temp);
}
      for (; i < m_a; ++i)
{
 dst[i+j*m_a] = src[i+j*rc];
}
    }
}

void sgemm( int m_a, int n_a, float *A, float *B, float *C ) {
  __m128 a1, c1, a2, c2, a3, c3;
  __m128 b1, b2, b3, b4;

  int i, j, k;
  int rc, krc, jrc, rr;
  float *backup = A;

  if ((A = paddingA(A, m_a, n_a)) == 0)
    {
      rc = m_a;
      rr = n_a;
      A = backup;
    }
  else
    {
      rc = roundcol;
      rr = roundrow;

      if (rr != n_a)
{
 B = paddingB(B, m_a, n_a);
}

      if (rc != m_a)
{
 backup = C;
 C = paddingC(m_a);
}
    }

  for( j = 0; j < m_a; ++j ) {
    jrc = j*rc;

    for(  k = 0; k < rr; k+=4 ) {
      b1 = _mm_load1_ps(B+j+k*m_a);
      b2 = _mm_load1_ps(B+j+(k+1)*m_a);
      b3 = _mm_load1_ps(B+j+(k+2)*m_a);
      b4 = _mm_load1_ps(B+j+(k+3)*m_a);

      for (i = 0; i < rc; i += 12)
{
 krc = k*rc;

 a1 = _mm_loadu_ps(A+i+krc);
 c1 = _mm_loadu_ps(C+i+jrc);
 c1 = _mm_add_ps(_mm_mul_ps(a1, b1), c1);

 a2 = _mm_loadu_ps(A+i+4+krc);
 c2 = _mm_loadu_ps(C+i+4+jrc);
 c2 = _mm_add_ps(_mm_mul_ps(a2, b1), c2);

 a3 = _mm_loadu_ps(A+i+8+krc);	
 c3 = _mm_loadu_ps(C+i+8+jrc);
 c3 = _mm_add_ps(_mm_mul_ps(a3, b1), c3);


 krc += rc;

 a1 = _mm_loadu_ps(A+i+krc);
 c1 = _mm_add_ps(_mm_mul_ps(a1, b2), c1);

 a2 = _mm_loadu_ps(A+i+4+krc);
 c2 = _mm_add_ps(_mm_mul_ps(a2, b2), c2);

 a3 = _mm_loadu_ps(A+i+8+krc);	
 c3 = _mm_add_ps(_mm_mul_ps(a3, b2), c3);


 krc+= rc;

 a1 = _mm_loadu_ps(A+i+krc);
 c1 = _mm_add_ps(_mm_mul_ps(a1, b3), c1);

 a2 = _mm_loadu_ps(A+i+4+krc);
 c2 = _mm_add_ps(_mm_mul_ps(a2, b3), c2);

 a3 = _mm_loadu_ps(A+i+8+krc);	
 c3 = _mm_add_ps(_mm_mul_ps(a3, b3), c3);


 krc+= rc;

 a1 = _mm_loadu_ps(A+i+krc);
 c1 = _mm_add_ps(_mm_mul_ps(a1, b4), c1);
 _mm_storeu_ps(C+i+jrc, c1);

 a2 = _mm_loadu_ps(A+i+4+krc);
 c2 = _mm_add_ps(_mm_mul_ps(a2, b4), c2);
 _mm_storeu_ps(C+i+4+jrc, c2);

 a3 = _mm_loadu_ps(A+i+8+krc);	
 c3 = _mm_add_ps(_mm_mul_ps(a3, b4), c3);
 _mm_storeu_ps(C+i+8+jrc, c3);

}


//	C[i+j*m_a] += A[i+k*m_a] * B[j+k*m_a];
    }
  }

  if (roundcol != m_a)
    {
      move(C, backup, m_a);

      free(A);
      free(C);
    }
}
