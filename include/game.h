#ifndef __GAME_H__
#define __GAME_H__

#include "header.h"
#include <stdbool.h>

struct Ball
{
    int x;
    int y;
    int radius;

    int x_speed;
    int y_speed;
} typedef Ball;

struct Rect
{
    int x;
    int y;
    int width;
    int height;

    int speed;
} typedef Rect;

bool Game();

#endif