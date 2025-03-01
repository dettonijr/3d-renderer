#include "Framebuffer.hpp"
#include <stdlib.h>
#include "X11Display.hpp"
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <limits>

Framebuffer::Framebuffer(X11Display& display, int width, int height) : _dpy(display.ptr()), _image(NULL)  {
    resize(width, height);
}

Framebuffer::~Framebuffer() {
    XFree(_image);
}

void Framebuffer::resize(int width, int height) {
    if (_width == width && _height == height) {
        printf("Same size. Do nothing\n");
        return;
    }

    buf.resize(width*height*2);
    zbuf.resize(width*height);
    _width = width;
    _height = height;
    if (_image != NULL) 
        XFree(_image);
    _image = XCreateImage(_dpy, DefaultVisual(_dpy, DefaultScreen(_dpy)), DefaultDepth(_dpy, DefaultScreen(_dpy)), ZPixmap, 0, (char*) &buf[0], _width, _height, 8, 0);
    
    // Initialize z-buffer with maximum depth values
    clear_zbuffer();
}

std::vector<uint32_t>& Framebuffer::get_raw_buffer() {
    return buf;
}

std::vector<float>& Framebuffer::get_zbuffer() {
    return zbuf;
}

void Framebuffer::clear_zbuffer() {
    std::fill(zbuf.begin(), zbuf.end(), std::numeric_limits<float>::max());
}

void Framebuffer::set_pixel(int x, int y, const Color& c) {
    if (x < 0)
        return;
    else if (x > _width)
        return;
    else if (y < 0)
        return;
    else if (y > _height)
        return;

    if (y*_width+x > (_width*_height << 2)) {
        printf("FUDEU\n");
    }

    buf[y*_width+x] = c.r << 16 | c.g << 8 | c.b;
}

void Framebuffer::fill(Color& c) {
    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            buf[i*_width+j] = c.r << 16 | c.g << 8 | c.b;
        }
    }
    
    // Reset z-buffer when filling
    clear_zbuffer();
}
