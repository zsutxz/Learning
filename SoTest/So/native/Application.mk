APP_STL := c++_static
APP_CPPFLAGS := -frtti -std=c++11
APP_PLATFORM := android-19
APP_ALLOW_MISSING_DEPS=true
APP_CFLAGS += -Wno-error=format-security
APP_BUILD_SCRIPT := Android.mk
APP_ABI := armeabi-v7a x86
