#ifndef __DISPLAY_HPP__
#define __DISPLAY_HPP__

#include <stdlib.h>
#include <X11/Xlib.h>
#include <string>
#include "X11Window.hpp"

class X11Display {
public:
    X11Display(const std::string& name);
    ~X11Display();
  
    void map_window(X11Window& w);
    void flush();
   
    Display* ptr() { return this->display; }

protected:
    Display* display;
};

#endif
