#ifndef OBJECTS
#define OBJECTS
#include <vector>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <cstddef>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <vector>
#include <charconv>

struct colour {
    int r; // red channel
    int g; // green channel
    int b; // blue channel
    int a; // alpha channel
    colour(int _r, int _g, int _b, int _a);
    colour();
};

class shape/*dot*/ {
    public:
    double x; // position x
    double y; // position y
    double StartX; // initial position x
    double StartY; // initial position y
    double vx; // velocity x
    double vy; // velocity y
    double ax; // acceleration x
    double ay; // acceleration y
    colour c; // color
};

class rectangle : public shape {
    public:
    double sx; // size x
    double sy; // size y
    rectangle(double _x, double _y, double _vx, double _vy, double _sx, double _sy, double _ax, double _ay, colour _c);
    rectangle();
    void move();
    void accelerate();
    void rend();
    void reset();
    bool chkcol(rectangle a) {return a.x < x + sx && a.x + a.sx > x && a.y < y + sy && a.y + a.sy > y;}
};

class rotateble_rectangle : public shape {
    public:
    double grad;
    double x1, y1, x2, y2, x3, y3;
    rotateble_rectangle(double _x, double _y, double _vx, double _vy, double _sx, double _sy, double _ax, double _ay, double _grad, colour _c);
    rotateble_rectangle();
    void move();
    void accelerate();
    void rend();
    void reset();
    void rotate();
};

class block : public rectangle {
    public:
    block(double _x, double _y);
};

class spike : public rectangle {
    public:
    spike(double _x, double _y);
};

class portal : public rectangle {
    public:
    char mode;
    portal(double _x, double _y, char _mode);
};

class speed_portal : public rectangle {
    public:
    char spd;
    speed_portal(double _x, double _y, char _spd);
};

class orb : public rectangle {
    public:
    char type;
    orb(double _x, double _y, char _type);
};

class pad : public rectangle {
    public:
    char type;
    bool activated = false;
    pad(double _x, double _y, char _type);
};

class button : public rectangle {
 public:
    char const* text;
    bool is_pressed();
    button(double _x, double _y, double _sx, double _sy, char const* _text, colour _c);
    void brend();
};

class level {
    public:
    std::vector<spike> danger;
    std::vector<block> colidable;
    std::vector<portal> portals;
    std::vector<speed_portal> speed;
    std::vector<orb> orbs;
    std::vector<pad> pads;
    char mode;
    int sp;
    double end;
    level(std::vector<spike> _danger, std::vector<block> _colidable, std::vector<portal> _portals, std::vector<speed_portal> _speed, std::vector<orb> _orbs, std::vector<pad> _pads, double _end, char _mode, int _sp);
    void render();
    void move(double sp);
};

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