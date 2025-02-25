#include "X11Window.hpp"
#include "X11Display.hpp"
#include <cstdlib>

X11Window::X11Window(X11Display *d, Window &w, int width, int height)
    : display(d), window(w), frame(*d, width, height) {
  this->gc = XCreateGC(display->ptr(), window, 0, NULL);

  XSelectInput(display->ptr(), w, ExposureMask | StructureNotifyMask);
}

X11Window::~X11Window() { XDestroyWindow(display->ptr(), window); }

X11Window X11Window::CreateSimpleWindow(X11Display *d, int width, int height) {
  int screenId = DefaultScreen(d->ptr());
  Window w = XCreateSimpleWindow(
    d->ptr(),
    RootWindow(d->ptr(),screenId),
    0,
    0,
    width,
    height,
    1,
    BlackPixel(d->ptr(),screenId),
    WhitePixel(d->ptr(),screenId)
  );
  return X11Window(d, w, width, height);
}

void X11Window::resize(int width, int height) { frame.resize(width, height); }

void X11Window::update() {
  Display *dpy = display->ptr();
  XEvent ev;

  while (XPending(dpy)) {
    XNextEvent(dpy, &ev);
    if (ev.type == ConfigureNotify) {
      XConfigureEvent xce = ev.xconfigure;
      printf("Resizing %d %d\n", xce.width, xce.height);
      this->resize(xce.width, xce.height);
    }
  }

  XPutImage(dpy, window, gc, frame.image(), 0, 0, 0, 0, frame.width(),
            frame.height());
  XSync(dpy, False);
}
