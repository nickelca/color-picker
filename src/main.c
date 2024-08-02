#include <assert.h>
#include <dlfcn.h>
#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "hotreload.h"

static void (*run)(void);

static void nop(void) {}

int main(void) {
    DL(color_picker, "./libcolor_picker.so", "run");
    assert(dynlib_populate(&color_picker, true) == DYNLIB_OK);
    run = color_picker.symbols[0];

    InitWindow(640, 480, "Color Picker");

    while (!WindowShouldClose()) {
        run();

        if (dynlib_changed(&color_picker)) {
            if (color_picker.handle) {
                dlclose(color_picker.handle);
            }
            switch (dynlib_populate(&color_picker, true)) {
            case DYNLIB_ERR_MISSING_FILE:
                run = nop;
                break;
            case DYNLIB_ERR_DLOPEN:
                fprintf(stderr, "error: dlopen failed");
                return 1;
            case DYNLIB_OK:
                run = color_picker.symbols[0];
                break;
            }
        }
    }

    dlclose(color_picker.handle);

    return 0;
}
