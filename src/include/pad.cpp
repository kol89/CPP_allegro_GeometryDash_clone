#include "gvar.hpp"
#include "include.hpp"

pad::pad(double _x, double _y, char _type) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = grid_size;
        sy = 5;
        type = _type;
        x = _x * grid_size;
        y = ground.y - _y * grid_size + grid_size-sy;
        StartX = x;
        StartY = y;
        switch (_type) {
        case 'p':
            c = colour(252, 103, 252, 0);
            break;
        case 'y':
            c = colour(255, 254, 108, 0);
            break;
        case 'r':
            c = colour(255, 97, 40, 0);
            break;
        case 'b':
            c = colour(116, 254, 254, 0);
            break;
        default:
            c = colour(0, 100, 100, 0);
        }
    }