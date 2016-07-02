#include <knightos/display.h>
#include <knightos/system.h>
#include <knightos/keys.h>
#include <corelib.h>
#include <stdint.h>
#include "defs.h"

//#include <experimental/debug.h>

#define SPRSIZE 8

bool getBit(unsigned char, unsigned char);
unsigned char getSector(SCREEN*, unsigned char, unsigned char);
bool getPixel(SCREEN*, unsigned char, unsigned char);

/* Warning! C support in KnightOS is highly experimental. Your mileage may vary. */

void main() {
	SCREEN *screen;
	uint8_t key=NULL, posY=22, height=16, space=24;
	uint8_t i, j;
	unsigned char _;
	load_library("/lib/core");
	get_lcd_lock();
	get_keypad_lock();
	screen = screen_allocate();
	screen_clear(screen);
	screen_draw(screen);
	draw_rect_or(screen, height, 95, 0, 0);
	draw_rect_or(screen, 64-(height+space), 95, height+space, 0);
	draw_sprite_xor(screen, 0, posY, airplane_height, &airplane_sprite);
	while (1) {
		//screen_clear(screen);
		draw_sprite_xor(screen, 0, posY, airplane_height, &airplane_sprite);
		//draw_window(screen, "Axenntio's Test", WIN_DEFAULTS);
		switch(get_random()%3){
			case 1:
				if(height>0) height--;
				break;
			case 2:
				if(height<95) height++;
				break;
			default:
				break;
		}

		for(i=0;i<96;i++){
			reset_pixel(screen, 95, i);
		}
		for(i=0;i<height;i++){
			set_pixel(screen, 95, i);
		}
		for(i=height+space;i<96;i++){
			set_pixel(screen, 95, i);
		}


		for(i=1;i<96;i++){
			for(j=0;j<64;j++){
				if(getPixel(screen, i, j)){set_pixel(screen, i-1, j);}
				else{reset_pixel(screen, i-1, j);}
			}
		}

		key = app_get_key(&_);
		if(key==KEY_UP) {posY--;}
		if(key==KEY_DOWN) {posY++;}
		draw_sprite_xor(screen, 0, posY, airplane_height, &airplane_sprite);
		screen_draw(screen);
		//ksleep(10);
	}
}

bool getPixel(SCREEN* data, unsigned char x, unsigned char y){
	return getBit(getSector(data, x/8, y), 7-(x%8));
}

bool getBit(unsigned char byte, unsigned char number){
	return (byte >> number) & 0x01;
}

unsigned char getSector(SCREEN* data, unsigned char x, unsigned char y){
	return data[(y*12)+x];
}
