#ifndef ORB_H
#define ORB_H
#include "rectangle.hpp"

class orb : public rectangle {
    public:
    char type;
    orb(double _x, double _y, char _type);
};

#endif