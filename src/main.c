#include "SDL3/SDL_render.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdint.h>
#include <time.h>

const uint16_t SCREEN_WIDTH = 680;
const uint16_t SCREEN_HEIGHT = 480;

const uint8_t PADDLE_VELOCITY = 2;

typedef struct
{
  uint16_t x_position;
  uint16_t y_position;
  uint16_t height;
  uint16_t width;
  int8_t vertical_velocity;
} Paddle;

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

  screen_surface = SDL_GetWindowSurface(window);
  SDL_FillSurfaceRect(screen_surface, NULL, SDL_MapSurfaceRGB(screen_surface, 0x11, 0x11, 0x11));
  SDL_UpdateWindowSurface(window);

  Paddle p = {
    (uint16_t)10,
    (uint16_t)SCREEN_HEIGHT / 2,
    100,
    10,
    0,
  };

  SDL_FRect windowRect = {
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
          p.vertical_velocity = -PADDLE_VELOCITY;
          break;
        case SDLK_S:
          p.vertical_velocity = PADDLE_VELOCITY;
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
          p.vertical_velocity = 0;
          break;
        default:
          break;
        }
      }
    }

    // logic
    p.y_position += p.vertical_velocity;
    SDL_Log("%d\n", p.y_position);

    // render
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
    SDL_RenderFillRect(renderer, &windowRect);

    SDL_FRect paddleRect = { p.x_position, p.y_position, p.width, p.height };

    SDL_SetRenderDrawColor(renderer, 0xEE, 0xEE, 0xEE, 0xFF);
    SDL_RenderFillRect(renderer, &paddleRect);
    SDL_RenderPresent(renderer);
  }

  // shutdown
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
