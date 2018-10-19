
__kernel void cl_mm(
   __global const float *A,
   const int stride_a,
   __global float *C,
   const int stride_c,
   const int m,
   const int n,
   const int k,
   __global const float *B
){

    int gx = get_global_id(0) << 3;
    int gy = get_global_id(1) << 3;
    int stride_b = stride_c;

    float8 a[8];
    float8 b[8];
    float8 c[8] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};

	// In OpenCL, vector data is load/store by:
	// vloadN(VECTOR_OFFSET, ADDRESS);
	// vstoreN(VECTOR, VECTOR_OFFSET, ADDRESS)

    for (int pos = 0; pos < k; pos += 8){
    }

    //store out
    for (int i = 0; i < 8; i++){
    }
}
