/*
 * 	decoder.h
 *
 *      Author: Evan Lalopoulos
 */

#ifndef DECODER_H_
#define DECODER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "token.h"

/* Used to calculate offset from ascii. */
/* A in teletext is 0xc1 */
#define TELETEXT_TO_ASCII (0xc1 - 'A')
#define SIXEL_BASE_CODE 0xa0
#define SIXELS_X 2
#define SIXELS_Y 3
#define TOTAL_SIXELS 6

#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

typedef enum outputOption {
	TEXT, GRAPHICS
} outputOption;

typedef enum color {
	RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE, BLACK
} color;

typedef enum heightOption {
	SINGLE, DOUBLE
} heightOption;

typedef enum partOption {
	WHOLE, TOP_HALF, BOTTOM_HALF
} partOption;

typedef enum graphicsOption {
	CONTIGUOUS, SEPERATED
} graphicsOption;

typedef enum holdOption {
	RELEASE, HOLD
} holdOption;

/* Block Graphics */
typedef bool sixel;

typedef struct Block_Graphic {
  sixel sixels[SIXELS_Y][SIXELS_X];
} Block_Graphic;

char convert_to_ascii(unsigned char value);
color decode_color(unsigned char value);
heightOption decode_height_mode(unsigned char value);
graphicsOption decode_graphics_mode(unsigned char value);
holdOption decode_hold_mode(unsigned char value);
Block_Graphic* init_block_graphic(unsigned char value);
bool is_breakthrough_text(unsigned char c);

#endif /* DECODER_H_ */
