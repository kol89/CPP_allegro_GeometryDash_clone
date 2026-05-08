#include "gvar.hpp"
#include "include.hpp"

portal::portal(double _x, double _y, char _mode) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = grid_size;
        sy = grid_size*3;
        mode = _mode;
        x = _x * grid_size;
        y = ground.y - _y * grid_size;
        StartX = x;
        StartY = y;
        switch (mode) {
            case 'c':
            break;
            case 's':
            break;
            case 'b':
            break;
            case 'u':
            break;
            case 'w':
            break;
            case 'r':
            break;
            case 'p':
            break;
            case 'g':
            break;
        }
        c = colour(0, 0, 255, 0);
    }