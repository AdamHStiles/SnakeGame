#include "raylib.h"
#include <array>
#include <stdexcept>
#include <thread>
#include <iostream>

#define NUM_CELLS 20
#define CELL_SIZE 30

enum class direction{
  UP = 0,
  LEFT,
  DOWN,
  RIGHT
};

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

int main(void) {
  const int screen_size = 600;
 
  std::array<Vector2, NUM_CELLS*NUM_CELLS> snake;
  int snake_len = 3; 
  direction snake_dir = direction::RIGHT;

  snake[0].x = (NUM_CELLS / 2) * CELL_SIZE;
  snake[0].y = (NUM_CELLS / 2) * CELL_SIZE;
  for (std::size_t section{1}; section < snake_len; ++section) {
    snake[section].x = snake[section - 1].x - CELL_SIZE;
    snake[section].y = snake[0].y;
  }

  InitWindow(screen_size, screen_size, "SnakeGame");
  
  int targetFPS = 10;
  SetTargetFPS(targetFPS);

  while (!WindowShouldClose()) {
    snake_dir = change_dir(snake_dir);
    move_snake(snake, snake_len, snake_dir);
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // draw snake
    for (std::size_t section {}; section < snake_len; ++section) {
      DrawRectangle(snake[section].x, snake[section].y, CELL_SIZE, CELL_SIZE, BLUE);
    }

    // draw grid lines
    for (std::size_t row {}; row < NUM_CELLS; ++row) {
      DrawLine(0, row * CELL_SIZE, screen_size, row * CELL_SIZE, GRAY);
    }
    for (std::size_t col {}; col < NUM_CELLS; ++col) {
      DrawLine(col * CELL_SIZE, 0, col * CELL_SIZE, screen_size, GRAY);
    }
    
    EndDrawing();
  } 

  CloseWindow();
  return 0;
}
