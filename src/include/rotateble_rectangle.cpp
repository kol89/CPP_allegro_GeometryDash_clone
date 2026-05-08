#include "gvar.hpp"
#include "include.hpp"

rotateble_rectangle::rotateble_rectangle(double _x, double _y, double _vx, double _vy, double _sx, double _sy, double _ax, double _ay, double _grad, colour _c) {
        grad = _grad;
        x = _x;
        y = _y;

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

rotateble_rectangle::rotateble_rectangle() {
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

void rotateble_rectangle::move() {
        x += vx / fps;
        y += vy / fps;
}

void rotateble_rectangle::accelerate() {
        vx += ax / fps;
        vy += ay / fps;
}

void rotateble_rectangle::rend() {
        //al_draw_filled_rectangle(x, y, x + sx, y + sy, al_map_rgb(c.r, c.g, c.b));
        float verts[] = {
            static_cast<float>(x),      static_cast<float>(y),
            static_cast<float>(x1),   static_cast<float>(y1),
            static_cast<float>(x2),   static_cast<float>(y2),
            static_cast<float>(x3),      static_cast<float>(y3)
        };
        if (skin_texture == nullptr) {
            al_init_image_addon();
            skin_texture = al_load_bitmap(player_sk);
        }
        
        float tw = al_get_bitmap_width(skin_texture);
        float th = al_get_bitmap_height(skin_texture);
        ALLEGRO_VERTEX quad[] = {
            {.x = static_cast<float>(x), .y = static_cast<float>(y), .z = 0, .u = 0,  .v = 0,  .color = al_map_rgb(255,255,255) }, // top-left
            {.x = static_cast<float>(x1), .y = static_cast<float>(y1), .z = 0, .u = tw, .v = 0,  .color = al_map_rgb(255,255,255) }, // top-right
            {.x = static_cast<float>(x2), .y = static_cast<float>(y2), .z = 0, .u = tw, .v = th, .color = al_map_rgb(255,255,255) }, // bottom-right
            {.x = static_cast<float>(x3), .y = static_cast<float>(y3), .z = 0, .u = 0,  .v = th, .color = al_map_rgb(255,255,255) }, // bottom-left
        };
        int indices[] = { 0, 1, 2,  0, 2, 3 };
        al_draw_indexed_prim(quad, NULL, skin_texture, indices, 6, ALLEGRO_PRIM_TRIANGLE_LIST);
        //al_draw_polygon(verts, 4, ALLEGRO_LINE_JOIN_ROUND, al_map_rgb(c.r, c.g, c.b), 1.0, 0.0);
        std::cout<<x<<" "<<x1<<" "<<x2<<" "<<x3<<" "<<y<<" "<<y1<<" "<<y2<<" "<<y3;
}

void rotateble_rectangle::reset() {
        x = StartX;
        y = StartY;
}

void rotateble_rectangle::rotate() {
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