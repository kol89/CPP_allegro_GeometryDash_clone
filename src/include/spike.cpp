#include "include.hpp"

spike::spike(double _x, double _y) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = 7.14;
        sy = 20.71;
        x = _x * grid_size + 20;
        y = ground.y - _y * grid_size + 14.29;
        StartX = x;
        StartY = y;
        c = colour(255, 0, 0, 0);
    }