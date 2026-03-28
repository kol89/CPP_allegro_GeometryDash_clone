#include "engine.hpp"
#include <allegro5/allegro_primitives.h>
#include <cstddef>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <vector>
#include <charconv>
ALLEGRO_KEYBOARD_STATE state;
const float FPS = 60;
const float WIDTH = 640;
const float HEIGHT = 360;
const double Pi = 3.14159265359;
const double g = 1500;
const double jumpSpeed = -600;
const double flightAcceleration = -700;
ALLEGRO_FONT* font = nullptr;
int attempts = 0;
int frame = 0;
int stat = 0;
int lvl = 1;
bool press = false;
bool press_orb = false;
bool on_ground = false;
std::string mod;

struct colour {
    int r; // red channel
    int g; // green channel
    int b; // blue channel
    int a; // alpha channel
    colour(int _r, int _g, int _b, int _a) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
    colour() {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }
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
    rectangle(double _x, double _y, double _vx, double _vy, double _sx, double _sy, double _ax, double _ay, colour _c) {
        x = _x;
        y = _y;
        StartX = _x;
        StartY = _y;
        vx = _vx;
        vy = _vy;
        sx = _sx;
        sy = _sy;
        ax = _ax;
        ay = _ay;
        c = _c;
    }
    rectangle() {
        x = 0;
        y = 0;
        StartX = 0;
        StartY = 0;
        vx = 0;
        vy = 0;
        sx = 1;
        sy = 1;
        ax = 0;
        ay = 0;
        c = colour();
    }
    void move() {
        x += vx / 60;
        y += vy / 60;
    }
    void accelerate() {
        vx += ax / 60;
        vy += ay / 60;
    }
    void rend() {
        al_draw_filled_rectangle(x, y, x + sx, y + sy, al_map_rgb(c.r, c.g, c.b));
    }
    void reset() {
        x = StartX;
        y = StartY;
    }
    bool chkcol(rectangle a) {
        return a.x < x + sx && a.x + a.sx > x && a.y < y + sy && a.y + a.sy > y;
    }
};

class rotateble_rectangle : public shape {
public:
    double grad;
    double x1, y1, x2, y2, x3, y3;
    rotateble_rectangle(double _x, double _y, double _vx, double _vy, double _sx, double _sy, double _ax, double _ay, double _grad, colour _c) {
        grad = _grad;
        x = _x;
        y = _y;

        // x1 = _x;
        // y1 = _sy+_y;

        // x2 = _sx+_x;
        // y2 = _y;

        // x3 = _sx+_x;
        // y3 = _sy+_y;

        x1 = _sx + _x;
        y1 = _y;

        x2 = _sx + _x;
        y2 = _sy + _y;

        x3 = _x;
        y3 = _sy + _y;

        StartX = _x;
        StartY = _y;
        vx = _vx;
        vy = _vy;
        ax = _ax;
        ay = _ay;
        c = _c;
    }
    rotateble_rectangle() {
        x = 0;
        y = 0;
        StartX = 0;
        StartY = 0;
        vx = 0;
        vy = 0;
        x1 = 0;
        y1 = 0;

        x2 = 0;
        y2 = 0;

        x3 = 0;
        y3 = 0;
        ax = 0;
        ay = 0;
        c = colour();
    }
    void move() {
        x += vx / 60;
        y += vy / 60;
    }
    void accelerate() {
        vx += ax / 60;
        vy += ay / 60;
    }
    void rend() {
        //al_draw_filled_rectangle(x, y, x + sx, y + sy, al_map_rgb(c.r, c.g, c.b));
        float verts[] = {
            x,      y,
            x1,   y1,
            x2,   y2,
            x3,      y3
        };
        al_draw_polygon(verts, 4, ALLEGRO_LINE_JOIN_ROUND, al_map_rgb(c.r, c.g, c.b), 1.0, 0.0);
    }
    void reset() {
        x = StartX;
        y = StartY;
    }

