#include "video.h"

#include <stdarg.h>
#include <stdio.h>

#include "defs.h"

#include "../build/assets/text.bmp.c"

#define PIXEL(s, x, y) (((u32*)(s)->pixels)[(x) + (y) * (s)->w])

#define LOAD_BITMAP(name) \
	(SDL_LoadBMP_RW(SDL_RWFromMem(name##_bmp_data, name##_bmp_len), 1))

static SDL_Window *win;
static SDL_Renderer *ren;
static SDL_Surface *screen;

static SDL_Surface *bmp[BITMAP_NUM];

static void
video_load_bitmaps(void)
{
	#define X(id, name) \
		bmp[id] = LOAD_BITMAP(name);
	BITMAPS
	#undef X
}

void
video_init(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	win = SDL_CreateWindow(
		"GAME",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		3 * SCREEN_WIDTH,
		3 * SCREEN_HEIGHT,
		0);
	
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);

	screen = SDL_CreateRGBSurface(
		0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		32,
		0x00FF0000,
		0x0000FF00,
		0x000000FF,
		0xFF000000);

	video_load_bitmaps();
}

void
video_clear(u32 col)
{
	col |= 0xFF << 24;
	SDL_FillRect(screen, NULL, col);
}

void
video_flip(void)
{
	static SDL_Texture *texture;

	if (!texture) {
		texture = SDL_CreateTexture(
			ren,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			SCREEN_WIDTH,
			SCREEN_HEIGHT);
	}

	SDL_UpdateTexture(texture, NULL, screen->pixels, screen->pitch);
	SDL_RenderClear(ren);
	SDL_RenderCopy(ren, texture, NULL, NULL);
	SDL_RenderPresent(ren);
}

void
video_draw_bitmap(s32 x, s32 y, u32 id, SDL_Rect *rc)
{
	SDL_Rect dst = {x, y, 0, 0};

	SDL_BlitSurface(bmp[id], rc, screen, &dst);
}

void
video_draw_text(s32 x, s32 y, u32 col, char *str)
{
	SDL_Rect rc = {0, 0, 8, 8};

	s32 x0 = x;

	for (u32 i = 0; str[i]; i++) {
		char c = str[i];

		if (c == '\n') {
			y += 8;
			x = x0;
		} else if (c == ' ') {
			x += 8;
		} else if (c <= '_') {
			rc.x = (c - ' ') % 16 * 8;
			rc.y = (c - ' ') / 16 * 8;
			video_draw_bitmap(x, y, BITMAP_TEXT, &rc);
			x += 8;
		}
	}
}

void
video_draw_text2(s32 x, s32 y, u32 col, char *fmt, ...)
{
	char str[1024];

	va_list args;
	va_start(args, fmt);
	vsnprintf(str, sizeof(str), fmt, args);
	va_end(args);

	video_draw_text(x, y, col, str);
}

void
video_set_pixel(u32 x, u32 y, u32 col)
{
	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
		return;
	}
	PIXEL(screen, x, y) = col;
}
