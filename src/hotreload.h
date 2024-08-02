#ifndef HOT_RELOAD_H
#define HOT_RELOAD_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/stat.h>

typedef struct timespec FileModTime;

#define DL(object_name, dl_name, ...)                                          \
    const char *object_name##_symbol_names[] = {__VA_ARGS__};                  \
    void *object_name##_symbols[sizeof(object_name##_symbol_names) /           \
                                sizeof *(object_name##_symbol_names)] = {0};   \
    struct DynLib object_name = {                                              \
        .name = dl_name,                                                       \
        .symbol_names = object_name##_symbol_names,                            \
        .symbols = object_name##_symbols,                                      \
        .symbol_count = sizeof(object_name##_symbol_names) /                   \
                        sizeof *(object_name##_symbol_names),                  \
    }

struct DynLib {
    const char *name;
    const char **symbol_names;
    size_t symbol_count;
    void **symbols;
    void *handle;
    FileModTime mod_time;
};

enum DYNLIB_ERR {
    DYNLIB_OK,
    DYNLIB_ERR_MISSING_FILE,
    DYNLIB_ERR_DLOPEN,
};

enum DYNLIB_ERR dynlib_populate(struct DynLib *dl, bool hard_fail);
bool dynlib_changed(struct DynLib *dl);

#endif
