#ifndef COLOUR_H
#define COLOUR_H

struct colour {
    int r; // red channel
    int g; // green channel
    int b; // blue channel
    int a; // alpha channel
    colour(int _r, int _g, int _b, int _a);
    colour();
};

#endif