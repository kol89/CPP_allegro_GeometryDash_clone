#include "gvar.hpp"
#include "include.hpp"

void player::chsp(int s) {
        switch (s) {
        case 0:
            vx = 25/50*grid_size;
        case 1:
            vx = 200/50*grid_size;
        case 2:
            vx = 300/50*grid_size;
        }
        sync();
    }

player::player(double _x, double _y, char _mode, int _speed) {
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
        sx = grid_size;
        sy = grid_size;
        ax = 0;
        ay = 0;
        c = colour(0, 0, 0, 255);
        hit_box = rectangle(x, y, vx, vy, sx, sy, ax, ay, colour(255, 0, 0, 0));
        skin = rotateble_rectangle(x, y, vx, vy, sx, sy, ax, ay, grad, colour(0, 0, 0, 0));
        collision_box = rectangle(x + (sx / 3), y + (sy / 3), vx, vy, sx / 3, sy / 3, ax, ay, colour(0, 0, 255, 0));
    }

void player::sync() {
        double grad = skin.grad;
        if (player_sk=="src/img/shipskin.png"){
            skin = rotateble_rectangle(x-(grid_size/2), y-(grid_size/2), vx, vy, sx+(grid_size/2), sy+(grid_size/2), ax, ay, grad, colour(255, 255, 255, 0));
            std::cout<<player_sk;
        }else{
            skin = rotateble_rectangle(x, y, vx, vy, sx, sy, ax, ay, grad, colour(255, 255, 255, 0));
        }
        skin.rotate();
        hit_box = rectangle(x, y, vx, vy, sx, sy, ax, ay, colour(255, 0, 0, 0));
        collision_box = rectangle(x + (sx / 3), y + (sy / 3), vx, vy, sx / 3, sy / 3, ax, ay, colour(0, 0, 255, 0));
    }

void player::pmove() {
        x += vx / fps;
        y += (vy/50*grid_size) / fps;
        sync();
    }

void player::paccelerate() {
        vx += ax / fps;
        if (flip_gr) { vy -= ay / fps; }
        else { vy += ay / fps; }
        sync();
    }
bool player::collision(level l) {
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
                                vy = jumpSpeed / 1.5; press = true;
                            }
                            break;
                        case 'y':
                            if (!press) {
                                vy = jumpSpeed; press = true;
                            }
                            break;
                        case 'r':
                            if (!press) {
                                vy = jumpSpeed * 1.5; press = true;
                            }
                            break;
                        case 'b':
                            if (flip_gr) {
                                if (!press) {
                                    flip_gr = !flip_gr; vy = -jumpSpeed/2; press = true;
                                }
                            }
                            else {
                                if (!press) {
                                    flip_gr = !flip_gr; vy = jumpSpeed/2; press = true;
                                }
                            }
                            break;
                        case 'g':
                            if (flip_gr) {
                                if (!press) {
                                    flip_gr = !flip_gr; vy = jumpSpeed; press = true;
                                }
                            }
                            else {
                                if (!press) {
                                    flip_gr = !flip_gr; vy = -jumpSpeed; press = true;
                                }
                            }
                            break;
                        case 'k':
                            if (!press) {
                                vy -= jumpSpeed; press = true;
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
                        if (l.pads[i].activated==false) {
                            switch (l.pads[i].type) {
                            case 'p':
                                vy = jumpSpeed / 1.5;
                                l.pads[i].activated = true;
                                break;
                            case 'y':
                                vy = jumpSpeed * 1.2;
                                l.pads[i].activated = true;
                                break;
                            case 'r':
                                vy = jumpSpeed * 1.5;
                                l.pads[i].activated = true;
                                break;
                            case 'b':
                                flip_gr = true;
                                vy = jumpSpeed / 2;
                                //std::cout << l.pads[i].activated<<"---------";
                                l.pads[i].activated = true;
                                break;
                            default:
                                l.pads[i].activated = true;
                            }
                        }
                    
                }
            }
        }

        return false;
    }