/*
 * Written by Evan Lalopoulos <evan.lalopoulos.2017@my.bristol.ac.uk>
 * Copyright (C) 2018 - All rights reserved.
 * Unauthorized copying of this file is strictly prohibited.
 */

#ifndef TOKEN_H_
#define TOKEN_H_

#define MAX_LENGTH 40
#define MAX_LINES 25
/* Control Codes */
#define AN_COLOR_S 0x81
#define AN_COLOR_E 0x87
#define SINGLE_HEIGHT_CODE 0x8c
#define	DOUBLE_HEIGHT_CODE 0x8d
#define GRAPHICS_COLOR_S 0x91
#define GRAPHICS_COLOR_E 0x97
#define CONT_GRAPHICS_CODE 0x99
#define	SEP_GRAPHICS_CODE 0x9a
#define BLACK_BG_CODE 0x9c
#define NEW_BG_CODE 0x9d
#define	HOLD_MODE_CODE 0x9e
#define RELEASE_MODE_CODE 0x9f
#define SYMBOL_RANGE_S 0xa0

typedef enum tokenType {
	UNUSED,
	SYMBOL,
	ALPHANUM_COLOR,
	GRAPHICS_COLOR,
	BLACK_BG_COLOR,
	NEW_BG_COLOR,
	HEIGHT_MODE,
	GRAPHICS_MODE,
	HOLD_MODE
} tokenType;

typedef struct token {
	tokenType type;
	unsigned char attribute;
	/* Debug */
	int n;
	int line;
	char str[50];
} Token;

typedef struct TokenStream {
	Token* tokens[MAX_LINES][MAX_LENGTH];
	int tokenCount;
} TokenStream;

#endif /* TOKEN_H_ */
