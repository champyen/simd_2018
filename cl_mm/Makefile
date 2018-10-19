CC=clang
EXEC = libcltk.so cl_mm
all: $(EXEC)

CFLAGS  = -O3 -I include -I stb
CFLAGS += -Wno-deprecated-declarations -fPIC -DCL_TARGET_OPENCL_VERSION=120

LIB_LDFLAGS = -L /usr/lib/x86_64-linux-gnu/ -lOpenCL
LIB_OBJS = \
	src/cltk.o

LDFLAGS = -L. -lm -lcltk -lOpenCL

CL_MM_OBJS = \
	mm/cl_mm.o

deps =
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -MMD -MF $@.d -c $<
deps += $(GEMM_OBJS:%.o=%.o.d)
deps += $(LIB_OBJS:%.o=%.o.d)

cl_mm: $(CL_MM_OBJS) libcltk.so
	$(CC) -o $@ $^ $(LDFLAGS)

libcltk.so: $(LIB_OBJS)
	$(CC) -shared -o $@ $^ $(LIB_LDFLAGS)

clean:
	$(RM) -f $(EXEC) $(LIB_OBJS) $(GEMM_OBJS) $(deps)
