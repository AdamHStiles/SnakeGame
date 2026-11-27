#include "raylib.h"
#include <sstream>
#include <stdexcept>

#include "snake.h"
#include "apple.h"

#define NUM_CELLS 20
#define CELL_SIZE 30
#define HEADER_SIZE 40

enum class game_state {
  START = 0,
  GAME, 
  DEAD
};

void draw_header(int score, game_state state) {
  switch (state) {
    case game_state::START: {
      const char *text = "Press any key to start";
      DrawText(text, 10, 5, 30, BLACK);
      break;
    }
    case game_state::GAME: {
      std::stringstream score_text("");
      score_text << "Score: " << score;
      DrawText(score_text.str().c_str(), 10, 5, 30, BLACK);
      break;
    }
    case game_state::DEAD: {
      const char *text = "Game Over!";
      DrawText(text, 10, 5, 30, RED);
      break;
    }
    default:
      throw std::runtime_error("invalid game state");
  }
}

void draw_grid() {
  for(std::size_t row {}; row < NUM_CELLS; ++row) {
    for(std::size_t col{}; col < NUM_CELLS; ++col) {
      int startX = row*CELL_SIZE;
      int startY = col*CELL_SIZE + HEADER_SIZE;
      if ((row + col) % 2 == 0)
        DrawRectangle(startX, startY, CELL_SIZE, CELL_SIZE, Fade(LIME, 0.85f)); 
      else
        DrawRectangle(startX, startY, CELL_SIZE, CELL_SIZE, Fade(LIME, 0.75f)); 
    }
  }
}

void setup(Snake *snake, Apple *apple, int *score) {
  *score = 0;
  *snake = Snake();
  apple->spawn(snake);
}

bool any_keypressed(void) {
  int key = GetKeyPressed();
  if (key >= 32 && key <= 127)
    return true;

  return false;
}

int main(void) {
  SetRandomSeed(1);
  const int screen_size = NUM_CELLS * CELL_SIZE;
  int score = 0; 
  game_state state = game_state::START;

  Snake snake; 
  Apple apple;

  apple.spawn(&snake);

  InitWindow(screen_size, screen_size + HEADER_SIZE, "SnakeGame");
  int targetFPS = 10;
  SetTargetFPS(targetFPS);

  while (!WindowShouldClose()) {
    switch (state) {
      case game_state::START:
        if (any_keypressed())
          state = game_state::GAME;
        break;

      case game_state::GAME:

        snake.change_dir();
        snake.move();

        if (snake.is_collision()) {
          setup(&snake, &apple, &score);
          state = game_state::DEAD;
        }

        if (snake.can_eat_apple(apple.getPos())) {
          snake.grow();
          apple.spawn(&snake);
          ++score;
        }

        break;

      case game_state::DEAD:
        if (any_keypressed())
          state = game_state::GAME;
        break;

      default:
        throw std::runtime_error("invalid game state!");
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    draw_header(score, state);
    draw_grid();

    apple.draw();
    snake.draw();
    
    EndDrawing();
  } 

  CloseWindow();
  return 0;
}
