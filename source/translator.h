/*
 * 	translator.h
 *
 *      Author: Evan Lalopoulos
 */

#ifndef TRANSLATOR_H_
#define TRANSLATOR_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "token.h"
#include "tokenizer.h"
#include "decoder.h"

#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

/* Stores how a single element must be displayed */
typedef struct pixel {
	unsigned char character;
	Block_Graphic* blockGraphic;
	outputOption outputMode;
	color alphanumColor;
	color graphicsColor;
	color bgColor;
	heightOption heightMode;
	partOption charPart;
	graphicsOption graphicsMode;
	holdOption holdMode;
	/* Debug */
	/* Token* token; */
} Pixel;

typedef struct teletext {
	Pixel pixels[MAX_LINES][MAX_LENGTH];
} Teletext;

typedef struct state {
	Block_Graphic* lastGraphic;
	graphicsOption lastGraphicsMode;
	outputOption outputMode;
	color alphanumColor;
	color graphicsColor;
	color bgColor;
	heightOption heightMode;
	graphicsOption graphicsMode;
	holdOption holdMode;
} State;

Teletext* TLT_translate(TokenStream* tokenStream);
void process_token(TokenStream* tokenStream, Teletext* teletext, State* currentState, State* nextState, int row, int column);
void handle_symbol_token(Teletext* teletext, Token* token, Pixel* pixel, State* currentState, State* nextState, int row, int column);
void handle_control_token(TokenStream* tokenStream, Pixel* pixel, State* currentState, State* nextState, int row, int column);
void handle_alphanum_color(Token* token, Pixel* pixel, State* currentState, State* nextState);
void handle_graphics_color(Token* token, Pixel* pixel, State* currentState, State* nextState);
void handle_new_bg_color(TokenStream* tokenStream, Pixel* pixel, State* nextState,  int row, int column);
void handle_height_mode(Token* token, Pixel* pixel, State* currentState, State* nextState);
void handle_graphics_mode(Token* token, Pixel* pixel, State* currentState, State* nextState);
void handle_hold_mode(Token* token, Pixel* pixel, State* currentState, State* nextState);
void set_pixel_properties(Pixel* pixel, State* state);
void set_pixel_to_hold(Pixel* pixel, State* currentState);
void release_graphics(Pixel* pixel, State* state);
void set_character_part(Teletext* teletext, Pixel* pixel, State* currentState, int row, int column);
void reset_state(State* state);
void debug_pixel(Pixel pixel);


#endif /* TRANSLATOR_H_ */
