#ifndef _GAME_H_
#define _GAME_H_

#include "touch.h"
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

void Game();

#endif