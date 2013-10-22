LOCAL_PATH := $(call my-dir)

include $(call all-subdir-makefiles)

include $(CLEAR_VARS)

LOCAL_MODULE		:= webrtc-for-android
LOCAL_SRC_FILES	:= \

include $(BUILD_SHARED_LIBRARY)