    void rotate() {
        double theta = grad * (Pi / 180);
        double sinus = std::sin(theta);
        double cosinus = std::cos(theta);
        double centerX = (x + x2) / 2;
        double centerY = (y + y2) / 2;
        double dx, dy;

        dx = x - centerX; dy = y - centerY;
        x = centerX + dx * cosinus - dy * sinus;
        y = centerY + dx * sinus + dy * cosinus;

        dx = x1 - centerX; dy = y1 - centerY;
        x1 = centerX + dx * cosinus - dy * sinus;
        y1 = centerY + dx * sinus + dy * cosinus;

        dx = x2 - centerX; dy = y2 - centerY;
        x2 = centerX + dx * cosinus - dy * sinus;
        y2 = centerY + dx * sinus + dy * cosinus;

        dx = x3 - centerX; dy = y3 - centerY;
        x3 = centerX + dx * cosinus - dy * sinus;
        y3 = centerY + dx * sinus + dy * cosinus;
    }
};

rectangle ground(0, 300, 0, 0, 700, 60, 0, 0, colour(255, 165, 0, 0));
class block : public rectangle {
public:
    block(double _x, double _y) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = 50;
        sy = 50;
        x = _x * 50;
        y = ground.y - _y * 50;
        StartX = x;
        StartY = y;
        c = colour(0, 0, 255, 0);
    }
};

class spike : public rectangle {
public:
    spike(double _x, double _y) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = 7.14;
        sy = 20.71;
        x = _x * 50 + 20;
        y = ground.y - _y * 50 + 14.29;
        StartX = x;
        StartY = y;
        c = colour(255, 0, 0, 0);
    }
};

class portal : public rectangle {
public:
    char mode;
    portal(double _x, double _y, char _mode) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = 50;
        sy = 150;
        mode = _mode;
        x = 50 + _x * 50;
        y = ground.y - _y * 50;
        StartX = x;
        StartY = y;
        c = colour(0, 0, 255, 0);
    }
};

class speed_portal : public rectangle {
public:
    char spd;
    speed_portal(double _x, double _y, char _spd) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = 50;
        sy = 150;
        spd = _spd;
        x = 50 + _x * 50;
        y = ground.y - _y * 50;
        StartX = x;
        StartY = y;
        c = colour(0, 0, 255, 0);
    }
};

class orb : public rectangle {
public:
    char type;
    orb(double _x, double _y, char _type) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = 50;
        sy = 50;
        x = _x * 50;
        type = _type;
        y = ground.y - _y * 50;
        StartX = x;
        StartY = y;
        c = colour(0, 100, 100, 0);
    }
};

class pad : public rectangle {
public:
    char type;
    pad(double _x, double _y, char _type) {
        ax = 0;
        ay = 0;
        vx = 0;
        vy = 0;
        sx = 50;
        sy = 10;
        type = _type;
        x = _x * 50;
        y = ground.y - _y * 50- 400;
        StartX = x;
        StartY = y;
        c = colour(0, 100, 100, 0);
    }
};

