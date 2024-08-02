#include <raylib.h>

void run(void) {
    BeginDrawing();

    ClearBackground(GRAY);
    DrawText("Hello World!", 320, 240, 32, SKYBLUE);

    EndDrawing();
}

#include <stdio.h>
void funny(void) {
    puts("Hello");
}
