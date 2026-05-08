#ifndef SHAPE_H
#define SHAPE_H
#include "colour.hpp"

class shape/*dot*/ {
    public:
    double x; // position x
    double y; // position y
    double StartX; // initial position x
    double StartY; // initial position y
    double vx; // velocity x
    double vy; // velocity y
    double ax; // acceleration x
    double ay; // acceleration y
    colour c; // color
};

#endif