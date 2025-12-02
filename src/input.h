#pragma once

#include "types.h"

#define KEYS \
	X(0, KEY_LEFT,  SDLK_LEFT) \
	X(1, KEY_RIGHT, SDLK_RIGHT) \
	X(2, KEY_UP,    SDLK_UP) \
	X(3, KEY_DOWN,  SDLK_DOWN) \
	X(4, KEY_SNOW,  SDLK_1) \
	X(5, KEY_RAIN,  SDLK_2) \
	X(6, KEY_PAUSE, SDLK_RETURN) \

enum {
	#define X(id, name, code) \
		name = (1 << id),
	KEYS
	#undef X
};

extern u8 keys;
extern u8 keys_hit;

void input_init(void);
s32 input_tick(void);
