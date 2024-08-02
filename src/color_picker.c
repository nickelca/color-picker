#include <raylib.h>

void run(void) {
    BeginDrawing();

    ClearBackground(DARKPURPLE);
    DrawText("Hello World", 320, 240, 32, SKYBLUE);

    EndDrawing();
}
