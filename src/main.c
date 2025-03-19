#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "constants.h"
#include "types.h"

#define CHECK_PTR_OR_RETURN(p) \
  if (!p)                      \
    return;

void init_game(Game* game)
{
  CHECK_PTR_OR_RETURN(game);

  Paddle p1 = {
    INITIAL_P1_PADDLE_X,
    INITIAL_P1_PADDLE_Y,
    PADDLE_HEIGHT,
    PADDLE_WIDTH,
    0,
    PS_STILL,
  };

  Paddle p2 = {
    INITIAL_P2_PADDLE_X,
    INITIAL_P2_PADDLE_Y,
    PADDLE_HEIGHT,
    PADDLE_WIDTH,
    0,
    PS_STILL,
  };

  Ball ball = {
    INITIAL_BALL_X_POSITION,
    INITIAL_BALL_Y_POSITION,
    INITIAL_BALL_HEIGHT,
    INITIAL_BALL_WIDTH,
    INITIAL_BALL_X_VELOCITY,
    INITIAL_BALL_Y_VELOCITY,
  };

  game->p1 = p1;
  game->p2 = p2;
  game->ball = ball;
  game->p1_score = 0;
  game->p2_score = 0;
  game->current_state = GS_MENU;
}

void reset_ball(Ball* ball)
{
  CHECK_PTR_OR_RETURN(ball);

  ball->x = INITIAL_BALL_X_POSITION;
  ball->y = INITIAL_BALL_Y_POSITION;
  ball->height = INITIAL_BALL_HEIGHT;
  ball->width = INITIAL_BALL_WIDTH;
  ball->x_velocity = INITIAL_BALL_X_VELOCITY;
  ball->y_velocity = INITIAL_BALL_Y_VELOCITY;
}

void update_paddle(Paddle* p)
{
  CHECK_PTR_OR_RETURN(p);
  switch (p->current_state)
  {
  case PS_MOVING_DOWN:
    p->y_velocity = PADDLE_VELOCITY;
    break;
  case PS_MOVING_UP:
    p->y_velocity = -PADDLE_VELOCITY;
    break;
  case PS_STILL:
    p->y_velocity = 0;
    break;
  }

  if (p->y_position + p->y_velocity + PADDLE_HEIGHT < SCREEN_HEIGHT && p->y_position + p->y_velocity > 0)
  {
    p->y_position += p->y_velocity;
  }
}

void update_game(Game* game)
{
  CHECK_PTR_OR_RETURN(game);
  switch (game->current_state)
  {
  case GS_MENU:
    break;
  case GS_MID_POINT:
  {
    update_paddle(&game->p1);
    update_paddle(&game->p2);

    // update ball
    if (game->ball.x == game->p1.x_position + game->p1.width && game->ball.y + game->ball.height >= game->p1.y_position && game->ball.y <= game->p1.y_position + game->p1.height)
    {
      game->ball.x_velocity = -game->ball.x_velocity;
    }

    if (game->ball.x + game->ball.width == game->p2.x_position && game->ball.y + game->ball.height >= game->p2.y_position && game->ball.y <= game->p2.y_position + game->p2.height)
    {
      game->ball.x_velocity = -game->ball.x_velocity;
    }

    if (game->ball.y <= 0 || game->ball.y + game->ball.height >= SCREEN_HEIGHT)
    {
      game->ball.y_velocity = -game->ball.y_velocity;
    }

    game->ball.x += game->ball.x_velocity;
    game->ball.y += game->ball.y_velocity;

    if (game->ball.x + game->ball.width == 0)
    {
      game->p2_score += 1;
      reset_ball(&game->ball);
    }
    else if (game->ball.x == SCREEN_WIDTH)
    {
      game->p1_score += 1;
      reset_ball(&game->ball);
    }
    break;
  }
  };
}

SDL_Texture* create_texture_from_string(SDL_Renderer* renderer, TTF_Font* font, const char* text, size_t text_length, SDL_Color color)
{
  SDL_Surface* text_surface = TTF_RenderText_Solid(font, text, text_length, color);
  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_DestroySurface(text_surface);

  return text_texture;
}

// change this so It will expand the width if I add more digits to the length, so like score_width = SCORE_DIGIT_WIDTH * digits in number
void render_score(SDL_Renderer* renderer, TTF_Font* font, uint8_t score, Point score_location, uint16_t score_width, uint16_t score_height)
{
  CHECK_PTR_OR_RETURN(renderer);
  CHECK_PTR_OR_RETURN(font);
  SDL_Color text_color = { 0xFF, 0xFF, 0xFF };
  char score_text[3];
  SDL_itoa(score, score_text, 10);
  SDL_Texture* score_texture = create_texture_from_string(renderer, font, score_text, 0, text_color);
  SDL_FRect score_rect = { score_location.x, score_location.y, score_width, score_height };
  SDL_RenderTexture(renderer, score_texture, NULL, &score_rect);
}

