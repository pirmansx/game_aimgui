APP_ABI :=  armeabi-v7a
#x86_64 x86 arm64-v8a

APP_STL          := c++_static
APP_OPTIM        := release
APP_THIN_ARCHIVE := true
APP_PIE 		 := true


APP_PLATFORM := android-18
APP_CPPFLAGS := -std=c++14 -fno-rtti -fno-exceptions -DNDEBUG -Wall -fpermissive -fpic
APP_LDFLAGS := -llog


ifneq ($(APP_OPTIM), debug)
  $(info APP_OPTIM is $(APP_OPTIM) ...)
  APP_LDFLAGS += -Wl,--strip-all
  APP_CFLAGS  += -fvisibility=hidden -fvisibility-inlines-hidden
  APP_CFLAGS  += -g0 -O3 -fomit-frame-pointer -ffunction-sections -fdata-sections
endif

