#include <knightos/display.h>
#include <knightos/system.h>
#include <knightos/keys.h>
#include <corelib.h>
#include <stdint.h>
#include <kernel.h>
#include "defs.h"

//#include <experimental/debug.h>

#define SPRSIZE 8

bool getBit(uint8_t, uint8_t);
uint8_t getSector(SCREEN*, uint8_t, uint8_t);
bool getPixel(SCREEN*, uint8_t, uint8_t);
void init();
void scrollRightASM();
void scrollRight();
uint16_t loadHighScore();
void saveHightScore(uint16_t);

const char scoreDir[] = "/var/tunnel";
const char scorePath[] = "/var/tunnel/high";

/* Warning! C support in KnightOS is highly experimental. Your mileage may vary. */

SCREEN *screen;
uint8_t key, posY, height, space, gen, forTime;
uint16_t score, highscore;
uint8_t lost_focus;

uint16_t max(uint16_t a, uint16_t b)
{
	return (a > b) ? a : b;
}

void main()
{
	load_library("/lib/core");
	get_lcd_lock();
	get_keypad_lock();
	screen = screen_allocate();
	highscore = loadHighScore();
	init();
	while (1) {
		draw_sprite_xor(screen, 88, posY, PLAYER_HEIGHT, &PLAYER_SPRITE);
		draw_rect_or(screen, 8, 96, 56, 0);

		if (!forTime) {
			gen = get_random() % 3;
			forTime = 2 + get_random() % 3;
		}
		else {
			forTime--;
		}
		switch (gen) {
		case 1:
			if (height > 1) {
				height--;
			}
			break;
		case 2:
			if (height < 56 - space) {
				height++;
			}
			break;
		default:
			break;
		}

		scrollRightASM();
		// scrollRight();
		space = max(10, INITIAL_SPACE - (score / (INITIAL_SPACE * 10)));
		ksleep(max(1, 20 - (score / 200)));

		key = app_get_key(&lost_focus);
		if (key == KEY_UP) {
			posY--;
		}
		if (key == KEY_DOWN) {
			posY++;
		}
		draw_sprite_xor(screen, 88, posY, PLAYER_HEIGHT, &PLAYER_SPRITE);
		draw_rect_and(screen, 7, 96, 57, 0);
		draw_string(screen, 1, 58, "Score:");
		draw_short(screen, 26, 58, score);
		draw_string(screen, 55, 58, "Best:");
		draw_short(screen, 76, 58, highscore);
		screen_draw(screen);

		if (!getPixel(screen, 95, posY) ||
			!getPixel(screen, 95, posY + 7) ||
			!getPixel(screen, 92, posY + 3) ||
			!getPixel(screen, 92, posY + 4) ) {
			if (score > highscore) {
				highscore = score;
				// saveHightScore();
			}
			init();
		}


		score++;
		if (score % 1000 == 0 && space > 12) {
			space--;
		}
		if (score == 60000) {
			space = 0;
		}
	}
}

bool getPixel(SCREEN* data, uint8_t x, uint8_t y)
{
	return getBit(getSector(data, x / 8, y), 7 - (x % 8));
}

bool getBit(uint8_t byte, uint8_t number)
{
	return (byte >> number) & 0x01;
}

uint8_t getSector(SCREEN* data, uint8_t x, uint8_t y)
{
	return data[(y * 12) + x];
}

void scrollRight(void)
{
	uint8_t *p = screen;
	uint8_t y, n, b, carry;
	uint8_t bottom = height + space;

	for (y = 0 ; y < 56; y++) {
		carry = (y < height || y >= bottom) ? 0x80: 0;
		for (n = 12; n; n--) {
			b = *p;
			*p++ = (b >> 1) | carry;
			carry = b << 7;
		}
	}
}

void scrollRightASM(void) __naked
{
	__asm
	ld hl, (_screen)
	ld a, (_height)
	ld d, a
	ld e, a
	ld a, (_space)
	add a, e
	ld e, a
	ld c, 0
.row:
	ld a, c
	cp d
	jr c, .wall
	cp e
	ccf
.wall:
	ld b, 12
.byte:
	rr (hl)
	inc hl
	djnz .byte
	inc c
	ld a, c
	cp 56
	jr nz, .row
	ret
	__endasm;
}



void init()
{
	key = NULL;
	space = INITIAL_SPACE;
	height = (56 - space) / 2;
	posY = 56 / 2 - PLAYER_HEIGHT / 2;
	forTime = 0;
	score = 0;
	screen_clear(screen);
	draw_rect_or(screen, height, 96, 0, 0);
	draw_rect_or(screen, 64 - (height + space), 96, height + space, 0);
	draw_sprite_xor(screen, 88, posY, PLAYER_HEIGHT, &PLAYER_SPRITE);
}

uint16_t loadHighScore() __naked
{
	__asm
	ld de, _scorePath
	PCALL(OPENFILEREAD)
	jr nz, .none
	PCALL(STREAMREADWORD)
	jr z, .close
	ld hl, 0
.close:
	push hl
	PCALL(CLOSESTREAM)
	pop hl
	ret
.none:
	ld hl, 0
	ret
	__endasm;
}

void saveHightScore(uint16_t) __naked
{
	__asm
	ld de, _scoreDir
	PCALL(DIRECTORYEXISTS)
	jr z, .open
	ld de, _scoreDir
	PCALL(CREATEDIRECTORY)
.open:
	ld de, _scorePath
	PCALL(OPENFILEWRITE)
	ret nz
	ld hl, (_highScore)
	PCALL(STREAMWRITEWORD)
	PCALL(CLOSESTREAM)
	ret
	__endasm
}