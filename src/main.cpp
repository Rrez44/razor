#include "platform.h"
#include <cstdio>

const double dt = 1.0 / 60.0;
double acc =0.0;

double y = 0.0;
double x = 0.0;

int main()
{

    if(!platform_init()){
        return -1;
    }

    WindowConfig wc;
    if(!window_create(wc)){
        return -1;
    }

    while(!window_should_close()){
        time_new_frame();
        double delta = time_delta();

        if(delta > 0.25){
            delta = 0.25;
        }
        acc += delta;
        input_new_frame();
        window_poll_events();

        while(acc >= dt){
            
            if(input_was_pressed(KeyboardKey::Space)){
                y += 1;
            }

            if(input_was_pressed(KeyboardKey::D)){
                x += 1;
            }
    
            acc -= dt;
        }
        const double alpha = acc / dt;

        window_swap_buffers();
    }
    window_destroy();
    platform_shutdown();

}