class button : public rectangle {
public:
    char const* text;
    bool is_pressed() {
        ALLEGRO_MOUSE_STATE state;
        al_get_mouse_state(&state);
        return state.x >= x & state.y >= y & state.x <= x + sx & state.y <= y + sy & al_mouse_button_down(&state, ALLEGRO_MOUSE_BUTTON_LEFT);
    }
    button(double _x, double _y, double _sx, double _sy, char const* _text, colour _c) {
        x = _x;
        y = _y;
        sx = _sx;
        sy = _sy;
        text = _text;
        vx = 0;
        vy = 0;
        ax = 0;
        ay = 0;
        c = _c;
    }
    void brend() {
        rend();
        al_draw_text(
            font,
            al_map_rgb(0, 0, 0),
            (x + sx / 2) - 45, (y + sy / 2) - 20,
            ALLEGRO_ALIGN_LEFT,
            text
        );
    }
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
    level(std::vector<spike> _danger, std::vector<block> _colidable, std::vector<portal> _portals, std::vector<speed_portal> _speed, std::vector<orb> _orbs, std::vector<pad> _pads, double _end, char _mode, int _sp) {
        danger = _danger;
        colidable = _colidable;
        portals = _portals;
        speed = _speed;
        orbs = _orbs;
        pads = _pads;
        end = _end;
        mode = _mode;
        sp = _sp;
    }
    void render() {
        if (danger.empty() == 0) {
            for (int i = 0; i < danger.size(); i++) {
                danger[i].rend();
            }
        }
        if (colidable.empty() == 0) {
            for (int i = 0; i < colidable.size(); i++) {
                colidable[i].rend();
            }
        }
        if (portals.empty() == 0) {
            for (int i = 0; i < portals.size(); i++) {
                portals[i].rend();
            }
        }
        if (speed.empty() == 0) {
            for (int i = 0; i < speed.size(); i++) {
                speed[i].rend();
            }
        }
        if (orbs.empty() == 0) {
            for (int i = 0; i < orbs.size(); i++) {
                orbs[i].rend();
            }
        }
        if (pads.empty() == 0) {
            for (int i = 0; i < pads.size(); i++) {
                pads[i].rend();
            }
        }
    }
    void move(double sp) {
        double s = sp / 60;
            if (danger.empty() == 0) {
                for (int i = 0; i < danger.size(); i++) {
                    danger[i].x = danger[i].x - s;
                }
            }
            if (colidable.empty() == 0) {
                for (int i = 0; i < colidable.size(); i++) {
                    colidable[i].x = colidable[i].x - s;
                }
            }
            if (portals.empty() == 0) {
                for (int i = 0; i < portals.size(); i++) {
                    portals[i].x = portals[i].x - s;
                }
            }
            if (speed.empty() == 0) {
                for (int i = 0; i < speed.size(); i++) {
                    speed[i].x = speed[i].x - s;
                }
            }
            if (orbs.empty() == 0) {
                for (int i = 0; i < orbs.size(); i++) {
                    orbs[i].x = orbs[i].x - s;
                }
            }
            if (pads.empty() == 0) {
                for (int i = 0; i < pads.size(); i++) {
                    pads[i].x = pads[i].x - s;
                }
            }
    }
};

