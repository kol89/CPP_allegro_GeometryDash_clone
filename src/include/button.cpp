#include "include.hpp"

bool button::is_pressed() {
        ALLEGRO_MOUSE_STATE state;
        al_get_mouse_state(&state);
        return state.x >= x & state.y >= y & state.x <= x + sx & state.y <= y + sy & al_mouse_button_down(&state, ALLEGRO_MOUSE_BUTTON_LEFT);
    }

button::button(double _x, double _y, double _sx, double _sy, char const* _text, colour _c) {
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

void button::brend() {
        rend();
        al_draw_text(
            font,
            al_map_rgb(0, 0, 0),
            (x + sx / 2), (y + sy / 2)-20,
            ALLEGRO_ALIGN_CENTER,
            text
        );
    }