#include <knightos/display.h>
#include <knightos/system.h>
#include <knightos/keys.h>
#include "sprites.h"

/* Warning! C support in KnightOS is highly experimental. Your mileage may vary. */

void main() {
	SCREEN *screen;
	get_lcd_lock();
	screen = screen_allocate();
	screen_clear(screen);
	draw_string(screen, 0, 0, "test");
	screen_draw(screen);
	draw_sprite_or(screen, 0, 0, ball_height, &ball_sprite_start);
	while (1){
		draw_rect_xor(screen, 64, 96, 0, 0);
		screen_draw(screen);
		ksleep(100);
	}
}
