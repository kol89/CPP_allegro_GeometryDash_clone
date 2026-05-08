#include "gvar.hpp"
#include "include.hpp"

speed_portal::speed_portal(double _x, double _y, char _spd) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = grid_size;
        sy = grid_size*3;
        spd = _spd;
        x = _x * grid_size;
        y = ground.y - _y * grid_size;
        StartX = x;
        StartY = y;
        c = colour(0, 0, 255, 0);
    }