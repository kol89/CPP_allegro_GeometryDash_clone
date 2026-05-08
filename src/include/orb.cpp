#include "include.hpp"

orb::orb(double _x, double _y, char _type) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = grid_size;
        sy = grid_size;
        x = _x * grid_size;
        type = _type;
        y = ground.y - _y * grid_size;
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
        case 'g':
            c = colour(104, 254, 105, 0);
            break;
        case 'k':
            c = colour(50, 50, 50, 0);
            break;
        default:
            c = colour(0, 100, 100, 0);
        }
        
    }