#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdint.h>
#include <time.h>

const uint16_t SCREEN_WIDTH = 680;
const uint16_t SCREEN_HEIGHT = 480;
const uint16_t PADDLE_HEIGHT = 100;
const uint16_t PADDLE_WIDTH = 10;
const uint8_t PADDLE_VELOCITY = 2;

const uint16_t INITIAL_P1_PADDLE_X = 10;
const uint16_t INITIAL_P1_PADDLE_Y = SCREEN_HEIGHT / 2;
const uint16_t INITIAL_P2_PADDLE_X = SCREEN_WIDTH - 10 - PADDLE_WIDTH;
const uint16_t INITIAL_P2_PADDLE_Y = SCREEN_HEIGHT / 2;

const char* BALL_PATH = "images/pongball.png";

typedef struct
{
  uint16_t x_position;
  uint16_t y_position;
  uint16_t height;
  uint16_t width;
  int8_t vertical_velocity;
} Paddle;

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

int main(int argc, char* args[])
{
  SDL_Window* window = NULL;
  SDL_Surface* screen_surface = NULL;
  SDL_Renderer* renderer = NULL;

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("SDL couldn't initialize. SDL_Error: %s\n", SDL_GetError());
  }

  if (!SDL_CreateWindowAndRenderer("pong", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer))
  {
    SDL_Log("Can't create window. SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_SetRenderVSync(renderer, 1);

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

    // logic
    if (p1.y_position + p1.vertical_velocity + PADDLE_HEIGHT < SCREEN_HEIGHT && p1.y_position + p1.vertical_velocity > 0)
      p1.y_position += p1.vertical_velocity;

    if (p2.y_position + p2.vertical_velocity + PADDLE_HEIGHT < SCREEN_HEIGHT && p2.y_position + p2.vertical_velocity > 0)
      p2.y_position += p2.vertical_velocity;

    // render
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
    SDL_RenderFillRect(renderer, &window_rect);

    SDL_FRect p1_rect = { p1.x_position, p1.y_position, p1.width, p1.height };
    SDL_FRect p2_rect = { p2.x_position, p2.y_position, p2.width, p2.height };

    SDL_FRect rects[] = { p1_rect, p2_rect };

    SDL_SetRenderDrawColor(renderer, 0xEE, 0xEE, 0xEE, 0xFF);
    SDL_RenderFillRects(renderer, rects, 2);
    SDL_RenderFillRect(renderer, &p1_rect);
    SDL_RenderFillRect(renderer, &p2_rect);

    SDL_FRect ball_rect = { (float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 16, 16 };

    SDL_RenderTexture(renderer, ball_texture, NULL, &ball_rect);

    SDL_RenderPresent(renderer);
  }

  // shutdown
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
