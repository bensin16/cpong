#pragma once

#include <stdint.h>

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
  int8_t vertical_velocity;
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
