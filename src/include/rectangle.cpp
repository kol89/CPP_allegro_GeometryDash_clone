#include "include.hpp"

rectangle::rectangle(double _x, double _y, double _vx, double _vy, double _sx, double _sy, double _ax, double _ay, colour _c) {
        x = _x;
        y = _y;
        StartX = _x;
        StartY = _y;
        vx = _vx;
        vy = _vy;
        sx = _sx;
        sy = _sy;
        ax = _ax;
        ay = _ay;
        c = _c;
}

rectangle::rectangle() {
        x = 0;
        y = 0;
        StartX = 0;
        StartY = 0;
        vx = 0;
        vy = 0;
        sx = 1;
        sy = 1;
        ax = 0;
        ay = 0;
        c = colour();
}

void rectangle::move() {
        x += vx / fps;
        y += vy / fps;
}

void rectangle::accelerate() {
        vx += ax / fps;
        vy += ay / fps;
    }
void rectangle::rend() {
        al_draw_filled_rectangle(x, y, x + sx, y + sy, al_map_rgb(c.r, c.g, c.b));
    }
void rectangle::reset() {
        x = StartX;
        y = StartY;
    }