#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define TEST_M 512
#define TEST_K 512
#define TEST_N 512


#ifdef MM_NEON

#include "arm_neon.h"
void gemm_vec(float *a, int sa, float *b, int sb, float *c, int sc)
{
}

#else

typedef float float4 __attribute__((ext_vector_type(4)));
void gemm_vec(float *a, int sa, float *b, int sb, float *c, int sc)
{
#ifdef ENABLE_VEC
#else
	for(int y = 0; y < 4; y++){
		for(int x = 0; x < 4; x++){
			float fc = *(c + sc*y + x);
			for(int k = 0; k < 4; k++){
				fc += a[sa*y + k] * b[sb*k + x];
			}
			*(c + sc*y + x) = fc;
		}
	}
#endif
}
#endif

int main(void)
{
	float* ma = (float*)malloc(sizeof(float)*TEST_K*TEST_M);
	float* mb = (float*)malloc(sizeof(float)*TEST_N*TEST_K);
	float* mc = (float*)malloc(sizeof(float)*TEST_N*TEST_M);
	float* chk = (float*)malloc(sizeof(float)*TEST_N*TEST_M);

	for(int y = 0; y < TEST_M; y++){
		for(int x = 0; x < TEST_K; x++){
			ma[y*TEST_K + x] = (float)(rand()%256/256.0);
		}
	}
	for(int y = 0; y < TEST_K; y++){
		for(int x = 0; x < TEST_N; x++){
			mb[y*TEST_N + x] = (float)(rand()%256/256.0);
		}
	}
	for(int y = 0; y < TEST_M; y++){
		for(int x = 0; x < TEST_N; x++){
			mc[y*TEST_N + x] = (float)0.0;
			chk[y*TEST_N + x] = (float)0.0;
		}
	}

	struct timeval stime, etime;
	gettimeofday(&stime, NULL);

    //parallel here
	for(int m = 0; m < TEST_M; m+=4){
		for(int n = 0; n < TEST_N; n+=4){
			for(int k = 0; k < TEST_K; k+=4){
				gemm_vec(
							ma + m*TEST_K + k, TEST_K,
							mb + k*TEST_N + n, TEST_N,
							mc + m*TEST_N + n, TEST_N
						);
			}
		}
	}
	gettimeofday(&etime, NULL);
	printf("FP32 SIMD: %ld us\n", (etime.tv_sec - stime.tv_sec)*1000000 + (etime.tv_usec - stime.tv_usec));

	gettimeofday(&stime, NULL);
	for(int m = 0; m < TEST_M; m++){
		for(int n = 0; n < TEST_N; n++){
			float val = 0.0;
			for(int k = 0; k < TEST_K; k++){
				val += ma[m*TEST_K + k]*mb[k*TEST_N+n];
			}
			chk[m*TEST_N + n] = val;
		}
	}
	gettimeofday(&etime, NULL);
	printf("NAIVE: %ld us\n", (etime.tv_sec - stime.tv_sec)*1000000 + (etime.tv_usec - stime.tv_usec));

	for(int m = 0; m < TEST_M; m++){
		for(int n = 0; n < TEST_N; n++){
			float val = chk[m*TEST_N + n] - mc[m*TEST_N + n];
			if( fabs(val) > 0.1){
				printf("(%d,%d), %f %f\n", m, n, chk[m*TEST_N + n], mc[m*TEST_N + n]);
			}
		}
	}

	printf("DONE!\n");
	return 0;
}
