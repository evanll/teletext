/*
* translator.c
*
*      Author: Evan Lalopoulos
*/

#include "translator.h"

/* A state machine to translate tokens to teletext */
Teletext* TLT_translate(TokenStream* tokenStream) {
  Teletext* teletext;
  State currentState;
  State nextState;
  int i,j;

  if (tokenStream == NULL) {
    ON_ERROR("Null pointer to tokenstream.");
  }

  teletext = (Teletext*) malloc(sizeof(Teletext));
  if (teletext == NULL) {
    ON_ERROR("Failed to create teletext");
  }

  for(i=0; i<MAX_LINES; i++) {
    reset_state(&currentState);
    reset_state(&nextState);
    for(j=0; j<MAX_LENGTH; j++) {
      set_pixel_properties(&(teletext->pixels[i][j]), &currentState);
      process_token(tokenStream, teletext, &currentState, &nextState, i, j);

      currentState = nextState;

      /*Debug*/
      /* debug_token(*(tokenStream->tokens[i][j])); */
      /* debug_pixel(teletext->pixels[i][j]); */
    }
  }

  return teletext;
}

void process_token(TokenStream* tokenStream, Teletext* teletext,
  State* currentState, State* nextState, int row, int column) {

  Token* token;

  token = tokenStream->tokens[row][column];

  if(token->type == SYMBOL || token->type == UNUSED) {
    handle_symbol_token(teletext, token, &(teletext->pixels[row][column]),
    currentState, nextState, row, column);
  } else {
    handle_control_token(tokenStream, &(teletext->pixels[row][column]),
    currentState, nextState, row, column);
  }
}

void handle_symbol_token(Teletext* teletext, Token* token, Pixel* pixel,
  State* currentState, State* nextState, int row, int column) {

  /* Determine output */
  switch (currentState->outputMode) {
    case TEXT:
    pixel->character = convert_to_ascii(token->attribute);
    set_character_part(teletext, pixel, currentState, row, column);
    break;
    case GRAPHICS:
    if (is_breakthrough_text(token->attribute)) {
      pixel->outputMode = TEXT;
      pixel->character = convert_to_ascii(token->attribute);
      pixel->alphanumColor = pixel->graphicsColor;
    } else {
      pixel->blockGraphic = init_block_graphic(token->attribute);

      /* Cache last graphic for hold */
      nextState->lastGraphic = pixel->blockGraphic;
    }
    break;
  }
}

void handle_control_token(TokenStream* tokenStream, Pixel* pixel,
  State* currentState, State* nextState, int row, int column) {

  Token* token;

  token = tokenStream->tokens[row][column];

  /* Determine control's code output */
  if (currentState->holdMode == HOLD && currentState->lastGraphic != NULL) {
    set_pixel_to_hold(pixel, currentState);
  } else {
    pixel->outputMode = TEXT;
    pixel->heightMode = SINGLE;
    pixel->charPart = WHOLE;
    pixel->character = ' ';
  }

  /* Now deal deals with the control token */
  switch (token->type) {
    case ALPHANUM_COLOR:
    handle_alphanum_color(token, pixel, currentState, nextState);
    break;
    case GRAPHICS_COLOR:
    handle_graphics_color(token, pixel, currentState, nextState);
    break;
    case BLACK_BG_COLOR:
    pixel->bgColor = nextState->bgColor = BLACK;
    break;
    case NEW_BG_COLOR:
    handle_new_bg_color(tokenStream, pixel, nextState, row, column);
    break;
    case HEIGHT_MODE:
    handle_height_mode(token, pixel, currentState, nextState);
    break;
    case GRAPHICS_MODE:
    handle_graphics_mode(token, pixel, currentState, nextState);
    break;
    case HOLD_MODE:
    handle_hold_mode(token, pixel, currentState, nextState);
    break;
    default:
    ON_ERROR("Failed to handle token type.");
    break;
  }
}

void handle_alphanum_color(Token* token, Pixel* pixel, State* currentState, State* nextState) {
  color alphanumColor;

  alphanumColor = decode_color(token->attribute);

  nextState->alphanumColor = alphanumColor;
  nextState->outputMode = TEXT;

  /* Apply color to this pixel in case it's displayed as hold */
  pixel->alphanumColor =  alphanumColor;

  /*Hold rule */
  if(currentState->holdMode == HOLD) {
    if (nextState->outputMode != currentState->outputMode) {
      release_graphics(pixel, nextState);
    }
  }
}

void handle_graphics_color(Token* token, Pixel* pixel, State* currentState, State* nextState) {
  color graphicsColor;

  graphicsColor = decode_color(token->attribute);

  nextState->outputMode = GRAPHICS;
  nextState->graphicsMode = CONTIGUOUS;
  nextState->graphicsColor = graphicsColor;

  /* Apply to this pixel also in case it's displayed as hold */
  pixel->graphicsColor =  graphicsColor;

  /* Hold Rule because this also sets CONTIGUOUS graphics mode */
  if(currentState->holdMode == HOLD) {
    if (nextState->graphicsMode != currentState->graphicsMode) {
      release_graphics(pixel, nextState);
    }
  }

}
void handle_new_bg_color(TokenStream* tokenStream, Pixel* pixel,
  State* nextState,  int row, int column) {

  Token* prevToken;

  /* Look back to previous token and check if it's color code */
  if ((column-1) >= 0) { /*boundary check */
    prevToken = tokenStream->tokens[row][column-1];
    if (prevToken->type == ALPHANUM_COLOR || prevToken->type == GRAPHICS_COLOR) {
      /* This also affects current pixel*/
      pixel->bgColor = nextState->bgColor = decode_color(prevToken->attribute);
    }
  }

  /* Do nothing if there's not a color code before and display bg as usual */
}

