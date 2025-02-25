#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdio.h>
#include "TGAFile.hpp"
    
int main() {
    X11Display d("");
    X11Window w = X11Window::CreateSimpleWindow(&d, 1024, 1024);
    d.map_window(w);      
    d.flush();;      

    Framebuffer& frame = w.get_framebuffer();
    TGAFile f("./african_head_diffuse.tga");

    XEvent ev;
    unsigned char off = 0;
    clock_t start = clock();
    for(int i = 0; ; i++) {
        std::copy(f.data.begin(), f.data.end(), frame.get_raw_buffer().begin());
        w.update();
        if (i % 10 == 0) {
            clock_t end = clock();
            unsigned long micros = end - start;
            start = clock();
            printf("FPS: %lu\n", 100000000/micros);
        }
        usleep(3000);
    } 

    return(0);
}