class player : public rectangle {
public:
    double grad;
    char mode;
    bool flip_gr;
    rotateble_rectangle skin;
    rectangle collision_box;
    rectangle hit_box;
    void chsp(int s) {
        switch (s) {
        case 0:
            vx = 25;
        case 1:
            vx = 200;
        case 2:
            vx = 300;
        }
        sync();
    }
    player(double _x, double _y, char _mode, int _speed) {
        mode = _mode;
        grad = 0;
        flip_gr = false;
        x = _x;
        y = _y;
        StartX = _x;
        StartY = _y;
        vx = 0;
        vy = 0;
        chsp(_speed);
        sx = 50;
        sy = 50;
        ax = 0;
        ay = 0;
        c = colour(0, 0, 0, 255);
        hit_box = rectangle(x, y, vx, vy, sx, sy, ax, ay, colour(255, 0, 0, 0));
        skin = rotateble_rectangle(x, y, vx, vy, sx, sy, ax, ay, grad, colour(255, 255, 255, 0));
        collision_box = rectangle(x + (sx / 3), y + (sy / 3), vx, vy, sx / 3, sy / 3, ax, ay, colour(0, 0, 255, 0));
    }
    void sync() {
        double grad = skin.grad;
        skin = rotateble_rectangle(x, y, vx, vy, sx, sy, ax, ay, grad, colour(255, 255, 255, 0));
        skin.rotate();
        hit_box = rectangle(x, y, vx, vy, sx, sy, ax, ay, colour(255, 0, 0, 0));
        collision_box = rectangle(x + (sx / 3), y + (sy / 3), vx, vy, sx / 3, sy / 3, ax, ay, colour(0, 0, 255, 0));
    }
    void pmove() {
        x += vx / 60;
        y += vy / 60;
        sync();
    }
    void paccelerate() {
        vx += ax / 60;
        if (flip_gr) { vy -= ay / 60; }
        else { vy += ay / 60; }
        sync();
    }
    bool collision(level l) {
        if (l.danger.empty() == 0) {
            for (int i = 0; i < l.danger.size(); i++) {
                if (hit_box.chkcol(l.danger[i])) {
                    return true;
                }
            }
        }

        if (l.colidable.empty() == 0) {
            for (int i = 0; i < l.colidable.size(); i++) {
                if (hit_box.chkcol(l.colidable[i])) {
                    if (flip_gr) {
                        if (collision_box.y + collision_box.sy > l.colidable[i].y) {
                            y = l.colidable[i].y + hit_box.sy;
                            vy = 0;
                            on_ground = true;
                        }
                    }
                    else {
                        if (collision_box.y + collision_box.sy < l.colidable[i].y) {
                            y = l.colidable[i].y - hit_box.sy;
                            vy = 0;
                            on_ground = true;
                        }
                    }
                    if (collision_box.chkcol(l.colidable[i])) {
                        return true;
                    }
                }
            }
        }

        if (l.portals.empty() == 0) {
            for (int i = 0; i < l.portals.size(); i++) {
                if (hit_box.chkcol(l.portals[i])) {
                    mode = l.portals[i].mode;
                }
            }
        }

        if (l.speed.empty() == 0) {
            for (int i = 0; i < l.speed.size(); i++) {
                if (hit_box.chkcol(l.speed[i])) {
                    vx = l.speed[i].spd;
                }
            }
        }

        al_get_keyboard_state(&state);
        if (al_key_down(&state, ALLEGRO_KEY_UP) || al_key_down(&state, ALLEGRO_KEY_SPACE)) {
            if (l.orbs.empty() == 0) {
                for (int i = 0; i < l.orbs.size(); i++) {
                    if (hit_box.chkcol(l.orbs[i])) {
                        switch (l.orbs[i].type) {
                        case 'p':
                            std::cout << "PINK";
                            if (!press) {
                                vy = jumpSpeed / 2; press = true;
                            }
                            break;
                        case 'y':
                            if (!press) {
                                vy = jumpSpeed; press = true;
                            }
                            break;
                        case 'r':
                            if (!press) {
                                vy = jumpSpeed * 2; press = true;
                            }
                            break;
                        case 'b':
                            if (!press) {
                                flip_gr = !flip_gr; press = true;
                            }
                            break;
                        default:
                            std::cout << "NOTHING";
                        }
                    }
                }
            }
        }
        else { press = false; }

        if (l.pads.empty() == 0) {
            for (int i = 0; i < l.pads.size(); i++) {
                if (hit_box.chkcol(l.pads[i])) {
                    switch (l.orbs[i].type) {
                    case 'p':
                        if (!press) {
                            vy = jumpSpeed / 2;
                        }
                        break;
                    case 'y':
                        if (!press) {
                            vy = jumpSpeed;
                        }
                        break;
                    case 'r':
                        if (!press) {
                            vy = jumpSpeed * 2;
                        }
                        break;
                    case 'b':
                        if (!press) {
                            flip_gr = !flip_gr;
                        }
                    }
                }
            }
        }

        return false;
    }
};
player p = player(0, 300, 'c', 1);



std::vector<spike> danger;
std::vector<block> colidable;
std::vector<portal> portals;
/*
    all modes:
    c - cube    DONE
    s - ship    DONE
    b - saw     DONE
    u - UFO     WIP
    w - wave    WIP
    r - robot   WIP
    p - spider  WIP
    g - swing   WIP
*/
std::vector<speed_portal> speed;
std::vector<orb> orbs;
/*
    all types:
    p - pink     DONE
    y - yelow    DONE
    r - red      DONE
    b - blue     DONE
    g - green    WIP
    k - black    WIP
    d - dash     WIP
*/
std::vector<pad> pads;
/*
    all types:
    p - pink     DONE
    y - yelow    DONE
    r - red      DONE
    b - blue     DONE
*/

