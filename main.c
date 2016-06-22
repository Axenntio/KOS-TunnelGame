#include <knightos/display.h>
#include <knightos/system.h>
#include <knightos/keys.h>
#include "defs.h"

/* Warning! C support in KnightOS is highly experimental. Your mileage may vary. */

void main() {
	SCREEN *screen;
	get_lcd_lock();
	screen = screen_allocate();
	screen_clear(screen);
	draw_string(screen, 0, 0, "Test");
	draw_sprite_or(screen, 32, 32, chess_height, &chess_sprite);
	screen_draw(screen);
	while (1){
		draw_rect_xor(screen, 64, 96, 0, 0);
		screen_draw(screen);
		ksleep(100);
	}
}
