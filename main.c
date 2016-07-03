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
void init();

/* Warning! C support in KnightOS is highly experimental. Your mileage may vary. */

SCREEN *screen;
uint8_t key, posY, height, space, bit1, bit2, gen, forTime;
uint16_t i, score, bestscore=0;
unsigned char _;

void main() {
	load_library("/lib/core");
	get_lcd_lock();
	get_keypad_lock();
	screen = screen_allocate();
	init();
	while(1){
		draw_sprite_xor(screen, 88, posY, player_height, &player_sprite);
		draw_rect_or(screen, 8, 96, 56, 0);
		
		if(!forTime){
			gen=get_random()%3;
			forTime=2+get_random()%3;
		}
		else{
			forTime--;
		}
		switch(gen){
			case 1:
				if(height>1) height--;
				break;
			case 2:
				if(height<56-space) height++;
					break;
			default:
				break;
		}

		//Right Shift
		for(i=0; i<768; i++) {
		    bit2 = screen[i] & 0x01;
		    screen[i] >>= 1;
		    screen[i] |= bit1 << 7;
		    bit1 = bit2;
		}

		for(i=0;i<56;i++){reset_pixel(screen, 0, i);}
		for(i=0;i<height;i++){set_pixel(screen, 0, i);}
		for(i=height+space;i<56;i++){set_pixel(screen, 0, i);}

		key = app_get_key(&_);
		if(key==KEY_UP) {posY--;}
		if(key==KEY_DOWN) {posY++;}
		draw_sprite_xor(screen, 88, posY, player_height, &player_sprite);
		draw_rect_and(screen, 7, 96, 57, 0);
		draw_string(screen, 1, 58, "Score:");
		draw_short(screen, 26, 58, score);
		draw_string(screen, 55, 58, "Best:");
		draw_short(screen, 76, 58, bestscore);
		screen_draw(screen);

		if(getPixel(screen, 95, posY) &&
			getPixel(screen, 95, posY+7) &&
			getPixel(screen, 92, posY+3) &&
			getPixel(screen, 92, posY+4)){
		}
		else{
			if(score>bestscore){
				bestscore=score;
			}
			init();
		}

		
		score++;
		if(score%1000==0 && space>12){
			space--;
		}
		if(score==60000){
			space=0;
		}
	}
}

bool getPixel(SCREEN* data, unsigned char x, unsigned char y){return getBit(getSector(data, x/8, y), 7-(x%8));}
bool getBit(unsigned char byte, unsigned char number){return (byte >> number) & 0x01;}
unsigned char getSector(SCREEN* data, unsigned char x, unsigned char y){return data[(y*12)+x];}
void init(){
	key=NULL;
	posY=22;
	height=16;
	space=24;
	forTime=0;
	score=0;
	screen_clear(screen);
	draw_rect_or(screen, height, 96, 0, 0);
	draw_rect_or(screen, 64-(height+space), 96, height+space, 0);
	draw_sprite_xor(screen, 88, posY, player_height, &player_sprite);
}