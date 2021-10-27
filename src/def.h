/*******************************************************************************************
*
*   Reversi AI
*
*   This game has been created using raylib 3.7 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2021 Derek Nguyen
*
*   DESCRIPTION:
*       Internally used key defines for game
*
********************************************************************************************/

#ifndef int8_t
typedef signed char int8_t;
#define int8_t int8_t
#define INT8_MIN (-128)
#define INT8_MAX (127)
#endif

#ifndef DEF_H
#define DEF_H

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define EMPTY_PIECE 0
#define PLAYER1_PIECE 1
#define PLAYER2_PIECE 2

#define BOARD_SIDE 8

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#endif //DEF_H
