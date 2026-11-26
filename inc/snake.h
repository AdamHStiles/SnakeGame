#include "raylib.h"
#include <array>

#define NUM_CELLS 20
#define CELL_SIZE 30
#define HEADER_SIZE 40

enum class direction {
  UP = 0,
  LEFT,
  DOWN,
  RIGHT
};

class Snake {
  std::array<Vector2, NUM_CELLS*NUM_CELLS> snake;
  direction dir;
  std::size_t snake_len;

public:
  
  Snake(void);

  void move(void);

  void change_dir(void);

  bool can_eat_apple(const Vector2 &apple);

  bool is_collision(void);

  void grow(void);

  void draw(void);

  std::array<Vector2, NUM_CELLS*NUM_CELLS> &getBody();

  std::size_t getLen() const;

};
