#ifndef GVAR_H
#define GVAR_H

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro.h>
#include <allegro5/bitmap_draw.h>
#include <allegro5/allegro_font.h>
#include <string>
#include "rectangle.hpp"

inline ALLEGRO_KEYBOARD_STATE state;
inline ALLEGRO_MOUSE_STATE statem;
inline double pointer_gx=0;
inline double pointer_gy=0;
inline double pointer_shiftx=0;
inline double pointer_shifty=0;
inline int mx = 0;
inline int my = 0;
inline int msx = 0;
inline int msy = 0;
const float local_game_speed = 1.4;
const double fps = 60/local_game_speed;
const float WIDTH = 1280; //640
const float HEIGHT = 720; //360
const double grid_size = WIDTH/25; //12.8
const double Pi = 3.14159265359;
inline double g = 1500/50*grid_size;
const double jumpSpeed = -600;
const double flightAcceleration = -g/1.4;
inline char* player_sk = "src/img/skin.png";
inline ALLEGRO_FONT* font = nullptr;
inline ALLEGRO_BITMAP* skin_texture = nullptr;
inline ALLEGRO_BITMAP* spike_texture = nullptr;
inline ALLEGRO_BITMAP* block_texture = nullptr;
const double gr = HEIGHT-grid_size*1.5;
inline int attempts = 0;
inline int frame = 0;
inline int stat = 0;
inline int lvl = 1;
inline bool press = false;
inline bool press_orb = false;
inline bool on_ground = false;
inline std::string mod;
inline rectangle ground(0, HEIGHT-grid_size*1.5, 0, 0, WIDTH, grid_size*10, 0, 0, colour(0, 102, 255, 0));

#endif