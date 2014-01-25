LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LOCAL_SRC_FILES := main.cpp pic.cpp

LOCAL_STATIC_LIBRARIES := SDL2_static SDL2_image_static
#LOCAL_STATIC_LIBRARIES += libstlport_static

include $(BUILD_SHARED_LIBRARY)
$(call import-module,SDL)LOCAL_PATH := $(call my-dir)
