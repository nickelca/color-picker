#include <raylib.h>

int main(void) {
    InitWindow(640, 480, "Color Picker");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(DARKPURPLE);
        DrawText("Hello World", 320, 240, 32, SKYBLUE);

        EndDrawing();
    }

    return 0;
}
