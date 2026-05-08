#ifndef ROTATABLE_RECTANGLE_H
#define ROTATABLE_RECTANGLE_H
#include "shape.hpp"

class rotateble_rectangle : public shape {
    public:
    double grad;
    double x1, y1, x2, y2, x3, y3;
    rotateble_rectangle(double _x, double _y, double _vx, double _vy, double _sx, double _sy, double _ax, double _ay, double _grad, colour _c);
    rotateble_rectangle();
    void move();
    void accelerate();
    void rend();
    void reset();
    void rotate();
};

#endif