#include "X11Display.hpp"
#include <exception>
#include <iostream>
#include <stdexcept>

X11Display::X11Display(const std::string &name) {
  display = XOpenDisplay(name.c_str());
  if (display == NULL) {
    throw std::runtime_error("Display not found");
  }

  printf("Display depth %d\n", DefaultDepth(display, DefaultScreen(display)));
}

X11Display::~X11Display() {
  if (display != NULL) {
    XCloseDisplay(display);
  }
}

void X11Display::map_window(X11Window &w) { XMapWindow(display, w.window); }

void X11Display::flush() { XFlush(display); }
