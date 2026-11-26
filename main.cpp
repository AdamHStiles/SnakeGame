#include "raylib.h"
#include <sstream>
#include <array>
#include <stdexcept>


#define NUM_CELLS 20
#define CELL_SIZE 30
#define HEADER_SIZE 40


enum class direction {
  UP = 0,
  LEFT,
  DOWN,
  RIGHT
};

enum class game_state {
  START = 0,
  GAME,
  DEAD
};

class Snake {
  std::array<Vector2, NUM_CELLS*NUM_CELLS> snake;
  direction dir;
  std::size_t snake_len;

public:
  
  Snake() {
    snake_len = 3;
    dir = direction::RIGHT;

    snake[0].x = (NUM_CELLS / 2.0f) * CELL_SIZE;
    snake[0].y = (NUM_CELLS / 2.0f) * CELL_SIZE + HEADER_SIZE;
    for (std::size_t section{1}; section < snake_len; ++section) {
      snake[section].x = snake[section - 1].x - CELL_SIZE;
      snake[section].y = snake[0].y;
    }
  }

  void move(void) { 
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

  void change_dir(void) {
    if(IsKeyPressed(KEY_W) && dir != direction::DOWN)
      dir = direction::UP;
    else if (IsKeyPressed(KEY_D) && dir != direction::LEFT)
      dir = direction::RIGHT;
    else if (IsKeyPressed(KEY_S) && dir != direction::UP)
      dir = direction::DOWN;
    else if (IsKeyPressed(KEY_A) && dir != direction::RIGHT)
      dir = direction::LEFT;
 }

  bool can_eat_apple(const Vector2 &apple) {
    if (snake[0].x == apple.x && snake[0].y == apple.y)
      return true;
    else
      return false;
  }

  bool is_collision(void) {
    Vector2 &head = snake[0];
    for(std::size_t segment {1}; segment < snake_len; ++segment) {
      if (head.x == snake[segment].x && head.y == snake[segment].y)
        return true;
    }

    if (head.x < 0 || head.x > GetScreenWidth())
      return true;
    else if (head.y < HEADER_SIZE || head.y > GetScreenHeight())
      return true;

    return false;
  }

  void grow(void) {
    snake[snake_len] = {snake[snake_len - 1].x, snake[snake_len - 1].y};
    ++snake_len;
  }

  void draw(void) {
    for (std::size_t section {}; section < snake_len; ++section) {
      DrawRectangle(snake[section].x, snake[section].y, CELL_SIZE, CELL_SIZE, BLUE);
    }
  }

  std::array<Vector2, NUM_CELLS*NUM_CELLS> &getBody() {
    return snake;
  }

  std::size_t getLen() const {
    return snake_len;
  }

};

class Apple {
  Vector2 pos;
  
  bool is_pos_valid(Snake *snake) {
    for(const auto &segment : snake->getBody()) {
      if (pos.x == segment.x && pos.y == segment.y)
        return false;
    }
    return true;
  }

public:
  void spawn(Snake *snake) {
    do{
      pos = {
        (float) GetRandomValue(0, NUM_CELLS-1) * CELL_SIZE, 
        (float) GetRandomValue(0, NUM_CELLS-1) * CELL_SIZE + HEADER_SIZE
      };
    } while (!is_pos_valid(snake));
  }

  void draw(void) const {
    DrawRectangle(pos.x, pos.y, CELL_SIZE, CELL_SIZE, RED);
  }

  Vector2 &getPos() {
    return pos;
  }

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
  for (std::size_t row {}; row < NUM_CELLS; ++row) {
    DrawLine(0, row * CELL_SIZE + HEADER_SIZE, GetScreenWidth(), row * CELL_SIZE + HEADER_SIZE, GRAY);
  }
  for (std::size_t col {}; col < NUM_CELLS; ++col) {
    DrawLine(col * CELL_SIZE, HEADER_SIZE, col * CELL_SIZE, GetScreenHeight(), GRAY);
  }

}

void setup(Snake *snake, Apple *apple, int *score) {
  *score = 0;
  *snake = Snake();
  apple->spawn(snake);
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
        if (IsKeyPressed(KEY_SPACE))
          state = game_state::GAME;
        break;

      case game_state::GAME:
        snake.change_dir();
        snake.move();

        if (snake.can_eat_apple(apple.getPos())) {
          snake.grow();
          apple.spawn(&snake);
          ++score;
        }

        if (snake.is_collision()) {
          setup(&snake, &apple, &score);
          state = game_state::DEAD;
        }
        break;

      case game_state::DEAD:
        snake.change_dir();
        snake.move();

        if (snake.can_eat_apple(apple.getPos())) {
          snake.grow();
          apple.spawn(&snake);
          ++score;
        }

        if (snake.is_collision()) {
          setup(&snake, &apple, &score);
          state = game_state::DEAD;
        }

        if (score >= 1)
          state = game_state::GAME;
        break;

      default:
        throw std::runtime_error("invalid game state!");
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);

    apple.draw();

    snake.draw();
    
    // draw world 
    draw_header(score, state);
    draw_grid();
    
    EndDrawing();
  } 

  CloseWindow();
  return 0;
}
