#include <knightos/display.h>
#include <knightos/system.h>
#include <knightos/keys.h>
#include <corelib.h>
#include <stdint.h>
#include "defs.h"

//#include <experimental/debug.h>

/* Warning! C support in KnightOS is highly experimental. Your mileage may vary. */

void main() {
	SCREEN *screen;
	uint8_t key=NULL;
	uint8_t posX=0;
	uint8_t posY=0;
	uint8_t dir=0;
	get_lcd_lock();
	get_keypad_lock();
	screen = screen_allocate();
	screen_clear(screen);
	//draw_string(screen, 1, 1, "Test");
	//draw_sprite_or(screen, 32, 32, chess_height, &chess_sprite);
	screen_draw(screen);
	while (1){
		screen_clear(screen);
		draw_window(screen, "Axenntio's Test", WIN_DEFAULTS);
		key=get_key();
		if(posX%8==0 && posY%8==0){
			if(key==KEY_DOWN) {posY++;dir=0;}
			else if(key==KEY_UP) {posY--;dir=1;}
			else if(key==KEY_RIGHT) {posX++;dir=2;}
			else if(key==KEY_LEFT) {posX--;dir=3;}
		}
		else{
			if(dir==0){posY++;}
			if(dir==1){posY--;}
			if(dir==2){posX++;}
			if(dir==3){posX--;}
		}
		draw_sprite_or(screen, posX, posY, chess_height, &chess_sprite);
		screen_draw(screen);
		ksleep(10);
	}
}