level level1(
    danger = {
    spike(10, 1),

    spike(18, 1),
    spike(19, 1),
    spike(28, 1),
    spike(29, 1),

    spike(46, 1),
    spike(47, 1),

    spike(61, 1),
    spike(62, 1),

    spike(75, 1),
    spike(76, 1),
    },
    colidable = {
    block(30, 1),

    block(34, 1),
    block(34, 2),

    block(38, 1),
    block(38, 2),
    block(38, 3),

    block(55, 1),
    block(56, 1),
    block(57, 1),
    block(58, 1),
    block(59, 1),
    block(60, 1),

    block(63, 1),
    block(64, 1),
    block(65, 1),
    block(66, 1),
    block(63, 2),
    block(64, 2),
    block(65, 2),
    block(66, 2),
    },
    portals = {},
    speed = {},
    orbs = {},
    pads = {},
    1000,
    'c',
    1
    );
level level2(
    danger = {
    spike(10, 1),
    spike(11, 1),
    spike(12, 1)
    },
    colidable = {
        block(9, 1),
        block(9, 2)
    },
    portals = {},
    speed = {},
    orbs = {
    orb(13, 3, 'p')
    },
    pads = {},
    1000,
    'c',
    1
    );

void reset_all() {
    p.reset();
    font = nullptr;
    switch (lvl) {
    case 1:
        for (int i = 0; i < level1.danger.size(); i++) { level1.danger[i].reset(); }
        for (int i = 0; i < level1.colidable.size(); i++) { level1.colidable[i].reset(); }
        for (int i = 0; i < level1.portals.size(); i++) { level1.portals[i].reset(); }
        for (int i = 0; i < level1.speed.size(); i++) { level1.speed[i].reset(); }
        for (int i = 0; i < level1.orbs.size(); i++) { level1.orbs[i].reset(); }
        for (int i = 0; i < level1.pads.size(); i++) { level1.pads[i].reset(); }
        p.chsp(level1.sp);
        p.mode = level1.mode;
    case 2:
        for (int i = 0; i < level2.danger.size(); i++) { level2.danger[i].reset(); }
        for (int i = 0; i < level2.colidable.size(); i++) { level2.colidable[i].reset(); }
        for (int i = 0; i < level2.portals.size(); i++) { level2.portals[i].reset(); }
        for (int i = 0; i < level2.speed.size(); i++) { level2.speed[i].reset(); }
        for (int i = 0; i < level2.orbs.size(); i++) { level2.orbs[i].reset(); }
        for (int i = 0; i < level2.pads.size(); i++) { level2.pads[i].reset(); }
        p.chsp(level2.sp);
        p.mode = level2.mode;
    }
    attempts++;
}

button l1(100, 120, 170, 100, "level 1", colour(255, 255, 255, 0));
button l2(350, 120, 170, 100, "level 2", colour(255, 255, 255, 0));



//~----------------------Main program------------------------
class GameEngine :public Engine {

