#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "types.h"

SDL_Texture* load_texture(const char* path, SDL_Renderer* renderer)
{
  SDL_Surface* loaded_surface = IMG_Load(path);
  if (!loaded_surface)
  {
    SDL_Log("Unable to load image %s. SDL_image Error: %s\n", path, SDL_GetError());
    return NULL;
  }

  SDL_Texture* new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
  if (!new_texture)
  {
    SDL_Log("Unable to create texture from %s. SDL Error: %s\n", path, SDL_GetError());
  }

  SDL_DestroySurface(loaded_surface);
  return new_texture;
}

SDL_Texture* create_texture_from_string(SDL_Renderer* renderer, TTF_Font* font, const char* text, size_t text_length, SDL_Color color)
{
  SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, text_length, color);
  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_DestroySurface(text_surface);

  return text_texture;
}

int main(int argc, char* args[])
{
  SDL_Window* window = NULL;
  SDL_Surface* screen_surface = NULL;
  SDL_Renderer* renderer = NULL;

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("SDL couldn't initialize. SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  if (!TTF_Init())
  {
    SDL_Log("SDL_ttf couldn't initialize. SDL_ttf: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  if (!SDL_CreateWindowAndRenderer("pong", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer))
  {
    SDL_Log("Can't create window. SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_SetRenderVSync(renderer, 1);

  TTF_Font* font = TTF_OpenFont(FONT_PATH, 64);
  if (!font)
  {
    SDL_Log("SDL_ttf couldn't initialize. SDL_ttf: %s\n", SDL_GetError());
    return 1;
  }

  uint8_t p1_score = 0;

  SDL_Texture* ball_texture = load_texture(BALL_PATH, renderer);
  if (!ball_texture)
  {
    SDL_Log("Failed to load ball texture\n");
    return 1;
  }

  screen_surface = SDL_GetWindowSurface(window);
  SDL_FillSurfaceRect(screen_surface, NULL, SDL_MapSurfaceRGB(screen_surface, 0x11, 0x11, 0x11));
  SDL_UpdateWindowSurface(window);

  Paddle p1 = {
    INITIAL_P1_PADDLE_X,
    INITIAL_P1_PADDLE_Y,
    PADDLE_HEIGHT,
    PADDLE_WIDTH,
    0,
  };

  Paddle p2 = {
    INITIAL_P2_PADDLE_X,
    INITIAL_P2_PADDLE_Y,
    PADDLE_HEIGHT,
    PADDLE_WIDTH,
    0,
  };

  Ball ball = {
    INITIAL_BALL_X_POSITION,
    INITIAL_BALL_Y_POSITION,
    INITIAL_BALL_HEIGHT,
    INITIAL_BALL_WIDTH,
    INITIAL_BALL_X_VELOCITY,
    INITIAL_BALL_Y_VELOCITY,
  };

  SDL_FRect window_rect = {
    0,
    0,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
  };

  // game loop
  bool quit = false;
  // input
  while (!quit)
  {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_EVENT_QUIT)
      {
        quit = true;
      }
      else if (e.type == SDL_EVENT_KEY_DOWN)
      {
        switch (e.key.key)
        {
        case SDLK_ESCAPE:
          quit = true;
          break;
        case SDLK_W:
          p1.vertical_velocity = -PADDLE_VELOCITY;
          break;
        case SDLK_S:
          p1.vertical_velocity = PADDLE_VELOCITY;
          break;
        case SDLK_I:
          p2.vertical_velocity = -PADDLE_VELOCITY;
          break;
        case SDLK_K:
          p2.vertical_velocity = PADDLE_VELOCITY;
          break;
        case SDLK_R:
          ball.x = INITIAL_BALL_X_POSITION;
          ball.y = INITIAL_BALL_Y_POSITION;
          ball.height = INITIAL_BALL_HEIGHT;
          ball.width = INITIAL_BALL_WIDTH;
          ball.x_velocity = INITIAL_BALL_X_VELOCITY;
          ball.y_velocity = INITIAL_BALL_Y_VELOCITY;
          break;
        default:
          break;
        }
      }
      else if (e.type == SDL_EVENT_KEY_UP)
      {
        switch (e.key.key)
        {
        case SDLK_W:
        case SDLK_S:
          p1.vertical_velocity = 0;
          break;
        case SDLK_I:
        case SDLK_K:
          p2.vertical_velocity = 0;
          break;
        default:
          break;
        }
      }
    }

    if (ball.x == p1.x_position + p1.width && ball.y >= p1.y_position && ball.y <= p1.y_position + p1.height)
    {
      ball.x_velocity = -ball.x_velocity;
    }

    if (ball.x + ball.width == p2.x_position && ball.y >= p2.y_position && ball.y <= p2.y_position + p2.height)
    {
      ball.x_velocity = -ball.x_velocity;
    }

    if (ball.y <= 0 || ball.y + ball.height >= SCREEN_HEIGHT)
    {
      ball.y_velocity = -ball.y_velocity;
    }

    // logic
    if (p1.y_position + p1.vertical_velocity + PADDLE_HEIGHT < SCREEN_HEIGHT && p1.y_position + p1.vertical_velocity > 0)
      p1.y_position += p1.vertical_velocity;

    if (p2.y_position + p2.vertical_velocity + PADDLE_HEIGHT < SCREEN_HEIGHT && p2.y_position + p2.vertical_velocity > 0)
      p2.y_position += p2.vertical_velocity;

    ball.x += ball.x_velocity;
    ball.y += ball.y_velocity;

    // render
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
    SDL_RenderFillRect(renderer, &window_rect);

    SDL_Color text_color = { 0xFF, 0xFF, 0xFF };
    char p1_score_text[3];
    SDL_itoa(p1_score, p1_score_text, 10);
    SDL_Texture* text_texture = create_texture_from_string(renderer, font, p1_score_text, 0, text_color);
    SDL_FRect p1_score_rect = { P1_SCORE_POINT.x, P1_SCORE_POINT.y, SCORE_DIGIT_WIDTH, SCORE_DIGIT_HEIGHT };
    SDL_RenderTexture(renderer, text_texture, NULL, &p1_score_rect);

    // SDL_FRect textRect = { 50, 50, 400, 100 };
    // SDL_RenderTexture(renderer, text_texture, NULL, &textRect);

    SDL_FRect p1_rect = { p1.x_position, p1.y_position, p1.width, p1.height };
    SDL_FRect p2_rect = { p2.x_position, p2.y_position, p2.width, p2.height };

    SDL_FRect rects[] = { p1_rect, p2_rect };
    SDL_SetRenderDrawColor(renderer, 0xEE, 0xEE, 0xEE, 0xFF);
    SDL_RenderFillRects(renderer, rects, 2);

    SDL_FRect ball_rect = { ball.x, ball.y, ball.width, ball.height };
    SDL_RenderTexture(renderer, ball_texture, NULL, &ball_rect);

    SDL_RenderPresent(renderer);
  }

  // shutdown
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
