#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <X11/Xlib.h>
#include "X11Framebuffer.hpp"
class X11Display;

class X11Window {
friend class X11Display;

public:
    X11Window(X11Display* d, Window& w, int width, int height);
    ~X11Window();

    static X11Window CreateSimpleWindow(X11Display* d, int width, int height);

    X11Framebuffer& get_framebuffer() { return frame; }

    void resize(int width, int height);

    void update();

    GC gc;
    Window window;
    X11Display* display;
    X11Framebuffer frame;
};

#endif
