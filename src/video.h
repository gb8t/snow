#pragma once

#include "SDL.h"

#include "types.h"

#define BITMAPS \
	X(BITMAP_TEXT,   text) \

enum {
	#define X(id, name) \
		id,
	BITMAPS
	#undef X

	BITMAP_NUM
};

void video_init(void);
void video_clear(u32 col);
void video_flip(void);
void video_draw_bitmap(s32 x, s32 y, u32 id, SDL_Rect *rc);
void video_draw_text(s32 x, s32 y, u32 col, char *str);
void video_draw_text2(s32 x, s32 y, u32 col, char *fmt, ...);
void video_set_pixel(u32 x, u32 y, u32 col);
