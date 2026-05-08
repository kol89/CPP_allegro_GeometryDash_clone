#ifndef BUTTON_H
#define BUTTON_H
#include "rectangle.hpp"

class button : public rectangle {
 public:
    char const* text;
    bool pressed;
    bool is_pressed();
    button(double _x, double _y, double _sx, double _sy, char const* _text, colour _c);
    void brend();
};

#endif