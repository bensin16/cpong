#pragma once

#include <stdint.h>

#include "types.h"

const int16_t SCREEN_WIDTH = 680;
const int16_t SCREEN_HEIGHT = 480;
const uint16_t PADDLE_HEIGHT = 100;
const uint16_t PADDLE_WIDTH = 10;
const uint8_t PADDLE_VELOCITY = 2;

const uint16_t INITIAL_P1_PADDLE_X = 10;
const uint16_t INITIAL_P1_PADDLE_Y = SCREEN_HEIGHT / 2;
const uint16_t INITIAL_P2_PADDLE_X = SCREEN_WIDTH - 10 - PADDLE_WIDTH;
const uint16_t INITIAL_P2_PADDLE_Y = SCREEN_HEIGHT / 2;

const int16_t INITIAL_BALL_X_POSITION = SCREEN_WIDTH / 2;
const int16_t INITIAL_BALL_Y_POSITION = SCREEN_HEIGHT / 2;
const uint16_t INITIAL_BALL_HEIGHT = 16;
const uint16_t INITIAL_BALL_WIDTH = 16;
const int8_t INITIAL_BALL_X_VELOCITY = -2;
const int8_t INITIAL_BALL_Y_VELOCITY = -1;

const uint16_t SCORE_DIGIT_WIDTH = 50;
const uint16_t SCORE_DIGIT_HEIGHT = 70;
const Point P1_SCORE_POINT = { SCREEN_WIDTH / 5, 10 };
const Point P2_SCORE_POINT = { SCREEN_WIDTH - (SCREEN_WIDTH / 5) - SCORE_DIGIT_WIDTH, 10 };

const char* BALL_PATH = "assets/pongball.png";
const char* FONT_PATH = "assets/8bitOperatorPlus8-Regular.ttf";
