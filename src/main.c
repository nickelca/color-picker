#include <assert.h>
#include <dlfcn.h>
#include <raylib.h>

static void (*run)(void);

int main(void) {
    void *libhandle = dlopen("./libcolor_picker.so", RTLD_LOCAL | RTLD_NOW);
    assert(libhandle);
    run = dlsym(libhandle, "run");
    assert(run);

    InitWindow(640, 480, "Color Picker");

    while (!WindowShouldClose()) {
        run();
    }

    dlclose(libhandle);

    return 0;
}
