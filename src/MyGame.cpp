#include "engine.hpp"
#include <allegro5/allegro_primitives.h>
#include <cstddef>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <vector>
#include <charconv>
const double Pi = 3.14;
ALLEGRO_FONT* font = nullptr;
int attempts = 0;
int frame = 0;
int stat = 0;
int lvl = 1;
const double g = 900;
std::string mod;
bool on_ground = false;
bool press = false;

struct colour{
    int r; // red channel
    int g; // green channel
    int b; // blue channel
    int a; // alpha channel
    colour(int _r, int _g, int _b, int _a){
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
    colour(){
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }
};

class shape/*dot*/{
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

class rectangle : public shape{
    public:
        double sx; // size x
        double sy; // size y
        rectangle(double _x, double _y, double _vx, double _vy, double _sx, double _sy, double _ax, double _ay, colour _c){
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
        rectangle(){
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
        void move(){
            x += vx/60;
            y += vy/60;
        }
        void accelerate(){
            vx += ax/60;
            vy += ay/60;
        }
        void rend(){
            al_draw_filled_rectangle(x, y, x + sx, y + sy, al_map_rgb(c.r, c.g, c.b));
        }
        void reset(){
            x = StartX;
            y = StartY;
        }
        bool chkcol(rectangle a){
            return a.x < x + sx && a.x + a.sx > x && a.y < y + sy && a.y + a.sy > y;
        }
};

class player: public rectangle{
    public:
        char mode;
        bool flip_gr;
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
        player(double _x,double _y,char _mode, int _speed){
            mode=_mode;
            flip_gr = false;
            x= _x;
            y= _y;
            StartX = _x;
            StartY = _y;
            vx = 0;
            vy = 0;
            chsp(_speed);
            sx = 50;
            sy = 50;
            ax = 0;
            ay = 0;
            c = colour(0,0,0,255);
            hit_box = rectangle(x,y,vx,vy,sx,sy,ax,ay,colour(255,0,0,0));
            collision_box = rectangle(x+(sx/3),y+(sy/3),vx,vy,sx/3,sy/3,ax,ay,colour(0,0,255,0));
        }
        void sync() {
            hit_box = rectangle(x,y,vx,vy,sx,sy,ax,ay,colour(255,0,0,0));
            collision_box = rectangle(x+(sx/3),y+(sy/3),vx,vy,sx/3,sy/3,ax,ay,colour(0,0,255,0));
        }
        void pmove() {
            x += vx/60;
            y += vy/60;
            sync();
        }
        void paccelerate(){
            vx += ax/60;
            if (flip_gr){vy -= ay/60;}else{vy += ay/60;}
            sync();
        }
        

};
player p = player(0, 300, 'c', 1);

rectangle ground(0,300,0,0,700,60,0,0,colour(255,165,0,0));
class block: public rectangle{
    public:
        block(double _x, double _y){
            ax = 0;
            ay = 0;
            vx = 0;
            vy = 0;
            sx = 50;
            sy = 50;
            x = _x*50;
            y = ground.y-_y*50;
            StartX = x;
            StartY = y;
            c = colour(0,0,255,0);
        }
};

class spike: public rectangle{
    public:
        spike(double _x, double _y){
            ax = 0;
            ay = 0;
            vx = 0;
            vy = 0;
            sx = 7.14;
            sy = 20.71;
            x = _x*50+20;
            y = ground.y-_y*50+14.29;
            StartX = x;
            StartY = y;
            c = colour(255,0,0,0);
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

class button: public rectangle{
    public:
        char const *text;
        bool is_pressed(){
            ALLEGRO_MOUSE_STATE state;
            al_get_mouse_state(&state);
            return state.x >= x & state.y >= y & state.x <= x+sx & state.y <= y+sy & al_mouse_button_down(&state, ALLEGRO_MOUSE_BUTTON_LEFT);
        }
        button(double _x, double _y, double _sx, double _sy, char const *_text, colour _c){
            x = _x;
            y = _y;
            sx = _sx;
            sy = _sy;
            text = _text;
            vx = 0;
            vy = 0;
            ax=0;
            ay=0;
            c = _c;
        }
        void brend(){
            rend();
            al_draw_text(
                font,
                al_map_rgb(0, 0, 0),
                (x+sx/2)-45, (y+sy/2)-20,
                ALLEGRO_ALIGN_LEFT,
                text
            );
        }
};

class level{
    public:
    std::vector<spike> danger;
    std::vector<block> colidable;
	std::vector<portal> portals;
	std::vector<speed_portal> speed;
    char mode;
    int sp;
    double end;
    level(std::vector<spike> _danger, std::vector<block> _colidable, std::vector<portal> _portals, std::vector<speed_portal> _speed, double _end, char _mode, int _sp){
        danger = _danger;
        colidable = _colidable;
        portals = _portals;
        speed = _speed;
        end = _end;
        mode = _mode;
        sp = _sp;
    }
    void render(){
        if (danger.empty() == 0){
            for (int i = 0; i<danger.size(); i++){
                danger[i].rend();
            }
        }
        if (colidable.empty() == 0){
            for (int i = 0; i<colidable.size(); i++){
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
    }
    bool collision(){
        if (danger.empty() == 0){
            for (int i = 0; i<danger.size(); i++){
                        if (p.hit_box.chkcol(danger[i])){
                            return true;
                        }
            }
        }

        if (colidable.empty() == 0){
            for (int i = 0; i<colidable.size(); i++){
                if (p.hit_box.chkcol(colidable[i])){
                    if(p.flip_gr){
                        if (p.collision_box.y+p.collision_box.sy>colidable[i].y){
                            p.y = colidable[i].y+p.hit_box.sy;
                            p.vy = 0;
                            on_ground = true;
                        }
                    }else{
                        if (p.collision_box.y+p.collision_box.sy<colidable[i].y){
                            p.y = colidable[i].y-p.hit_box.sy;
                            p.vy = 0;
                            on_ground = true;
                        }
                    }
                    if (p.collision_box.chkcol(colidable[i])){
                        return true;
                    }
                }
            }
        }

		if (portals.empty() == 0) {
			for (int i = 0; i < portals.size(); i++) {
				if (p.hit_box.chkcol(portals[i])) {
                    p.mode = portals[i].mode;
				}
			}
		}
	    
        if (portals.empty() == 0) {
			for (int i = 0; i < speed.size(); i++) {
				if (p.hit_box.chkcol(speed[i])) {
					p.vx = speed[i].spd;
				}
			}
        }

        return false;
    }
    void move(){
        if (p.x>=150){
            p.x = p.x - p.vx/60;
            if (danger.empty() == 0){
                for (int i = 0; i<danger.size(); i++){
                    danger[i].x = danger[i].x - p.vx/60;
                }
            }
            if (colidable.empty() == 0){
                for (int i = 0; i<colidable.size(); i++){
                    colidable[i].x = colidable[i].x - p.vx/60;
                }
            }
			if (portals.empty() == 0) {
				for (int i = 0; i < portals.size(); i++) {
                    portals[i].x = portals[i].x - p.vx / 60;
				}
			}
			if (speed.empty() == 0) {
				for (int i = 0; i < speed.size(); i++) {
                    speed[i].x = speed[i].x - p.vx / 60;
				}
			}
        }   
    }
};





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
    block (30, 1),

    block (34, 1),
    block (34, 2),

    block (38, 1),
    block (38, 2),
    block (38, 3),

    block (55, 1),
    block (56, 1),
    block (57, 1),
    block (58, 1),
    block (59, 1),
    block (60, 1),

    block (63, 1),
    block (64, 1),
    block (65, 1),
    block (66, 1),
    block (63, 2),
    block (64, 2),
    block (65, 2),
    block (66, 2),
    }, 
	portals = {},
	speed = {},
    1000,
    'c',
    1
);
level level2(
    danger = {
    spike(10, 1),
    spike(11, 1),
    spike(12, 1),
    }, 
    colidable = {

    },
    portals = {},
    speed = {},
    1000,
	'c',
	1
);

void reset_all(){
    p.reset();
    font = nullptr;
    switch (lvl){
        case 1:
            for (int i = 0; i<level1.danger.size(); i++){level1.danger[i].reset();}
            for (int i = 0; i<level1.colidable.size(); i++){level1.colidable[i].reset();}
            p.chsp(level1.sp);
            p.mode = level1.mode;
        case 2:
            for (int i = 0; i<level2.danger.size(); i++){level2.danger[i].reset();}
            for (int i = 0; i<level2.colidable.size(); i++){level2.colidable[i].reset();}
			p.chsp(level2.sp);
			p.mode = level2.mode;
    }
    attempts++;
}

button l1 (100,120,170,100, "level 1", colour(255,255,255,0));
button l2 (350,120,170,100, "level 2", colour(255,255,255,0));



//~----------------------Main program------------------------
class GameEngine:public Engine{

    //*---FPS---+
    virtual void physics_process() override {
        if (stat==0){
            if (attempts!=0){
                attempts=0;
            }
            if (l1.is_pressed()){
                font = nullptr;
                lvl=1;
                stat=1;
            }
            if (l2.is_pressed()){
                font = nullptr;
                lvl=2;
                stat=1;
            }
        }
        if (stat==1){
            if(frame == 60){
                frame=0;
            }
            std::cout << frame << std::endl;
            ALLEGRO_KEYBOARD_STATE state;
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
            if (p.vy < -1000){
                reset_all();
            }
			switch (lvl) {
			case 1:
                p.mode = level1.mode;
                p.chsp(level1.sp);
				level1.move();
				if (level1.collision()) {
					reset_all();
				}
				break;

			case 2:
				p.mode = level2.mode;
                p.chsp(level2.sp);
				level2.move();
				if (level2.collision()) {
					reset_all();
				}
				break;
			}

            if (al_key_down(&state, ALLEGRO_KEY_UP) || al_key_down(&state, ALLEGRO_KEY_SPACE)) {
                switch (p.mode) {
                case 's':
                    p.ay = -700;
                    break;
                case 'c':
                    if (on_ground) { p.vy = -410; }
                    break;
                case 'b':
                    if (on_ground && !press) { p.flip_gr = !p.flip_gr; press = true; }
                    break;
                }
            }else{ press = false; }

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
            p.paccelerate();
            p.pmove();

            frame++;
        }

    }

    //*---draw---+
    virtual void render_process() override {
        if (stat==0){
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
        if (stat==1){
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
            if (p.flip_gr){
                mod = "gravity: flipped";
            }else{
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
            p.hit_box.rend();
            p.collision_box.rend();
            ground.rend();
            if (lvl == 1){
                level1.render();
            }
            if (lvl == 2){
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
