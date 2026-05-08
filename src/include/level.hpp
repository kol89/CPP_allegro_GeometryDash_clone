#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include "block.hpp"
#include "spike.hpp"
#include "portal.hpp"
#include "speed_portal.hpp"
#include "orb.hpp"
#include "pad.hpp"

class level {
    public:
    std::vector<spike> danger;
    std::vector<block> colidable;
    std::vector<portal> portals;
    std::vector<speed_portal> speed;
    std::vector<orb> orbs;
    std::vector<pad> pads;
    double level_shiftx;
    double level_shifty;
    char mode;
    int sp;
    double end;
    level(std::vector<spike> _danger, std::vector<block> _colidable, std::vector<portal> _portals, std::vector<speed_portal> _speed, std::vector<orb> _orbs, std::vector<pad> _pads, double _end, char _mode, int _sp);
    void render();
    void move(double sp);
    void shift(double x, double y);
};

#endif
