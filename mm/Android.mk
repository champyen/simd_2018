LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := mm
LOCAL_SRC_FILES := mm.c


include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += -DMM_NEON
LOCAL_MODULE := mm_neon
LOCAL_SRC_FILES := mm.c
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += -DENABLE_VEC
LOCAL_MODULE := mm_vec
LOCAL_SRC_FILES := mm.c
include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)
LOCAL_MODULE := mm_omp
LOCAL_CFLAGS += -fopenmp
LOCAL_LDLIBS += -fopenmp
LOCAL_SRC_FILES := mm.c
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += -DMM_NEON
LOCAL_MODULE := mm_neon_omp
LOCAL_CFLAGS += -fopenmp
LOCAL_LDLIBS += -fopenmp
LOCAL_SRC_FILES := mm.c
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_CFLAGS += -DENABLE_VEC
LOCAL_MODULE := mm_vec_omp
LOCAL_CFLAGS += -fopenmp
LOCAL_LDLIBS += -fopenmp
LOCAL_SRC_FILES := mm.c
include $(BUILD_EXECUTABLE)
