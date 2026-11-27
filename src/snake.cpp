#include "snake.h"
#include "raylib.h"
#include <array>
#include <stdexcept>

#define NUM_CELLS 20
#define CELL_SIZE 30
#define HEADER_SIZE 40

Snake::Snake(void) {
  snake_len = 3;
  dir = direction::RIGHT;

  snake[0].x = (NUM_CELLS / 2.0f) * CELL_SIZE;
  snake[0].y = (NUM_CELLS / 2.0f) * CELL_SIZE + HEADER_SIZE;
  for (std::size_t section{1}; section < snake_len; ++section) {
    snake[section].x = snake[section - 1].x - CELL_SIZE;
    snake[section].y = snake[0].y;
  }
}

void Snake::move(void) {
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

void Snake::change_dir(void) {
  if(IsKeyPressed(KEY_W) && dir != direction::DOWN)
    dir = direction::UP;
  else if (IsKeyPressed(KEY_D) && dir != direction::LEFT)
    dir = direction::RIGHT;
  else if (IsKeyPressed(KEY_S) && dir != direction::UP)
    dir = direction::DOWN;
  else if (IsKeyPressed(KEY_A) && dir != direction::RIGHT)
    dir = direction::LEFT;
}

bool Snake::can_eat_apple(const Vector2 &apple) {
  if (snake[0].x == apple.x && snake[0].y == apple.y)
    return true;
  else
    return false;
}

bool Snake::is_collision(void) {
  Vector2 &head = snake[0];
  int right_edge = (NUM_CELLS-1) * CELL_SIZE;
  int bottom_edge = (NUM_CELLS-1) * CELL_SIZE + HEADER_SIZE;
  if (head.x < 0 || head.x > right_edge)
    return true;
  else if (head.y < HEADER_SIZE || head.y > bottom_edge)
    return true;

  for(std::size_t segment {1}; segment < snake_len; ++segment) {
    if (head.x == snake[segment].x && head.y == snake[segment].y)
      return true;
  }
  return false;
}

void Snake::grow(void) {
  snake[snake_len] = {snake[snake_len - 1].x, snake[snake_len - 1].y};
  ++snake_len;
}

void Snake::draw(void) {
  for (std::size_t section {0}; section < snake_len; ++section) {
    DrawRectangle(snake[section].x, snake[section].y, CELL_SIZE, CELL_SIZE, BLUE);
  }
  Rectangle head_rec = {snake[0].x, snake[0].y, CELL_SIZE, CELL_SIZE};
  DrawRectangleLinesEx(head_rec, 6, Fade(DARKBLUE, 0.5f));
}

std::array<Vector2, NUM_CELLS*NUM_CELLS> &Snake::getBody() {
  return snake;
}

std::size_t Snake::getLen() const {
  return snake_len;
}
