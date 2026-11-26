#include "raylib.h"
#include <sstream>
#include <array>
#include <stdexcept>


#define NUM_CELLS 20
#define CELL_SIZE 30
#define HEADER_SIZE 40


enum class direction{
  UP = 0,
  LEFT,
  DOWN,
  RIGHT
};

void draw_header(int score) {
  std::stringstream score_text("");
  score_text << "Score: " << score;
  DrawText(score_text.str().c_str(), 10, 5, 30, BLACK);
}

void draw_grid() {
  for (std::size_t row {}; row < NUM_CELLS; ++row) {
    DrawLine(0, row * CELL_SIZE + HEADER_SIZE, GetScreenWidth(), row * CELL_SIZE + HEADER_SIZE, GRAY);
  }
  for (std::size_t col {}; col < NUM_CELLS; ++col) {
    DrawLine(col * CELL_SIZE, HEADER_SIZE, col * CELL_SIZE, GetScreenHeight(), GRAY);
  }

}

void setup() {

}
 void move_snake(std::array<Vector2, NUM_CELLS*NUM_CELLS> &snake, std::size_t snake_len, direction dir) { 
  Vector2 prev = snake[0];

  switch(dir) {
    case direction::UP:
      snake[0].y -= CELL_SIZE;
      break;
    case direction::LEFT:
      snake[0].x -= CELL_SIZE;
      break;
    case direction::DOWN:
      snake[0].y += CELL_SIZE;
      break;
    case direction::RIGHT:
      snake[0].x += CELL_SIZE;
      break;
    default:
        throw std::runtime_error("Invlaid Direction!");
  }

  Vector2 temp;
  for (std::size_t section{1}; section < snake_len; ++section) {
    temp = snake[section];
    snake[section] = prev;
    prev = temp;
  }
} 

direction change_dir(direction dir) {
  if(IsKeyPressed(KEY_W) && dir != direction::DOWN)
    dir = direction::UP;
  else if (IsKeyPressed(KEY_D) && dir != direction::LEFT)
    dir = direction::RIGHT;
  else if (IsKeyPressed(KEY_S) && dir != direction::UP)
    dir = direction::DOWN;
  else if (IsKeyPressed(KEY_A) && dir != direction::RIGHT)
    dir = direction::LEFT;

  return dir;
}

bool is_apple_in_snake(const Vector2 *apple, const std::array<Vector2, NUM_CELLS*NUM_CELLS> &snake) {
  for (const auto &segment : snake)
    if (apple->x == segment.x && apple->y == segment.y)
      return 1;

  return 0;
}

void spawn_apple(Vector2 *apple, const std::array<Vector2, NUM_CELLS*NUM_CELLS> &snake) {
  do{
    *apple = {
      (float) GetRandomValue(0, NUM_CELLS) * CELL_SIZE, 
      (float) GetRandomValue(0, NUM_CELLS) * CELL_SIZE + HEADER_SIZE
    };
  } while (is_apple_in_snake(apple, snake));
}

int main(void) {
  SetRandomSeed(1);
  const int screen_size = NUM_CELLS * CELL_SIZE;
 
  std::array<Vector2, NUM_CELLS*NUM_CELLS> snake;
  int snake_len = 3; 
  direction snake_dir = direction::RIGHT;

  snake[0].x = (NUM_CELLS / 2.0f) * CELL_SIZE;
  snake[0].y = (NUM_CELLS / 2.0f) * CELL_SIZE + HEADER_SIZE;
  for (std::size_t section{1}; section < snake_len; ++section) {
    snake[section].x = snake[section - 1].x - CELL_SIZE;
    snake[section].y = snake[0].y;
  }
  
  Vector2 apple;
  spawn_apple(&apple, snake);

  InitWindow(screen_size, screen_size + HEADER_SIZE, "SnakeGame");
  int targetFPS = 10;
  SetTargetFPS(targetFPS);

  while (!WindowShouldClose()) {
    snake_dir = change_dir(snake_dir);
    move_snake(snake, snake_len, snake_dir);

    if (snake[0].x == apple.x && snake[0].y == apple.y) {
      snake[snake_len] = {snake[snake_len - 1].x, snake[snake_len - 1].y};
      ++snake_len;
      spawn_apple(&apple, snake);
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    // draw apple
    DrawRectangle(apple.x, apple.y, CELL_SIZE, CELL_SIZE, RED);

    // draw snake
    for (std::size_t section {}; section < snake_len; ++section) {
      DrawRectangle(snake[section].x, snake[section].y, CELL_SIZE, CELL_SIZE, BLUE);
    }
    
    // draw world 
    draw_header(snake_len - 3);
    draw_grid();
    
    EndDrawing();
  } 

  CloseWindow();
  return 0;
}
