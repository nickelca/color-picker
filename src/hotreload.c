#include <assert.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>

#include "hotreload.h"

static FileModTime file_mod_time(const char *name) {
    struct stat stat_buf;
    if (stat(name, &stat_buf) < 0)
        return (FileModTime){0};
    return stat_buf.st_mtim;
}

static bool file_mod_time_changed(FileModTime a, FileModTime b) {
    return (a.tv_sec != b.tv_sec) || (a.tv_nsec != b.tv_nsec);
}

enum DYNLIB_ERR dynlib_populate(struct DynLib *dl, bool hard_fail) {
    if (access(dl->name, F_OK) != 0)
        return DYNLIB_ERR_MISSING_FILE;

    dl->handle = dlopen(dl->name, RTLD_LOCAL | RTLD_NOW);
    if (hard_fail) {
        assert(dl->handle);
    } else {
        return DYNLIB_ERR_DLOPEN;
    }

    for (size_t i = 0; i < dl->symbol_count; i++) {
        dl->symbols[i] = dlsym(dl->handle, dl->symbol_names[i]);
        if (hard_fail)
            assert(dl->symbols[i]);
    }

    dl->mod_time = file_mod_time(dl->name);

    return DYNLIB_OK;
}

bool dynlib_changed(struct DynLib *dl) {
    FileModTime test_time = file_mod_time(dl->name);

    if (file_mod_time_changed(test_time, dl->mod_time)) {
        dl->mod_time = test_time;
        return true;
    }
    return false;
}
