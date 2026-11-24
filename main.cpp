#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

typedef struct {
  float x;
  float y;
} FloatVector2;

class Ball {
  int radius;

  FloatVector2 *pos;
  FloatVector2 *speed;
  FloatVector2 *accel;

  void detectCollision(void) {
    if (pos->x - radius <= 0) {
      pos->x = radius;
      speed->x *= -1.0f;
    }
    else if (pos->x + radius >= SCREEN_WIDTH) {
      pos->x = SCREEN_WIDTH - radius;
      speed->x *= -1.0f;
    }
    
    if (pos->y - radius <= 0) {
      pos->y = radius;
      speed->y *= -1.0f;
    }
    else if (pos->y + radius >= SCREEN_HEIGHT) {
      pos->y = SCREEN_HEIGHT - radius;
      speed->y *= -1.0f;
    }
  }

public:

  Ball(int radius) : 
  radius(radius) { 
    pos = new FloatVector2{(float)radius + 10, (float)radius + 10};
    speed = new FloatVector2{0,0};
    accel = new FloatVector2{1.0f,9.8f};

  }

  void move(float time) {
    speed->x += time * accel->x * 100.0f;
    speed->y += time * accel->y * 100.0f;

    pos->x += (speed->x * time) + (0.5f * accel->x * time*time);
    pos->y += (speed->y * time) + (0.5f * accel->y * time*time);

    detectCollision(); 
  }
  
  int getRadius() { return radius;  }
  int getX() { return (int) pos->x; }
  int getY() { return (int) pos->y; }

  ~Ball() {
    delete pos;
    delete speed;
    delete accel;
  }

};

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "window name");

  Ball ball(15);
  
  SetTargetFPS(60);
  while (!WindowShouldClose()) {

    BeginDrawing();

    ClearBackground(RAYWHITE);
    
    ball.move(GetFrameTime());
    DrawCircle(ball.getX(), ball.getY(), ball.getRadius(), RED);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
