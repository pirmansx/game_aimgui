LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := raylib
LOCAL_SRC_FILES := raylib5/$(TARGET_ARCH_ABI)/libraylib.a
include $(PREBUILT_STATIC_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    := libnative_app_glue
LOCAL_SRC_FILES := raylib5/$(TARGET_ARCH_ABI)/libnative_app_glue.a
include $(PREBUILT_STATIC_LIBRARY)

###

include $(CLEAR_VARS)
LOCAL_MODULE    := game
LOCAL_LDFLAGS   += -u ANativeActivity_onCreate
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -ldl -lOpenSLES -latomic -lm -lc
LOCAL_STATIC_LIBRARIES := raylib libnative_app_glue
LOCAL_SRC_FILES := game/game.cpp
include $(BUILD_SHARED_LIBRARY)

###

include $(CLEAR_VARS)
LOCAL_MODULE := libdobby
LOCAL_SRC_FILES := Dobby/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

###

include $(CLEAR_VARS)
LOCAL_MODULE    := imgui
LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -Werror -std=c++17
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fpermissive -Wall -fexceptions
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all,-llog
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2
LOCAL_ARM_MODE := arm
LOCAL_STATIC_LIBRARIES := libdobby
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ImGui
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ImGui/backends

LOCAL_SRC_FILES := Main.cpp \
    Substrate/hde64.c \
    Substrate/SubstrateDebug.cpp \
    Substrate/SubstrateHook.cpp \
    Substrate/SubstratePosixMemory.cpp \
    Substrate/SymbolFinder.cpp \
    KittyMemory/KittyMemory.cpp \
    KittyMemory/MemoryPatch.cpp \
    KittyMemory/MemoryBackup.cpp \
    KittyMemory/KittyUtils.cpp \
    ImGui/imgui.cpp \
    ImGui/imgui_draw.cpp \
    ImGui/imgui_demo.cpp \
    ImGui/imgui_widgets.cpp \
    ImGui/imgui_tables.cpp \
    ImGui/backends/imgui_impl_opengl3.cpp \
    ImGui/backends/imgui_impl_android.cpp \

#And64InlineHook/And64InlineHook.cpp \
#ByNameModding/BNM.cpp \

include $(BUILD_SHARED_LIBRARY)
