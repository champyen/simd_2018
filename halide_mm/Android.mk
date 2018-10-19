LOCAL_PATH := $(call my-dir)


LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := halide_mm
ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_SRC_FILES := $(LOCAL_PATH)/aot_ndk_arm64/halide_mm.a
endif
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_SRC_FILES := $(LOCAL_PATH)/aot_ndk_arm32/halide_mm.a
endif
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPPFLAGS := -I $(LOCAL_PATH)/../include

ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
LOCAL_MODULE := mm_arm64
LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/aot_ndk_arm64 
LOCAL_LDLIBS += -L$(LOCAL_PATH)/aot_ndk_arm64 -llog
endif

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_MODULE := mm_arm32
LOCAL_CPPFLAGS += -I$(LOCAL_PATH)/aot_ndk_arm32
LOCAL_LDLIBS += -L$(LOCAL_PATH)/aot_ndk_arm32 -llog
endif

LOCAL_SRC_FILES := mm.cpp
LOCAL_STATIC_LIBRARIES := halide_mm

include $(BUILD_EXECUTABLE)