void handle_height_mode(Token* token, Pixel* pixel, State* currentState, State* nextState) {

  nextState->heightMode = decode_height_mode(token->attribute);

  /* Hold rule */
  if(currentState->holdMode == HOLD) {
    if (nextState->heightMode != currentState->heightMode) {
      release_graphics(pixel, nextState);
    }
  }
}

void handle_graphics_mode(Token* token, Pixel* pixel, State* currentState, State* nextState) {

  nextState->graphicsMode = decode_graphics_mode(token->attribute);
  nextState->outputMode = GRAPHICS;

  /* Hold rule */
  if(currentState->holdMode == HOLD) {
    if (nextState->graphicsMode != currentState->graphicsMode) {
      release_graphics(pixel, nextState);
    }
  }
}

void handle_hold_mode(Token* token, Pixel* pixel, State* currentState, State* nextState) {
  holdOption holdMode;

  holdMode = decode_hold_mode(token->attribute);

  switch (holdMode) {
    case HOLD:
    if (currentState->lastGraphic)  {
      set_pixel_to_hold(pixel, currentState);
      nextState->holdMode = HOLD;
    } else {
      nextState->holdMode = RELEASE;
    }
    break;
    case RELEASE:
    release_graphics(pixel, nextState);
    break;
  }
}

void set_pixel_to_hold(Pixel* pixel, State* currentState) {
  pixel->holdMode = HOLD;
  pixel->outputMode = GRAPHICS;
  pixel->graphicsMode = currentState->lastGraphicsMode;
  /* Allocate space and copy most recent graphic to pixel */
  pixel->blockGraphic = (Block_Graphic*) malloc(sizeof(Block_Graphic));
  if (pixel->blockGraphic == NULL) {
    ON_ERROR("Failed allocating memory for block graphic.");
  }
  memcpy(pixel->blockGraphic, currentState->lastGraphic, sizeof(Block_Graphic));
}

/* Affects both output and state */
void release_graphics(Pixel* pixel, State* state) {
  state->holdMode = RELEASE;
  state->lastGraphic = NULL;
  state->lastGraphicsMode = CONTIGUOUS;

  pixel->holdMode = RELEASE;
  pixel->graphicsMode = CONTIGUOUS;
  pixel->outputMode = TEXT;
  pixel->character = ' ';
  free(pixel->blockGraphic); /* have to watch this one */
  pixel->blockGraphic = NULL; /* need this to avoid double free */
}

void set_character_part(Teletext* teletext, Pixel* pixel, State* currentState, int row, int column) {
  /* Now determine which part of the character is displayed */
  switch (currentState->heightMode) {
    case SINGLE:
    pixel->charPart = WHOLE;
    break;
    case DOUBLE:
    /* Looks for the neighboor pixel vertically above */
    /* If it's double height, it displays the bottom half of this character */
    pixel->charPart = TOP_HALF;
    if ((row-1) >=0) { /* Boundary check */
      if (teletext->pixels[row-1][column].heightMode == DOUBLE) {
        pixel->charPart = BOTTOM_HALF;
      }
    }
    break;
  }
}

void set_pixel_properties(Pixel* pixel, State* state) {
  pixel->outputMode = state->outputMode;
  pixel->alphanumColor = state->alphanumColor;;
  pixel->graphicsColor = state->graphicsColor;
  pixel->bgColor = state->bgColor;
  pixel->heightMode = state->heightMode;
  pixel->graphicsMode = state->graphicsMode;
  pixel->holdMode = state->holdMode;
}

void reset_state(State* state) {
  state->lastGraphic = NULL;
  state->lastGraphicsMode = CONTIGUOUS;
  state->outputMode = TEXT;
  state->alphanumColor = WHITE;
  state->graphicsColor = WHITE;
  state->bgColor = BLACK;
  state->heightMode = SINGLE;
  state->graphicsMode = CONTIGUOUS;
  state->holdMode = RELEASE;
}

void debug_pixel(Pixel pixel) {
  printf("- Pixel -\n");
  printf("char: %c\n", pixel.character);
  printf("outputMode: %d\n", pixel.outputMode);
  printf("alphanumColor: %d\n", pixel.alphanumColor);
  printf("graphicsColor: %d\n", pixel.graphicsColor);
  printf("bgColor: %d\n", pixel.bgColor);
  printf("heightMode: %d\n", pixel.heightMode);
  printf("charPart: %d\n", pixel.charPart);
  printf("graphicsMode: %d\n", pixel.graphicsMode);
  printf("holdMode: %d\n", pixel.holdMode);
  printf("\n");
}
