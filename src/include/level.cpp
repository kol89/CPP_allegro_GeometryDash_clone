#include "include.hpp"

level::level(std::vector<spike> _danger, std::vector<block> _colidable, std::vector<portal> _portals, std::vector<speed_portal> _speed, std::vector<orb> _orbs, std::vector<pad> _pads, double _end, char _mode, int _sp) {
        danger = _danger;
        colidable = _colidable;
        portals = _portals;
        speed = _speed;
        orbs = _orbs;
        pads = _pads;
        end = _end;
        mode = _mode;
        sp = _sp;
        level_shiftx=0;
        level_shifty=0;
    }

void level::render() {
        if (danger.empty() == 0) {
            if (spike_texture == nullptr) {
                al_init_image_addon();
                spike_texture = al_load_bitmap("src/img/spike.png");
            }
            for (int i = 0; i < danger.size(); i++) {
                if(danger[i].x<=WIDTH && danger[i].x+danger[i].sx>=0){
                    double tx = danger[i].x-20;
                    double ty = danger[i].y-14.29;
                    //al_draw_bitmap(spike_texture, tx, ty, 0);
                    al_draw_scaled_bitmap(spike_texture, 0, 0, 1000, 1000, tx, ty, grid_size, grid_size, 0);
                }
                //danger[i].rend();
            }
        }
        if (colidable.empty() == 0) {
            if (block_texture == nullptr) {
                al_init_image_addon();
                block_texture = al_load_bitmap("src/img/block.jpg");
            }
            for (int i = 0; i < colidable.size(); i++) {
                if(colidable[i].x<=WIDTH && colidable[i].x+colidable[i].sx>=0){
                double tx = colidable[i].x;
                double ty = colidable[i].y;
                al_draw_scaled_bitmap(block_texture, 0, 0, 50, 50, tx, ty, grid_size, grid_size, 0);
                }
                //colidable[i].rend();
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

void level::move(double sp) {
        double s = sp / fps;
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

void level::shift(double x, double y) {
        double s = x / fps;
        double sy = y / fps;
        level_shiftx+=s;
        level_shifty+=sy;
            if (danger.empty() == 0) {
                for (int i = 0; i < danger.size(); i++) {
                    danger[i].x = danger[i].x - s;
                    danger[i].y = danger[i].y - sy;
                }
            }
            if (colidable.empty() == 0) {
                for (int i = 0; i < colidable.size(); i++) {
                    colidable[i].x = colidable[i].x - s;
                    colidable[i].y = colidable[i].y - sy;
                }
            }
            if (portals.empty() == 0) {
                for (int i = 0; i < portals.size(); i++) {
                    portals[i].x = portals[i].x - s;
                    portals[i].y = portals[i].y - sy;
                }
            }
            if (speed.empty() == 0) {
                for (int i = 0; i < speed.size(); i++) {
                    speed[i].x = speed[i].x - s;
                    speed[i].y = speed[i].y - sy;
                }
            }
            if (orbs.empty() == 0) {
                for (int i = 0; i < orbs.size(); i++) {
                    orbs[i].x = orbs[i].x - s;
                    orbs[i].y = orbs[i].y - sy;
                }
            }
            if (pads.empty() == 0) {
                for (int i = 0; i < pads.size(); i++) {
                    pads[i].x = pads[i].x - s;
                    pads[i].y = pads[i].y - sy;
                }
            }
    }