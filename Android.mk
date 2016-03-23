LOCAL_PATH := $(call my-dir)

VERSION = $(shell git describe --always --dirty=-dev 2>/dev/null || echo Unknown)
OPTEE_CLIENT_PATH ?= $(LOCAL_PATH)/../optee_client

include $(CLEAR_VARS)
LOCAL_MODULE := teec
LOCAL_SRC_FILES := $(OPTEE_CLIENT_PATH)/libs/$(TARGET_ARCH_ABI)/libteec.so
LOCAL_EXPORT_C_INCLUDES := $(OPTEE_CLIENT_PATH)/public
include $(PREBUILT_SHARED_LIBRARY)

-include $(TA_DEV_KIT_DIR)/host_include/conf.mk

include $(CLEAR_VARS)
LOCAL_MODULE := neo
LOCAL_SHARED_LIBRARIES := teec

srcs := quiz.c

LOCAL_SRC_FILES := $(patsubst %,host/%,$(srcs))

LOCAL_C_INCLUDES += host/

LOCAL_C_INCLUDES += $(TA_DEV_KIT_DIR)/host_include

LOCAL_C_INCLUDES += ta/include

ifeq ($(CFG_ENC_FS),y)
LOCAL_CFLAGS += -DCFG_ENC_FS
endif

LOCAL_CFLAGS += -DUSER_SPACE
LOCAL_CFLAGS += -DTA_DIR=\"/system/lib/optee_armtz\"
LOCAL_CFLAGS += -pthread

include $(BUILD_EXECUTABLE)
