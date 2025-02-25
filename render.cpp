#include <stdlib.h>
#include "X11Display.hpp"
#include "Framebuffer.hpp"
#include <unistd.h>
#include <stdio.h>
#include "Obj.h"
#include <cmath>
#include "Renderer.h"

const float  PI_F=3.14159265358979f;

int main() {
    X11Display d("");
    X11Window w = X11Window::CreateSimpleWindow(&d, 640, 640);
    d.map_window(w);      
    d.flush();;      

    printf("Depth %d\n", DefaultDepth(d.ptr(), DefaultScreen(d.ptr())));

    XSelectInput (d.ptr(), w.window,
                  ExposureMask |
                  StructureNotifyMask);

    TGAFile tga("./body_diffuse.tga");
    Renderer r;
    Obj o("./body.obj", tga);
    o.transform(Transform::rotX(PI_F));
    r.add_obj(o);  
    // for (float f = -1.; f < 2; f += 0.5) {
    //     for (float g = -1.; g < 2; g += 0.5) {
    //         Obj o("./african_head.obj", tga);
    //         o.transform(Transform::translate(f, g, 0.0) * Transform::scale(0.3, 0.3, 0.3) * Transform::rotZ(PI_F)); 
    //         r.add_obj(o);
    //     }
    // }
    r.set_light(Point<float>(0,0,1));
    Framebuffer& frame = w.get_framebuffer();

    XEvent ev;
    Point<float> t0[3] = {Point<float> (10, 70, 10), Point<float> (50, 160, 10), Point<float> (70,80,10)};
    Point<float> t1[3] = {Point<float> (180, 50, 10), Point<float> (150, 1, 10), Point<float> (70,180,10)};
    Point<float> t2[3] = {Point<float> (180, 150, 10), Point<float> (120, 160, 10), Point<float> (130,180,10)};
    unsigned char off = 0;
    clock_t start = clock();
    for(int i = 0; ; i++) {
        while(XPending(d.ptr())) {
            XNextEvent(d.ptr(), &ev);
            if (ev.type == ConfigureNotify) {
                XConfigureEvent xce = ev.xconfigure;
                printf("Resizing %d %d\n", xce.width, xce.height);
                w.resize(xce.width, xce.height);
            }   
        }
        frame.fill(Color::Black);

        Transform transform = Transform::scale(300,300,300) * Transform::translate(1.,1.,0) * Transform::rotY(i*0.05);
        r.render(frame, transform);
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
