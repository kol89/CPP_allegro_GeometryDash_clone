#ifndef PORTAL_H
#define PORTAL_H
#include "rectangle.hpp"

class portal : public rectangle {
    public:
    char mode;
    portal(double _x, double _y, char _mode);
};

#endif