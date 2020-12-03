LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := Exboard
LOCAL_SRC_FILES := io/Exboard.c \
				io/key.c \
				io/exboard_mgr.c \
                sram/hw_sram.c \
				sram/hw_sl05.c \
				sram/sram.c \
				sram/sl05.c
LOCAL_LDLIBS += -llog 
LOCAL_LDLIBS +=-lm
include $(BUILD_SHARED_LIBRARY)