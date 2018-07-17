/*
 * Written by Evan Lalopoulos <evan.lalopoulos.2017@my.bristol.ac.uk>
 * Copyright (C) 2018 - All rights reserved.
 * Unauthorized copying of this file is strictly prohibited.
 */

#include "decoder.h"

char convert_to_ascii(unsigned char value) {
	return (char) (value - TELETEXT_TO_ASCII);
}

color decode_color(unsigned char value) {
	color result;

	switch (value) {
		case 0x81:
		case 0x91:
		result = RED;
		break;
		case 0x82:
		case 0x92:
		result = GREEN;
		break;
		case 0x83:
		case 0x93:
		result = YELLOW;
		break;
		case 0x84:
		case 0x94:
		result = BLUE;
		break;
		case 0x85:
		case 0x95:
		result = MAGENTA;
		break;
		case 0x86:
		case 0x96:
		result = CYAN;
		break;
		case 0x87:
		case 0x97:
		result = WHITE;
		break;
		default:
		ON_ERROR("Decoding failed. Not a valid color.");
		break;
	}

	return result;
}

heightOption decode_height_mode(unsigned char value) {
	heightOption result;

	switch(value) {
		case 0x8c:
		result = SINGLE;
		break;
		case 0x8d:
		result = DOUBLE;
		break;
		default:
		ON_ERROR("Decoding failed. Not a valid height.");
		break;
	}

	return result;
}

graphicsOption decode_graphics_mode(unsigned char value) {
	graphicsOption result;

	switch(value) {
		case 0x99:
		result = CONTIGUOUS;
		break;
		case 0x9a:
		result = SEPERATED;
		break;
		default:
		ON_ERROR("Decoding failed. Not a valid graphics mode.");
		break;
	}

	return result;
}

holdOption decode_hold_mode(unsigned char value) {
	holdOption result;

	switch(value) {
		case 0x9e:
		result = HOLD;
		break;
		case 0x9f:
		result = RELEASE;
		break;
		default:
		ON_ERROR("Decoding failed. Not a valid hold mode.");
		break;
	}

	return result;
}

Block_Graphic* init_block_graphic(unsigned char value) {
	Block_Graphic* blockGraphic;
	unsigned char s;
  int i, j, k;
  bool bits[TOTAL_SIXELS + 1];

	blockGraphic = (Block_Graphic*) malloc(sizeof(Block_Graphic));
	if (blockGraphic == NULL) {
		ON_ERROR("Block Graphic initialization failed. Memory allocation failed.");
	}

	/* Decode control code */
	s = value - SIXEL_BASE_CODE;

	/* Handle it as an 7 bit value with each bit representing a sixel */
	/* The 5th bit is ignored */
	/* 64 *(32) 16 8 4 2 1 */
	for(i = TOTAL_SIXELS; i >= 0; i--) {
    /* Shift s to right by i and check if last bit is set */
    bits[i] = (s >> i) & 1;
		/* Debug */
		/*printf("bits[i=%d] = %d\n", i, bits[i]);*/
	}

  /* Assign bit to correspoing sixels */
  k = 0;
  for(i=0; i < SIXELS_Y; i++) {
    for (j=0; j < SIXELS_X; j++) {
      blockGraphic->sixels[i][j] =  bits[k];
			/* Debug */
			/* printf("sixels[i=%d][j=%d] = bits[k=%d] = %d\n", i, j, k, bits[k]); */
			k++;

			/*Ignore 5th bit */
			if (k==5) {
				k++;
			}
    }
  }

	return blockGraphic;
}

bool is_breakthrough_text(unsigned char c) {
	if (c >= 0xc0 && c <= 0xdf) {
		return true;
	}

	return false;
}