    //*---FPS---+
    virtual void physics_process() override {
        if (stat == 0) {
            if (attempts != 0) {
                attempts = 0;
            }
            if (l1.is_pressed()) {
                font = nullptr;
                lvl = 1;
                stat = 1;
            }
            if (l2.is_pressed()) {
                font = nullptr;
                lvl = 2;
                stat = 1;
            }
        }
        if (stat == 1) {
            if (frame == FPS) {
                frame = 0;
            }
            std::cout << frame << std::endl;
            
            al_get_keyboard_state(&state);
            if (al_key_down(&state, ALLEGRO_KEY_ESCAPE)) {
                reset_all();
                stat = 0;
                return;
            }
            on_ground = false;
            p.ay = g;

            if (p.hit_box.y + p.hit_box.sy >= ground.y) {
                p.y = ground.y - p.hit_box.sy;
                p.vy = 0;
                on_ground = true;
            }
            if (p.vy < -1000) {
                reset_all();
            }
            
            
            switch (lvl) {
            case 1:
                p.mode = level1.mode;
                p.chsp(level1.sp);
                if (p.x > 150) {
                    p.x = p.x - p.vx / 60;
                    level1.move(p.vx);
                }
                if (p.collision(level1)) {
                    reset_all();
                }
                break;

            case 2:
                p.mode = level2.mode;
                p.chsp(level2.sp);
                if (p.x > 150) {
                    p.x = p.x - p.vx / 60;
                    level2.move(p.vx);
                }
                if (p.collision(level2)) {
                    reset_all();
                }
                break;
            }
            

            if (al_key_down(&state, ALLEGRO_KEY_UP) || al_key_down(&state, ALLEGRO_KEY_SPACE)) {
                switch (p.mode) {
                case 's':
                    p.ay = flightAcceleration;
                    break;
                case 'c':
                    if (on_ground) { p.vy = jumpSpeed; }
                    break;
                case 'b':
                    if (on_ground && !press) { p.flip_gr = !p.flip_gr; press = true; }
                    break;
                }
            }
            else { press = false; }

            if (al_key_down(&state, ALLEGRO_KEY_S)) {
                switch (lvl) {
                case 1:
                    level1.mode = 's';
                case 2:
                    level2.mode = 's';
                }
            }
            if (al_key_down(&state, ALLEGRO_KEY_C)) {
                switch (lvl) {
                case 1:
                    level1.mode = 'c';
                case 2:
                    level2.mode = 'c';
                }
            }
            if (al_key_down(&state, ALLEGRO_KEY_B)) {
                switch (lvl) {
                case 1:
                    level1.mode = 'b';
                case 2:
                    level2.mode = 'b';
                }
            }
            if (al_key_down(&state, ALLEGRO_KEY_F)) {
                p.flip_gr = true;
            }
            if (al_key_down(&state, ALLEGRO_KEY_G)) {
                p.flip_gr = false;
            }
            if (al_key_down(&state, ALLEGRO_KEY_R)) {
                reset_all();
            }
            p.sync();
            if (!on_ground) {
                p.skin.grad += 3.8;
            }
            if (on_ground) {
                p.skin.grad = 0;
            }
            p.skin.rotate();
            p.paccelerate();
            p.pmove();

            frame++;
        }

    }

    //*---draw---+
    virtual void render_process() override {
        if (stat == 0) {
            if (font == nullptr) {
                font = al_load_ttf_font(
                    "src/arial.ttf", // font file
                    32,         // font size
                    0           // flags (just use zero)
                );
            }
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(
                font,
                al_map_rgb(255, 255, 255),
                150, 50,
                ALLEGRO_ALIGN_LEFT,
                "Geography Jump -alpha-"
            );
            l1.brend();
            l2.brend();
        }
        if (stat == 1) {
            if (font == nullptr) {
                font = al_load_ttf_font(
                    "src/arial.ttf", // font file
                    16,         // font size
                    0           // flags (just use zero)
                );
            }

            switch (p.mode) {
            case 'c':
                mod = "mode: cube";
                break;
            case 's':
                mod = "mode: ship";
                break;
            case 'b':
                mod = "mode: saw";
                break;
            case 'u':
                mod = "mode: UFO";
                break;
            case 'w':
                mod = "mode: wave";
                break;
            case 'r':
                mod = "mode: robot";
                break;
            case 'p':
                mod = "mode: spider";
                break;
            case 'g':
                mod = "mode: swing";
                break;

            }

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(
                font,
                al_map_rgb(255, 255, 255),
                5, 5,
                ALLEGRO_ALIGN_LEFT,
                mod.c_str()
            );
            if (p.flip_gr) {
                mod = "gravity: flipped";
            }
            else {
                mod = "gravity: normal";
            }
            al_draw_text(
                font,
                al_map_rgb(255, 255, 255),
                5, 20,
                ALLEGRO_ALIGN_LEFT,
                mod.c_str()
            );
            al_draw_text(
                font,
                al_map_rgb(255, 255, 255),
                5, 45,
                ALLEGRO_ALIGN_LEFT,
                std::to_string(attempts).c_str()
            );
            p.skin.rend();
            //p.hit_box.rend();
            //p.collision_box.rend();
            ground.rend();
            if (lvl == 1) {
                level1.render();
            }
            if (lvl == 2) {
                level2.render();
            }
        }

    }
};
//~--------------------------------------------------------------


int main() {
    GameEngine game;
    game.start();
    return 0;
}
