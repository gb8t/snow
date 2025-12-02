#include "input.h"

#include "SDL.h"

u8 keys;
u8 keys_hit;

static u8 keys_prev;

void
input_init(void)
{
	keys = 0;
	keys_hit = 0;
}

s32
input_tick(void)
{
	SDL_Event ev;
	while (SDL_PollEvent(&ev)) {
		switch (ev.type) {
		case SDL_QUIT:
			return 1;
			break;
		case SDL_KEYDOWN:
			if (ev.key.repeat) {
				break;
			}

			#define X(id, name, code) \
				if (ev.key.keysym.sym == code) keys |= name;
			KEYS
			#undef X
			break;
		case SDL_KEYUP:
			#define X(id, name, code) \
				if (ev.key.keysym.sym == code) keys &= ~name;
			KEYS
			#undef X
			break;
		}
	}

	keys_hit = (keys ^ keys_prev) & keys;
	keys_prev = keys;

	return 0;
}
