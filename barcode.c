#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 400
#define HEIGHT 200

int main(int argc, char *argv[]) {
  // X11 setup
  Display *display = XOpenDisplay(NULL);
  if (display == NULL) {
    fprintf(stderr, "Cannot open X11 display\n");
    return EXIT_FAILURE;
  }

  int screen = DefaultScreen(display);
  Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, WIDTH, HEIGHT, 1,
                                      BlackPixel(display, screen), WhitePixel(display, screen));
  XSelectInput(display, window, ExposureMask | KeyPressMask);
  XMapWindow(display, window);

  GC gc = XCreateGC(display, window, 0, NULL);
  XSetForeground(display, gc, BlackPixel(display, screen));

  // Event loop
  XEvent event;
  while (1) {
    XNextEvent(display, &event);
    if (event.type == KeyPress) {
      break;
    }
  }

  // Clean up
  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);

  return EXIT_SUCCESS;
}

