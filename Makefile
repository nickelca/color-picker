default: build

SOURCES = main.c

CC = gcc
CFLAGS = -g3 -Wall -Wextra -Wpedantic -Werror -fsanitize=undefined,address -fno-strict-aliasing -lraylib
# CFLAGS = -Wall -Wextra -Wpedantic -Werror -fno-strict-aliasing -O2 -lraylib
OUT = main

OBJECTS = $(SOURCES:%.c=./obj/%.o)

$(OBJECTS): src/$(SOURCES)
	$(CC) $(CFLAGS) -c $< -o $@

build: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o ./bin/$(OUT)

run: ./bin/$(OUT)
	./bin/$(OUT)

clean:
	rm -f $(OBJECTS) ./bin/$(OUT)
