#include "raylib.h"

int main(void) {
  const int screenWidth = 600;
  const int screenHeight = 400;

  InitWindow(screenWidth, screenHeight, "window name");

  SetTargetFPS(60);

  while(!WindowShouldClose()) {
    BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawText("Hello World!", 190, 200, 20, LIGHTGRAY);
    EndDrawing();

  }

  CloseWindow();
  return 0;
}
