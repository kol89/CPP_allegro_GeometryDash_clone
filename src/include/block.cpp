#include "include.hpp"

block::block(double _x, double _y) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = grid_size; 
        sy = grid_size;
        x = _x * grid_size;
        y = ground.y - _y * grid_size;
        StartX = x;
        StartY = y;
        c = colour(0, 0, 255, 0);
    }