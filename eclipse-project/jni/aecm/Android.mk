LOCAL_PATH := $(call my-dir)

# define library containing our NEON code
# ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)

include $(CLEAR_VARS)
LOCAL_MODULE		:= webrtc_aecm

#LOCAL_ARM_NEON  := true #enable __ARM_NEON__ to use <arm_neon.h> in aec_core_neon.c

LOCAL_SRC_FILES	:= \
				randomization_functions.c \
				spl_sqrt_floor.c \
				division_operations.c \
				vector_scaling_operations.c \
				downsample_fast.c \
				cross_correlation.c \
				spl_init.c \
				min_max_operations.c \
				complex_fft.c \
				complex_bit_reverse.c \
				real_fft.c \
				delay_estimator.c \
				delay_estimator_wrapper.c \
				ring_buffer.c \
				aecm_core.c \
				echo_control_mobile.c \
				aecm_wrapper.c

include $(BUILD_SHARED_LIBRARY)

#endif # TARGET_ARCH_ABI == armeabi-v7a