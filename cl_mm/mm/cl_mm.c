#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

#include <CL/cl.h>
#include "cltk.h"


#define FLT_MAX     __FLT_MAX__


void naive_gemm(float *a, float*b, float *c, int m, int n, int k)
{
    for(int y = 0; y < m; y++){
        for(int x = 0; x < n; x++){
            float sum = 0.f;
            for(int z = 0; z < k; z++){
                sum += (*(a + y*k + z)) * (*(b + z*n + x));
            }
            *(c + y*n + x) = sum;
        }
    }
}

#define MAT_M   512
#define MAT_N   512
#define MAT_K   512

void result_cmp(float *cpu, float *gpu, int m, int n)
{
    for(int y = 0; y < m; y++){
        for(int x = 0; x < n; x++){
            float val = fabsf(gpu[y*n + x] - cpu[y*n + x]);
            if(val > 1.f){
                printf("%d %d min: %.2f %.2f\n", x, y, gpu[y*MAT_N + x], cpu[y*MAT_N + x]);
            }
        }
    }

}

int main(void)
{
    struct timeval s_time, e_time;
    cltk_context ctx = cltk_context_create();
    #ifdef __ANDROID__
    cltk_lib lib = cltk_lib_load(ctx, "/data/local/tmp/mm.cl", NULL, "");
    #else
    cltk_lib lib = cltk_lib_load(ctx, "mm/mm.cl", NULL, "");
    #endif
    cltk_buffer A_buf = cltk_buffer_alloc(ctx, MAT_M*MAT_K*sizeof(float));
    cltk_buffer B_buf = cltk_buffer_alloc(ctx, MAT_N*MAT_K*sizeof(float));
    cltk_buffer C_buf = cltk_buffer_alloc(ctx, MAT_M*MAT_N*sizeof(float));

    srand(time(NULL));

    float *a_buf = (float*)calloc(sizeof(float), MAT_M*MAT_K);
    float *b_buf = (float*)calloc(sizeof(float), MAT_N*MAT_K);
    float *c_buf = (float*)calloc(sizeof(float), MAT_M*MAT_N);
    for(int idx = 0; idx < MAT_M*MAT_K; idx++){
        a_buf[idx] = (float)((rand()%65536)*(rand()%16))/65535.f;
    }
    for(int idx = 0; idx < MAT_K*MAT_N; idx++){
        b_buf[idx] = (float)((rand()%65536)*(rand()%16))/65535.f;
    }
    naive_gemm(a_buf, b_buf, c_buf, MAT_M, MAT_N, MAT_K);

    float *Aptr = (float*)cltk_buffer_map(A_buf);
    float *Bptr = (float*)cltk_buffer_map(B_buf);

    //gettimeofday(&s_time, NULL);
    memcpy(Aptr, a_buf, MAT_M*MAT_K*sizeof(float));
    for(int idx = 0; idx < MAT_K; idx++){
        memcpy(Bptr, b_buf + idx*MAT_N, MAT_N*sizeof(float));
        Bptr += MAT_N;
    }

    //gettimeofday(&e_time, NULL);
    //printf("%s data copy takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);
    cltk_buffer_unmap(A_buf);
    cltk_buffer_unmap(B_buf);

    int lda = MAT_K;
    int ldc = MAT_N;
    int m = MAT_M;
    int n = MAT_N;
    int k = MAT_K;
    size_t gsize[3] = {MAT_N/8, MAT_M/8, 1 };
    cltk_func func = cltk_func_get(lib, "cl_mm");
    gettimeofday(&s_time, NULL);

    CLTK_FUNC_CALL(func, 2, gsize, NULL, A_buf, lda, C_buf, ldc, m, n, k, B_buf);

    gettimeofday(&e_time, NULL);
    printf("%s test overhead mm takes %ld us\n", __func__, (e_time.tv_sec - s_time.tv_sec)*1000000 + e_time.tv_usec - s_time.tv_usec);
    cltk_func_release(func);

    printf("check result ...\n");
    float *Cptr = (float*)cltk_buffer_map(C_buf);
    result_cmp(c_buf, Cptr, MAT_M, MAT_N);
    cltk_buffer_unmap(C_buf);

    free(a_buf);
    free(b_buf);
    free(c_buf);

    cltk_buffer_free(A_buf);
    cltk_buffer_free(B_buf);
    cltk_buffer_free(C_buf);
    cltk_lib_unload(lib);
    cltk_context_destroy(ctx);
}
