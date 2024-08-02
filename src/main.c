#include <assert.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <raylib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static struct timespec file_modification_time(const char *name) {
    struct stat stat_buf;
    if (stat(name, &stat_buf) < 0)
        return (struct timespec){0};
    return stat_buf.st_mtim;
}

static int32_t compare_file_modification_time(struct timespec a,
                                              struct timespec b) {
    return (a.tv_sec - b.tv_sec) + (a.tv_nsec - b.tv_nsec);
}

static void (*run)(void);

static void nop(void) {}

static const char *libname = "./libcolor_picker.so";

static void *dlopen_color_picker() {
    if (access(libname, F_OK) != 0)
        return NULL;
    void *libhandle = dlopen(libname, RTLD_LOCAL | RTLD_NOW);
    assert(libhandle);
    run = dlsym(libhandle, "run");
    assert(run);
    return libhandle;
}

int main(void) {
    void *libhandle = dlopen_color_picker();

    InitWindow(640, 480, "Color Picker");

    struct timespec updated_time = file_modification_time(libname);
    while (!WindowShouldClose()) {
        run();

        struct timespec test_time = file_modification_time(libname);
        if (compare_file_modification_time(test_time, updated_time)) {
            if (libhandle) {
                dlclose(libhandle);
            }
            void *libhandle = dlopen_color_picker();
            if (!libhandle)
                run = nop;
            updated_time = test_time;
        }
    }

    dlclose(libhandle);

    return 0;
}
