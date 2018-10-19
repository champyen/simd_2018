CC=clang -O3

all: mm mm_vec mm_omp mm_vec_omp

mm: mm.c
	$(CC) -o mm mm.c

mm_vec: mm.c
	$(CC) -o mm_vec mm.c -DENABLE_VEC

mm_omp: mm.c
	$(CC) -o mm_omp mm.c -fopenmp

mm_vec_omp: mm.c
	$(CC) -o mm_vec_omp mm.c -DENABLE_VEC -fopenmp

clean:
	rm -f mm mm_vec mm_omp mm_vec_omp