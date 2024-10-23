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
#include <string.h>

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

// Function to draw a line in the X11 window
void draw_line(Display *display, Window window, GC gc, int x1, int y1, int x2, int y2) {
    XDrawLine(display, window, gc, x1, y1, x2, y2);
}

// Function to draw the UPC-A barcode
void draw_upc(Display *display, Window window, GC gc, const char *upc_code) {
    int x = 50;        // Start X position for barcode drawing
    int y = 50;        // Top Y position
    int bar_width = 2; // Width of each bar
    int height = 100;  // Height of the barcode

    // Draw left guard
    for (int i = 0; i < 3; i++) {
        if (left_guard[i] == '1') {
            draw_line(display, window, gc, x, y, x, y + height);
        }
        x += bar_width;
    }

    // Draw left-side digits
    for (int i = 0; i < 6; i++) {
        int digit = upc_code[i] - '0';
        const char *encoding = left_encoding[digit];
        for (int j = 0; j < 7; j++) {
            if (encoding[j] == '1') {
                draw_line(display, window, gc, x, y, x, y + height);
            }
            x += bar_width;
        }
    }

    // Draw center guard
    for (int i = 0; i < 5; i++) {
        if (center_guard[i] == '1') {
            draw_line(display, window, gc, x, y, x, y + height);
        }
        x += bar_width;
    }

    // Draw right-side digits
    for (int i = 6; i < 12; i++) {
        int digit = upc_code[i] - '0';
        const char *encoding = right_encoding[digit];
        for (int j = 0; j < 7; j++) {
            if (encoding[j] == '1') {
                draw_line(display, window, gc, x, y, x, y + height);
            }
            x += bar_width;
        }
    }

    // Draw right guard
    for (int i = 0; i < 3; i++) {
        if (right_guard[i] == '1') {
            draw_line(display, window, gc, x, y, x, y + height);
        }
        x += bar_width;
    }
}

int main(int argc, char *argv[]) {
    // Validate input
    if (argc != 2 || strlen(argv[1]) != 12) {
        fprintf(stderr, "Usage: %s <12-digit UPC>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *upc_code = argv[1];

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
        if (event.type == Expose) {
            // Draw the UPC-A barcode on the window
            draw_upc(display, window, gc, upc_code);
        }
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

