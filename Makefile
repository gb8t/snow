CC := gcc --std=c99

CFLAGS := -c -MMD -Wall -Wextra -Werror -Wno-unused

LDFLAGS := -lm

SDL_CONFIG := sdl2-config
CFLAGS += `$(SDL_CONFIG) --cflags`
LDFLAGS += `$(SDL_CONFIG) --libs`

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=build/%.o)

ASSETS_ := $(wildcard assets/*)
ASSETS  := $(ASSETS_:%=build/%.c)

EXEC := game

.PHONY: all
all: $(EXEC)

$(EXEC): $(ASSETS) $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

build/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

build/assets/%.tmx.c: assets/%.tmx
	@mkdir -p $(dir $@)
	./tool/tmx2c.py $< $@

build/assets/%.bmp.c: assets/%.bmp
	@mkdir -p $(dir $@)
	./tool/bin2c.py $< $@

.PHONY: clean
clean:
	rm -rf build

-include $(OBJS:.o=.d)
