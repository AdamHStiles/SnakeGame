#include "raylib.h"

class Apple {
  Vector2 pos;
  
  bool is_pos_valid(Snake *snake);

public:
  void spawn(Snake *snake);

  void draw(void) const;

  Vector2 &getPos();

};