void render_menu(SDL_Renderer* renderer, TTF_Font* font)
{
  SDL_Color text_color = { 0xFF, 0xFF, 0xFF };
  SDL_Texture* pong_text_texture = create_texture_from_string(renderer, font, PONG_TITLE, CHARS_IN_PONG, text_color);
  SDL_FRect pong_text_rect = { GAME_TITLE_TEXT_X, GAME_TITLE_TEXT_Y, GAME_TITLE_CHAR_WIDTH * CHARS_IN_PONG, GAME_TITLE_CHAR_HEIGHT };
  SDL_RenderTexture(renderer, pong_text_texture, NULL, &pong_text_rect);

  SDL_Texture* enter_text_texture = create_texture_from_string(renderer, font, ENTER_TO_BEGIN_TEXT, ENTER_TO_BEGIN_TEXT_LENGTH, text_color);
  SDL_FRect enter_text_rect = { ENTER_TO_BEGIN_TEXT_X, ENTER_TO_BEGIN_TEXT_Y, ENTER_TO_BEGIN_TEXT_CHAR_WIDTH * ENTER_TO_BEGIN_TEXT_LENGTH, ENTER_TO_BEGIN_TEXT_CHAR_HEIGHT };
  SDL_RenderTexture(renderer, enter_text_texture, NULL, &enter_text_rect);
}

// its just ball texture for now but I should make a struct that holds ptrs to all the textures I need?
// Since I have few textures now, I could do an array of texture ptrs and i can index them with an enum
// in the future I might have sprites for paddles, the stadium, hit effects, buildings?
void render_game(Game* game, SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* ball_texture)
{
  CHECK_PTR_OR_RETURN(game);
  CHECK_PTR_OR_RETURN(renderer);
  CHECK_PTR_OR_RETURN(font);
  CHECK_PTR_OR_RETURN(ball_texture);

  SDL_FRect window_rect = {
    0,
    0,
    SCREEN_WIDTH,
    SCREEN_HEIGHT
  };

  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
  SDL_RenderFillRect(renderer, &window_rect);

  switch (game->current_state)
  {
  case GS_MENU:
    render_menu(renderer, font);
    break;
  case GS_MID_POINT:
  {
    render_score(renderer, font, game->p1_score, P1_SCORE_POINT, SCORE_DIGIT_WIDTH, SCORE_DIGIT_HEIGHT);
    render_score(renderer, font, game->p2_score, P2_SCORE_POINT, SCORE_DIGIT_WIDTH, SCORE_DIGIT_HEIGHT);

    SDL_FRect p1_rect = { game->p1.x_position, game->p1.y_position, game->p1.width, game->p1.height };
    SDL_FRect p2_rect = { game->p2.x_position, game->p2.y_position, game->p2.width, game->p2.height };

    SDL_FRect rects[] = { p1_rect, p2_rect };
    SDL_SetRenderDrawColor(renderer, 0xEE, 0xEE, 0xEE, 0xFF);
    SDL_RenderFillRects(renderer, rects, 2);

    SDL_FRect ball_rect = { game->ball.x, game->ball.y, game->ball.width, game->ball.height };
    SDL_RenderTexture(renderer, ball_texture, NULL, &ball_rect);

    break;
  }
  };

  SDL_RenderPresent(renderer);
}

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

  SDL_Texture* ball_texture = load_texture(BALL_PATH, renderer);
  if (!ball_texture)
  {
    SDL_Log("Failed to load ball texture\n");
    return 1;
  }

  screen_surface = SDL_GetWindowSurface(window);
  SDL_FillSurfaceRect(screen_surface, NULL, SDL_MapSurfaceRGB(screen_surface, 0x11, 0x11, 0x11));
  SDL_UpdateWindowSurface(window);

  bool frame_by_frame_mode = false;
  bool next_frame = false;

  Game* game = (Game*)malloc(sizeof(Game));
  init_game(game);

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
        case SDLK_T:
          frame_by_frame_mode = !frame_by_frame_mode;
          break;
        case SDLK_C:
          next_frame = true;
          break;
        case SDLK_W:
          game->p1.current_state = PS_MOVING_UP;
          break;
        case SDLK_S:
          game->p1.current_state = PS_MOVING_DOWN;
          break;
        case SDLK_I:
          game->p2.current_state = PS_MOVING_UP;
          break;
        case SDLK_K:
          game->p2.current_state = PS_MOVING_DOWN;
          break;
        case SDLK_R:
          reset_ball(&game->ball);
          break;
        case SDLK_RETURN:
          game->current_state = GS_MID_POINT;
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
          game->p1.current_state = PS_STILL;
          break;
        case SDLK_I:
        case SDLK_K:
          game->p2.current_state = PS_STILL;
          break;
        default:
          break;
        }
      }
    }

    if (frame_by_frame_mode && !next_frame)
    {
      continue;
    }

    update_game(game);

    // render
    render_game(game, renderer, font, ball_texture);

    next_frame = false;
  }

  // shutdown
  free(game);

  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
