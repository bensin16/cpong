#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

const int SCREEN_WIDTH = 680;
const int SCREEN_HEIGHT = 480;

void shutdown_sdl()
{
}

int main(int argc, char* args[])
{
  SDL_Window* window = NULL;
  SDL_Surface* screen_surface = NULL;

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("SDL couldn't initialize. SDL_Error: %s\n", SDL_GetError());
  }

  window = SDL_CreateWindow("pong", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!window)
  {
    SDL_Log("Can't create window. SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  screen_surface = SDL_GetWindowSurface(window);
  SDL_FillSurfaceRect(screen_surface, NULL, SDL_MapSurfaceRGB(screen_surface, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);

  // game loop

  bool quit = false;
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
        default:
          break;
        }
      }
    }
  }

  // shutdown
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
