#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "Dobby/dobby.h"
#include "Includes/Macros.h"
#include "imgui.h"
//#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"

int glHeight, glWidth;
bool setup;

#define HOOKAF(ret, func, ...) \
    ret (*old_##func)(__VA_ARGS__); \
    ret my_##func(__VA_ARGS__)

//hooking inputs/touches for so we can add input to oir imgui
HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    old_Input(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
}

//make new window
void myview(){
    ImGui::SetNextWindowPos(ImVec2(0,0), ImGuiCond_Always, ImVec2(0,0));
	ImGui::SetNextWindowSize(ImVec2(glWidth,glHeight), 0);
    ImGui::Begin("",0,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove);
    ImGui::Text(obf("Testtttt"));
    ImGui::End();
}

void SetupImgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);
    // Setup Dear ImGui style
    // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init(obf("#version 100"));
    // We load the default font with increased size to improve readability on many devices with "high" DPI.
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 22.0f;
    io.Fonts->AddFontDefault(&font_cfg);
    // Arbitrary scale-up
    ImGui::GetStyle().ScaleAllSizes(3.0f);
}


//hooking EGL for displaying our imgui
HOOKAF(EGLBoolean,eglSwapBuffers,EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    if (!setup) {
        SetupImgui();
        setup = true;
    }
    ImGuiIO &io = ImGui::GetIO();
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();
    //Render ImGui windows here.
    myview();
    ImGui::ShowDemoWindow();
    //Rendering
    ImGui::EndFrame();
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return old_eglSwapBuffers(dpy, surface);
}


void *hack_thread(void *) {
    do {
        sleep(1);
    } while (!isLibraryLoaded("libil2cpp.so"));    
    pthread_exit(nullptr);
}

void *imgui_go(void *) {
    uintptr_t addr = (uintptr_t)dlsym(RTLD_NEXT, "eglSwapBuffers");
    DobbyHook((void *)addr, (void *)my_eglSwapBuffers, (void **)&old_eglSwapBuffers);
    void *sym_input = DobbySymbolResolver(("/system/lib/libinput.so"), ("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));
    DobbyHook((void *)sym_input, (void *) my_Input, (void **)&old_Input);
    pthread_exit(nullptr);
}

__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, imgui_go, NULL);
    pthread_t hacks;
    pthread_create(&hacks, NULL, hack_thread, NULL);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * reserved)
{
    JNIEnv *env;
    vm->GetEnv((void **) &env, JNI_VERSION_1_6);
    return JNI_VERSION_1_6;
}
