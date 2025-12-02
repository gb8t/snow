#include "SDL.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "defs.h"
#include "input.h"
#include "types.h"
#include "util.h"
#include "video.h"

#define SNOW_MAX 0xFF

typedef struct {
	f32 x, y;
	f32 dx, dy;
	u32 col;
	bool water;
	bool stat;
	u8 just_stat;
} SNOW;

static SNOW snow[SNOW_MAX];

int
main(int, char**)
{
	bool pause = 0;
	s32 mx, my;
	u8 click;
	f32 flash = 0;
	u8 bg = 0;
	u8 mode = 0;

	srand(time(NULL));

	video_init();

	for (u16 i = 0; i < SNOW_MAX; i++) {
		SNOW *s = &snow[i];
		s->x = rand() % SCREEN_WIDTH;
		s->y = rand() % SCREEN_HEIGHT;
	}

	while (!input_tick()) {
		if (keys_hit & KEY_PAUSE) {
			pause = !pause;
		}

		click = SDL_GetMouseState(&mx, &my);
		mx /= 3;
		my /= 3;

		if (keys_hit & KEY_SNOW) mode = 0;
		if (keys_hit & KEY_RAIN) mode = 1;

		if (!pause) {
			for (u16 i = 0; i < SNOW_MAX; i++) {
				SNOW *s = &snow[i];
				f32 prev_x = s->x;
				f32 prev_y = s->y;

				if (keys & KEY_LEFT)  s->x += 2;
				if (keys & KEY_RIGHT) s->x -= 2;
				if (keys & KEY_UP)    s->y += 2;
				if (keys & KEY_DOWN)  s->y -= 2;

				if (s->water) {
					if (mode == 1) {
						s->dx = MIN(1, s->dx + .01);
						s->dy = MIN(5, s->dy + .05);
					} else if (rand() % 5 == 0) {
						s->dy *= .9;
					}

					s->y += s->dy;
					s->x += s->dx;

					if (click) {
						f32 dx, dy, d;

						dx = s->x - mx;
						dy = s->y - my;
						d = sqrt(dx * dx + dy * dy);

						if (d < 32) {
							f32 a = atan2(dy, dx);
							s->dx = cos(a) * .5;
							s->dy = sin(a) * .5;
						}
					}

					if (s->x > SCREEN_WIDTH)  s->x = 0;
					if (s->y > SCREEN_HEIGHT) s->y = 0;
					if (s->x < 0) s->x = SCREEN_WIDTH;
					if (s->y < 0) s->y = SCREEN_HEIGHT;

					f32 d = sqrt(s->dx * s->dx + s->dy * s->dy);
					u32 col = LERP(0xFF, 0, MIN(1, d));
					s->col = (col << 16) | (col << 8) | 0xFF;

					if (mode != 1 && s->dy < .5) {
						s->water = 0;
						s->just_stat = s->col & 0xFF;
					}

					continue;
				}

				if (s->stat) {
					f32 dx = s->x - mx;
					f32 dy = s->y - my;
					f32 d = sqrt(dx * dx + dy * dy);

					s->col = -1;

					if (!click || d > 32) {
						s->dx = s->dy = 0;
						s->stat = 0;
						s->just_stat = s->col & 0xFF;
					}

					continue;
				}

				s->dx += (f32)(rand() % 11 - 5) / 100;
				s->dy += (f32)(rand() % 11 - 5) / 100;

				if (s->dx >  .5) s->dx *= .5;
				if (s->dy >  .5) s->dy *= .5;
				if (s->dx < -.5) s->dx *= .5;
				if (s->dy < -.5) s->dy *= .5;

				s->x += s->dx;
				s->y += s->dy + .5;

				f32 d = sqrt(s->dx * s->dx + s->dy * s->dy);
				d = d * d * 5;

				u8 col = LERP(bg, 0xFF, MIN(1, d));
				if (s->just_stat > col) {
					s->just_stat--;
				} else {
					s->just_stat = 0;
					s->col = (col << 16) | (col << 8) | col;
					//SDL_Log("%x", s->col);
				}

				if (d == 0) {
					s->x = rand() % SCREEN_WIDTH;
					s->y = rand() % SCREEN_HEIGHT;
				}

				if (mode == 1 && !(rand() % 20)) {
					s->water = 1;
				}

				if (click) {
					f32 dx = s->x - mx;
					f32 dy = s->y - my;
					f32 d = sqrt(dx * dx + dy * dy);

					dx = prev_x - mx;
					dy = prev_y - my;
					f32 d2 = sqrt(dx * dx + dy * dy);

					if ((d < 32) != (d2 < 32)) {
						s->stat = 1;
					}
				}

				if (s->x > SCREEN_WIDTH)  s->x = 0;
				if (s->y > SCREEN_HEIGHT) s->y = 0;
				if (s->x < 0) s->x = SCREEN_WIDTH;
				if (s->y < 0) s->y = SCREEN_HEIGHT;
			}

			flash += (rand() % 3) - 1;
			if (flash > 10 || flash < 0) flash = 0;

			if (mode == 1) {
				bg = LERP(bg, 0x20, .02);
			} else {
				bg = LERP(bg, 0xA0, .02);
			}
		}

		u32 bg_col = (bg << 16) | (bg << 8) | bg;
		video_clear((mode == 1 && (flash > 9)) ? (u32)-1 : bg_col);

		for (u16 i = 0; i < SNOW_MAX; i++) {
			SNOW *s = &snow[i];
			video_set_pixel(
				s->x,
				s->y,
				(mode == 1 && (flash > 9)) ? 0 : s->col);
		}

		video_flip();
	}

	SDL_Quit();

	return 0;
}
