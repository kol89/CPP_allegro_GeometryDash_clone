#ifndef SPEED_PORTAL_H
#define SPEED_PORTAL_H
#include "rectangle.hpp"

class speed_portal : public rectangle {
    public:
    int spd;
    speed_portal(double _x, double _y, char _spd);
};

#endif