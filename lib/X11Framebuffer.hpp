#ifndef __X11FRAMEBUFFER_HPP__
#define __X11FRAMEBUFFER_HPP__

#include <X11/Xlib.h>
#include <stdint.h>
#include "Color.h"
#include "Point.h"
#include <vector>
class X11Display;

class X11Framebuffer {
public:
    X11Framebuffer(X11Display& display, int width, int height);
    ~X11Framebuffer();

    void resize(int width, int height);
    std::vector<uint32_t>& get_raw_buffer();
    std::vector<float>& get_zbuffer();
    void clear_zbuffer();

    void set_pixel(int x, int y, const Color& c);
    void fill(Color& c);

    int width() { return _width; }
    int height() { return _height; }
    XImage* image() { return _image; }

private:
    int _width;
    int _height;
    std::vector<uint32_t> buf;
    std::vector<float> zbuf;
    XImage* _image;
    Display* _dpy;
};

#endif
