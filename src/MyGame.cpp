#include "include/block.hpp"
#include "include/engine.hpp"
#include "include/gvar.hpp"
#include "include/include.hpp"
#include <allegro5/mouse.h>
#include <cstdio>

class sblock : public block{
    public:
        double shiftx;
        double shifty;
        double visshx;
        double visshy;
        sblock( double _x, double _y, double _shiftx, double _shifty) : block(_x, _y) {
            shiftx = _shiftx;
            shifty = _shifty;
        }
        void shift(){
            x-=shiftx*grid_size;
            y-=shifty*grid_size;
        }
        void ren() {
            al_draw_filled_rectangle(x+visshx, y+visshy, x + sx+visshx, y + sy+visshy, al_map_rgb(c.r, c.g, c.b));
        }
    };     

std::vector<sblock> col;
std::vector<block> col2;

player p = player(0, 300, 'c', 1);

std::vector<spike> danger;
std::vector<block> colidable;
std::vector<portal> portals;
/*
    all modes:
    c - cube    DONE
    s - ship    DONE
    b - saw     DONE
    u - UFO     DONE      
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
    g - green    DONE
    k - black    DONE
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
    spike(66, 3),

    spike(71, 1),
    spike(72, 1),
    spike(73, 1),
    spike(74, 1),
    spike(75, 1),
    spike(76, 1),
    spike(77, 1),
    spike(78, 1),
    spike(79, 1),


    spike(86, 1),
    spike(87, 1),
    spike(88, 1),
    spike(89, 1),
    spike(90, 1),
    spike(91, 1),
    spike(92, 1),
    spike(93, 1),
    spike(94, 1),

    spike(82, 6),
    spike(83, 6),
    spike(84, 6),
    spike(85, 6),
    spike(86, 6),
    spike(87, 6),
    spike(88, 6),
    spike(89,6),
    spike(90, 6),
    spike(91, 6),
    spike(92, 6),
    spike(93, 6),
    spike(94, 6),
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

    block(67, 1),
    block(68, 1),
    block(69, 1),
    block(70, 1),
    block(63, 2),
    block(64, 2),
    block(65, 2),
    block(66, 2),
    block(67, 2),
    block(68, 2),
    block(69, 2),
    block(70, 2),

    block(74,3),
    block(78,4),
    block(100, 1),
    block(101, 1),
    block(102, 1),
    block(103, 1),
    block(104, 1),
    block(105, 1),
    block(106, 1),
    block(100, 2),
    block(101, 2),
    block(102, 2),
    block(103, 2),
    block(104, 2),
    block(105, 2),
    block(106, 2),
    block(104, 3),
    block(105, 3),
    block(106, 3),


    },
    portals = {
        portal(83, 3, 's'),
        portal(95, 4, 'c')
    },
    speed = {},
    orbs = {
        orb(109,2,'y')
    },
    pads = {
        pad(93.2, 3,'p')
    },
    1000,
    'c',
    1
    );
// level level2(
//     danger = {
//     spike(10, 1),
//     spike(11, 1),
//     spike(12, 1)
//     },
//     colidable = {
//         block(9, 1),
//         block(9, 2)
//     },
//     portals = {},
//     speed = {},
//     orbs = {
//     orb(13, 3, 'g'),
//     orb(18, 5, 'g'),
//     orb(30, 2.5, 'y'),
//     orb(40, 2.5, 'p'),
//     orb(50, 2.5, 'r'),
//     orb(65, 2.5, 'k'),
//     orb(75, 2.5, 'b'),
//     orb(77, 5, 'b')
//     },
//     pads = {
//         pad(85,1,'p'),
//         pad(90,1,'y'),
//         pad(100,1,'r'),
//         pad(110,1,'b'),
//     },
//     1000,
//     'c',
//     1
//     );

level level2 = load_level("src/levels/1.json");

level ulevel(
    danger = {spike(5,1)}, colidable = {block(6,1)}, portals = {}, speed = {}, orbs = {}, pads = {}, 0, 'c', 1
);

void reset_all() {
    p.reset();
    font = nullptr;
    ground.y=gr;
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
        break;
    case 2:
        for (int i = 0; i < level2.danger.size(); i++) { level2.danger[i].reset(); }
        for (int i = 0; i < level2.colidable.size(); i++) { level2.colidable[i].reset(); }
        for (int i = 0; i < level2.portals.size(); i++) { level2.portals[i].reset(); }
        for (int i = 0; i < level2.speed.size(); i++) { level2.speed[i].reset(); }
        for (int i = 0; i < level2.orbs.size(); i++) { level2.orbs[i].reset(); }
        for (int i = 0; i < level2.pads.size(); i++) { level2.pads[i].reset(); }
        p.chsp(level2.sp);
        p.mode = level2.mode;
        break;
    case -1:
        for (int i = 0; i < ulevel.danger.size(); i++) { ulevel.danger[i].reset(); }
        for (int i = 0; i < ulevel.colidable.size(); i++) { ulevel.colidable[i].reset(); }
        for (int i = 0; i < ulevel.portals.size(); i++) { ulevel.portals[i].reset(); }
        for (int i = 0; i < ulevel.speed.size(); i++) { ulevel.speed[i].reset(); }
        for (int i = 0; i < ulevel.orbs.size(); i++) { ulevel.orbs[i].reset(); }
        for (int i = 0; i < ulevel.pads.size(); i++) { ulevel.pads[i].reset(); }
        p.chsp(ulevel.sp);
        p.mode = ulevel.mode;
        break;
    }
    attempts++;
}

button l1(100, 120, 75, 75, "<-", colour(255, 255, 255, 0));
button l2(490, 120, 75, 75, "->", colour(255, 255, 255, 0));
button enter(250, 200, 150, 100, "ENTER", colour(255, 255, 255, 0));
button creator(250, 370, 150, 100, "CREATE", colour(255, 255, 255, 0));



//~----------------------Main program------------------------
class GameEngine :public Engine {

    //*---FPS---+
    virtual void physics_process() override {
        if (stat == 0) {
            load_level("src/levels/1.json");
            if (attempts != 0) {
                attempts = 0;
            }
            if (l1.is_pressed() && !l1.pressed) {
                lvl--;
                l1.pressed=true;
            }
            if(!l1.is_pressed()){
                l1.pressed=false;
            }
            if (l2.is_pressed() && !l2.pressed) {
                lvl++;
                l2.pressed=true;
            }
            if(!l2.is_pressed()){
                l2.pressed=false;
            }
            if(enter.is_pressed()){

                font=nullptr;
                stat=1;
            }

            if(creator.is_pressed()){
                font=nullptr;
                lvl=-1;
                stat=2;
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
            if (p.vy < -1000/50*grid_size) {
                reset_all();
            }
            
            
            switch (lvl) {
            case 1:
                //p.mode = level1.mode;
                p.chsp(level1.sp);
                if (p.x > 150) {
                    p.x = p.x - p.vx / fps;
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
                    p.x = p.x - p.vx / fps;
                    level2.move(p.vx);
                }
                if (p.collision(level2)) {
                    reset_all();
                }
                break;
            case -1:
                p.mode = ulevel.mode;
                p.chsp(ulevel.sp);
                if (p.x > 150) {
                    p.x = p.x - p.vx / fps;
                    ulevel.move(p.vx);
                }
                if (p.collision(ulevel)) {
                    reset_all();
                }
                break;
            }
            //-flight logic-
            if(p.mode=='s'){
                g=-flightAcceleration/1.2;
                if(p.vy>-flightAcceleration/1.2){
                    p.vy=1500;
                }
                if(p.vy<flightAcceleration/1.2){
                    p.vy=-1500;
                }
            }else{
                g=1500;
            }

            
            //-------
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
                case 'u':
                    if (!press) { p.vy = jumpSpeed; press = true; }
                    break;
                case 'w':
                    g = 0;
                    p.vy = jumpSpeed;
                    break;
                }
            } 
            else { press = false; }
            if (p.mode == 'w' && !(al_key_down(&state, ALLEGRO_KEY_UP) || al_key_down(&state, ALLEGRO_KEY_SPACE))){
                p.vy = -jumpSpeed;
            }
            if(!(p.mode!='w')){
                g=1500;
            }
            p.skin.x1+=grid_size;
            p.skin.x3+=grid_size;
            p.skin.x2+=grid_size;
            p.skin.x+=grid_size;
            p.skin.y1+=grid_size;
            p.skin.y3+=grid_size;
            p.skin.y2+=grid_size;
            p.skin.y+=grid_size;
            
            if (al_key_down(&state, ALLEGRO_KEY_S)) {
                p.skin.x+=grid_size;
                p.skin.x3+=grid_size;
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
            if (al_key_down(&state, ALLEGRO_KEY_W)) {
                switch (lvl) {
                case 1:
                    level1.mode = 'w';
                case 2:
                    level2.mode = 'w';
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
            switch(p.mode){
                case 'c':
                if (!on_ground) {
                    p.skin.grad += 3.9*local_game_speed; //3.8
                }
                break;
                case 's':
                if (!on_ground) {
                    p.skin.grad = p.vy/16; //3.8
                }
                break;
            }
            
            if (on_ground) {
                if(p.skin.grad>=315 && p.skin.grad<=360 || p.skin.grad<=45) {
                    p.skin.grad = 0;
                }
                if(p.skin.grad>45 && p.skin.grad<=135) {
                    p.skin.grad = 90;
                }
                if(p.skin.grad>135 && p.skin.grad<=225) {
                    p.skin.grad = 180;
                }
                if(p.skin.grad>225 && p.skin.grad<315) {
                    p.skin.grad = 270;
                }
            }
            std::cout<<p.skin.grad<<std::endl;
            if(p.skin.grad>=360){
                p.skin.grad=0;
            }
            p.skin.rotate();
            p.paccelerate();
            p.pmove();

            frame++;
        }
        if (stat == 2) {

            //reset_all();
            al_get_mouse_state(&statem);
            al_get_keyboard_state(&state);
            int msx = mx-statem.x;
            int msy = my-statem.y;
            if (al_key_down(&state, ALLEGRO_KEY_ESCAPE)) {
                for (int i =0; i<col.size(); i++){
                    col[i].shift();
                    ulevel.colidable.push_back(block(col[i].x/grid_size,col[i].y/grid_size));
                }
                std::cout<<"-----------------------"<<ulevel.colidable.size()<<std::endl;
                ground.y=gr;
                reset_all();
                stat = 0;
                return;
            }

            if (al_key_down(&state, ALLEGRO_KEY_SPACE)) {
                reset_all();
            }

            if (al_mouse_button_down(&statem, ALLEGRO_MOUSE_BUTTON_LEFT)){
                for (int i =0; i<col.size(); i++){
                    col[i].visshx-=msx;
                    col[i].visshy-=msy;
                }
                ulevel.shift(msx*fps,msy*fps);
                ground.y-=msy;
                pointer_shiftx-=msx;
                pointer_shifty-=msy;
            }
            if (al_mouse_button_down(&statem, ALLEGRO_MOUSE_BUTTON_RIGHT)){
                //ulevel.colidable.push_back(block(pointer_gx, pointer_gy));
                col.push_back(sblock(pointer_gx, pointer_gy, ulevel.level_shiftx, ulevel.level_shifty));
            }
            pointer_gx=((ulevel.level_shiftx+pointer_shiftx+statem.x)/grid_size);
            pointer_gy=((ulevel.level_shifty+pointer_shifty+ground.y-statem.y)/grid_size);
            mx = statem.x;
            my = statem.y;
            ulevel.colidable[0].x=ulevel.colidable[0].StartX+pointer_gx*grid_size;
            ulevel.colidable[0].y=ulevel.colidable[0].StartY-pointer_gy*grid_size;
            std::cout <<"pointer_gx: "<< pointer_gx << " " <<"pointer_gy: "<< pointer_gy << std::endl;
            std::cout <<"pointer_shiftx: "<< pointer_shiftx << " " <<"pointer_shifty: "<< pointer_shifty << std::endl;
            std::cout <<"level_shiftx: "<< ulevel.level_shiftx << " " <<"level_shifty: "<< ulevel.level_shifty << std::endl;
        }

    }

    //*---draw---+
    virtual void render_process() override {
        if (stat == 0) {
            if (font == nullptr) {
                font = al_load_ttf_font(
                    "src/font/pusab.otf", // font file
                    32,         // font size
                    0           // flags (just use zero)
                );
            }
            if (skin_texture == nullptr) {
                al_init_image_addon();
                skin_texture = al_load_bitmap("src/img/skin.png");
            }
            
            al_clear_to_color(al_map_rgb(40, 125, 255));
            /*if (skin_texture != nullptr) {
                al_draw_bitmap(skin_texture, 0, 0, 2);
            }*/
            al_draw_text(
                font,
                al_map_rgb(255, 255, 255),
                150, 50,
                ALLEGRO_ALIGN_LEFT,
                "Geography Jump -alpha-"
            );
            al_draw_text(
                font,
                al_map_rgb(255, 255, 255),
                295, 120,
                ALLEGRO_ALIGN_LEFT,
                std::to_string(lvl).c_str()
            );
            l1.brend();
            l2.brend();
            enter.brend();
            creator.brend();
        }
        if (stat == 1) {
            if (font == nullptr) {
                font = al_load_ttf_font(
                    "src/font/pusab.otf", // font file
                    16,         // font size
                    0           // flags (just use zero)
                );
            }

            switch (p.mode) {
            case 'c':
                skin_texture=nullptr;
                player_sk = "src/img/skin.png";
                mod = "mode: cube";
                break;
            case 's':
                skin_texture=nullptr;
                player_sk = "src/img/shipskin.png";
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

            al_clear_to_color(al_map_rgb(40, 125, 255));
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
            if (lvl == -1) {
                ulevel.render();
            }
        }
        if (stat == 2) {
            if (font == nullptr) {
                font = al_load_ttf_font(
                    "src/font/pusab.otf", // font file
                    16,         // font size
                    0           // flags (just use zero)
                );
            }
            al_clear_to_color(al_map_rgb(40, 125, 255));
            for (int i =0; i<col.size(); i++){
                col[i].ren();
            }
            block(pointer_gx, pointer_gy).rend();
            ground.rend();
            ulevel.render();

        }
    }
};
//~--------------------------------------------------------------


int main() {
    GameEngine game;
    game.start();
    return 0;
}
