#include "raylib.h"

#include "snake.h"
#include "apple.h"

bool Apple::is_pos_valid(Snake *snake) {
  for(const auto &segment : snake->getBody()) {
    if (pos.x == segment.x && pos.y == segment.y)
      return false;
  }
  return true;
}

void Apple::spawn(Snake *snake) {
  do{
    pos = {
      (float) GetRandomValue(0, NUM_CELLS-1) * CELL_SIZE, 
      (float) GetRandomValue(0, NUM_CELLS-1) * CELL_SIZE + HEADER_SIZE
    };
  } while (!is_pos_valid(snake));
}

void Apple::draw(void) const {
  DrawRectangle(pos.x, pos.y, CELL_SIZE, CELL_SIZE, RED);
}

Vector2 &Apple::getPos() {
  return pos;
}
