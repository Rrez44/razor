#pragma once
#include <cstdint>

struct WindowConfig {
    int width = 100;
    int height = 100;
    const char* title = "razor";
    bool vsync = true;
    bool fullscreen = false;
    bool resizable = true;
};

enum class KeyboardKey {
    Space =32,
    A = 65, B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
    Escape = 256
};

bool platform_init();
void platform_shutdown();
bool window_create(const WindowConfig&);
void window_destroy();
bool window_should_close();
void window_request_close();
void window_swap_buffers();
void window_poll_events();
void window_size(int* width, int* height);
void window_framebuffer_size(int* width, int* height);
void input_new_frame();
bool input_is_down(KeyboardKey key);
bool input_was_pressed(KeyboardKey key);
bool input_was_released(KeyboardKey key);
void time_new_frame();
double time_delta();
double time_now();
