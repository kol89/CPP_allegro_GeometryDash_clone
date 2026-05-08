#ifndef PLAYER_H
#define PLAYER_H
#include "rectangle.hpp"
#include "rotateble_rectangle.hpp"
#include "level.hpp"

class player : public rectangle {
    public:
    double grad;
    char mode;
    bool flip_gr;
    rotateble_rectangle skin;
    rectangle collision_box;
    rectangle hit_box;
    void chsp(int s);
    player(double _x, double _y, char _mode, int _speed);
    void sync();
    void pmove();
    void paccelerate();
    bool collision(level l);
};

#endif 