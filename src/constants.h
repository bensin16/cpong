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

const uint8_t CHARS_IN_PONG = 4; // lol
const uint16_t GAME_TITLE_CHAR_WIDTH = 50;
const uint16_t GAME_TITLE_CHAR_HEIGHT = 70;
const uint16_t GAME_TITLE_TEXT_X = (SCREEN_WIDTH / 2) - (GAME_TITLE_CHAR_WIDTH * CHARS_IN_PONG / 2);
const uint16_t GAME_TITLE_TEXT_Y = SCREEN_HEIGHT / 3;

// better to calculate x when drawn and strlen this value so I can change the string and have this updated?
const uint8_t ENTER_TO_BEGIN_TEXT_LENGTH = 20;
const uint16_t ENTER_TO_BEGIN_TEXT_CHAR_WIDTH = 25;
const uint16_t ENTER_TO_BEGIN_TEXT_CHAR_HEIGHT = 35;
const uint16_t ENTER_TO_BEGIN_TEXT_X = (SCREEN_WIDTH / 2) - (ENTER_TO_BEGIN_TEXT_CHAR_WIDTH * ENTER_TO_BEGIN_TEXT_LENGTH / 2);
const uint16_t ENTER_TO_BEGIN_TEXT_Y = SCREEN_HEIGHT - SCREEN_HEIGHT / 3;

const char* PONG_TITLE = "Pong";
const char* ENTER_TO_BEGIN_TEXT = "Press Enter to start";

const char* BALL_PATH = "assets/pongball.png";
const char* FONT_PATH = "assets/8bitOperatorPlus8-Regular.ttf";
