#ifndef PAD_H
#define PAD_H
#include "rectangle.hpp"

class pad : public rectangle {
    public:
    char type;
    bool activated = false;
    pad(double _x, double _y, char _type);
};

#endif