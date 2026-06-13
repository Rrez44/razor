#include "platform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstring>
#include <chrono>
static struct{
    GLFWwindow* window;
    int width, height;
    int fb_width, fb_height;
    bool platform_initialized;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point last_frame;
    double delta;
} State;

static uint8_t key_current[GLFW_KEY_LAST + 1];
static uint8_t key_previous[GLFW_KEY_LAST + 1];

static void on_framebuffer_resize(GLFWwindow* window, int width, int height){

    // warning for unused parameter
    (void)window; 

    State.fb_width = width;
    State.fb_height = height;
}

static void error_callback(int error, const char* description)
{
    // warning for unused parameter
    (void)error;

    fprintf(stderr, "Error: %s\n", description);
}

static void on_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
    (void)window;
    (void)scancode;
    (void)mods;

    if(key < 0) return;

    key_current[key] = (action != GLFW_RELEASE);
}

bool platform_init(){
    glfwSetErrorCallback(error_callback);
    if(!glfwInit()){
        return false;
    }
    State.start = std::chrono::steady_clock::now();
    State.last_frame = std::chrono::steady_clock::now();
    State.platform_initialized = true;
    return true;
}

void platform_shutdown(){
    glfwTerminate();
    State.platform_initialized = false;
}

bool window_create(const WindowConfig& config){
    if(!State.platform_initialized) return false;

    // TODO: remove magic numbers
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);

    // TODO: full screen stuff

    State.window = glfwCreateWindow(config.width, config.height, config.title, NULL, NULL);
    if(!State.window){
        return false;
    }

    glfwMakeContextCurrent(State.window);
    glfwSwapInterval(config.vsync ? 1 : 0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        window_destroy();
        return false;
    }

    glfwSetFramebufferSizeCallback(State.window, on_framebuffer_resize);
    glfwSetKeyCallback(State.window, on_key_callback);
    glfwGetFramebufferSize(State.window, &State.fb_width, &State.fb_height);
    State.width = config.width;
    State.height = config.height;
    return true;
}

void window_destroy(){
    if(!State.window) return;
    glfwDestroyWindow(State.window);
    State.window = nullptr;
}

bool window_should_close(){
    if(!State.window) return true;
    return glfwWindowShouldClose(State.window);
}

void window_request_close(){
    if(!State.window) return;
    glfwSetWindowShouldClose(State.window, true);
}

void window_swap_buffers(){
    if(!State.window) return;
    glfwSwapBuffers(State.window);
}

void window_poll_events(){
    glfwPollEvents();
}

void window_size(int* width, int* height){
    *width = State.width;
    *height = State.height;
}

void window_framebuffer_size(int* width, int* height){
    *width = State.fb_width;
    *height = State.fb_height;
}

void input_new_frame(){
    if(!State.platform_initialized || !State.window){
        return;
    }

    std::memcpy(key_previous, key_current, sizeof(key_current));
}

bool input_is_down(KeyboardKey key){
    return key_current[(int)key];
}
bool input_was_pressed(KeyboardKey key){
    return !key_previous[(int)key] && key_current[(int)key];
}
bool input_was_released(KeyboardKey key){
    return key_previous[(int)key] && !key_current[(int)key];
}


// clock

void time_new_frame(){
    auto now = std::chrono::steady_clock::now();
    State.delta = std::chrono::duration<double>(now - State.last_frame).count();
    State.last_frame = now;
}

double time_delta(){
    return State.delta;
}

double time_now(){
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(now - State.start).count();
}
