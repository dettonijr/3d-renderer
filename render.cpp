#include <stdlib.h>
#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdio.h>
#include "Obj.h"
#include "Renderer.h"

const float  PI_F=3.14159265358979f;

int main() {
    X11Display d("");
    X11Window w = X11Window::CreateSimpleWindow(&d, 640, 640);
    d.map_window(w);      
    d.flush();;      

    printf("Depth %d\n", DefaultDepth(d.ptr(), DefaultScreen(d.ptr())));

    TGAFile tga("./body_diffuse.tga");
    Framebuffer& frame = w.get_framebuffer();
    Renderer r(frame);
    Obj o("./body.obj", tga);
    o.transform(Transform::rotX(PI_F));
    r.add_obj(o);  
    r.set_light(Point<float>(0,0,1));

    clock_t start = clock();
    for(int i = 0; ; i++) {
        frame.fill(Color::Black);

        Transform transform = Transform::scale(300,300,300) * Transform::translate(1.,1.,0) * Transform::rotY(i*0.05);
        r.render_all_objs(transform);
        w.update();

        int framesSample = 50;
        if (i % framesSample == 0) {
            clock_t end = clock();
            unsigned long micros = end - start;
            start = clock();
            int fps = framesSample * CLOCKS_PER_SEC / micros;
            printf("FPS: %d\n", fps);
        }
    } 

    return(0);
}
