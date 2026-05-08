#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "shape.hpp"


class rectangle : public shape {
    public:
    double sx; // size x
    double sy; // size y
    rectangle(double _x, double _y, double _vx, double _vy, double _sx, double _sy, double _ax, double _ay, colour _c);
    rectangle();
    void move();
    void accelerate();
    void rend();
    void reset();
    bool chkcol(rectangle a) {return a.x < x + sx && a.x + a.sx > x && a.y < y + sy && a.y + a.sy > y;}
};

#endif