#include "raylib.h"

int main(void) {
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "window name");

  int currentFPS = 60;

  Vector2 deltaCircle = {0, (float)screenHeight / 3.0f};
  Vector2 frameCircle = {0, (float)screenHeight * (2.0f / 3.0f)};

  const float speed = 10.0f;
  const float circleRadius = 32.0f;

  SetTargetFPS(currentFPS);

  while (!WindowShouldClose()) {

    float mouseWheel = GetMouseWheelMove();
    if (mouseWheel != 0) {
      currentFPS += (int)mouseWheel;
      if (currentFPS < 0)
        currentFPS = 0;
      SetTargetFPS(currentFPS);
    }

    deltaCircle.x += GetFrameTime() * 6.0f * speed;
    frameCircle.x += 0.1f * speed;

    if (deltaCircle.x >= screenWidth)
      deltaCircle.x = 0;
    if (frameCircle.x >= screenWidth)
      frameCircle.x = 0;

    if (IsKeyPressed(KEY_R)) {
      deltaCircle.x = 0;
      frameCircle.x = 0;
    }

    BeginDrawing();

    ClearBackground(RAYWHITE);
    DrawCircleV(deltaCircle, speed, RED);
    DrawCircleV(frameCircle, speed, BLUE);

    const char *fpsText = 0;
    if (currentFPS <= 0)
      fpsText = TextFormat("FPS: unlimited (%i)", GetFPS());
    else
      fpsText = TextFormat("FPS: %i (target %i)", GetFPS(), currentFPS);

    DrawText(fpsText, 10, 10, 20, DARKGRAY);
    DrawText(TextFormat("Frame time: %2.0f ms", GetFrameTime() * 1000.0f), 10,
             30, 20, DARKGRAY);

    DrawText("FUNC: x += GetFrameTime()*speed", 10, 90, 20, RED);
    DrawText("FUNC: x += speed", 10, 240, 20, RED);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
