/*
    Copyright 2024 Clint Moyer

    This program is free software: you can redistribute it and/or modify it under
    the terms of the GNU General Public License as published by the Free Software
    Foundation, either version 3 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with
    this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 400
#define HEIGHT 200

// UPC-A encoding tables
const char *left_encoding[10] = {"0001101", "0011001", "0010011", "0111101",
                                 "0100011", "0110001", "0101111", "0111011",
                                 "0110111", "0001011"};

const char *right_encoding[10] = {"1110010", "1100110", "1101100", "1000010",
                                  "1011100", "1001110", "1010000", "1000100",
                                  "1001000", "1110100"};

// Guard patterns
const char *left_guard = "101";
const char *center_guard = "01010";
const char *right_guard = "101";

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

