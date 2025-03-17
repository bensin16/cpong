#pragma once

#include <stdint.h>

typedef enum
{
  PS_STILL,
  PS_MOVING_UP,
  PS_MOVING_DOWN,
} PaddleState;

typedef enum
{
  GS_MENU,
  GS_MID_POINT,
} GameState;

typedef struct
{
  uint16_t x;
  uint16_t y;
} Point;

typedef struct
{
  uint16_t x_position;
  uint16_t y_position;
  uint16_t height;
  uint16_t width;
  int8_t y_velocity;
  PaddleState current_state;
} Paddle;

typedef struct
{
  int16_t x;
  int16_t y;
  uint16_t height;
  uint16_t width;
  int8_t x_velocity;
  int8_t y_velocity;
} Ball;

typedef struct
{
  Paddle p1;
  Paddle p2;
  Ball ball;
  uint8_t p1_score;
  uint8_t p2_score;
  GameState current_state;
} Game;
