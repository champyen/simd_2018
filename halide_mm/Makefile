CXX=clang++ -O3
HALIDE_ROOT=../../
CXX_FLAGS=-fno-rtti -std=c++11 -I ../include/ -I $(HALIDE_ROOT)/tools -L ../bin/
CXX_LIBS=-L ../bin/ -lHalide -ltinfo -lpthread -ldl
CXX_MM_LIBS=-L ../bin/ -lpthread -ldl -ljpeg -lpng

mm: mm.cpp aot/halide_mm.a aot/halide_mm.h
	$(CXX) $(CXX_FLAGS) -I aot -o mm mm.cpp aot/halide_mm.a  $(CXX_MM_LIBS)

aot/halide_mm.a: mm_generator
	LD_LIBRARY_PATH=../bin/ ./mm_generator -g halide_mm -o ./aot MA.type=float32 MB.type=float32 output.type=float32 auto_schedule=true target=host	
	LD_LIBRARY_PATH=../bin/ ./mm_generator -g halide_mm -o ./aot_ndk_arm64 MA.type=float32 MB.type=float32 output.type=float32 auto_schedule=true target=arm-64-android
	LD_LIBRARY_PATH=../bin/ ./mm_generator -g halide_mm -o ./aot_ndk_arm32 MA.type=float32 MB.type=float32 output.type=float32 auto_schedule=true target=arm-32-android

mm_generator: mm_generator.cpp
	$(CXX) $(CXX_FLAGS) -o mm_generator mm_generator.cpp $(HALIDE_ROOT)/tools/GenGen.cpp $(CXX_LIBS)

clean:
	rm -f mm_generator mm aot/* aot_ndk_arm64/* aot_ndk_arm32/*
