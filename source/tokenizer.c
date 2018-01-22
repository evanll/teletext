/*
 * tokenizer.c
 *
 *      Author: Evan Lalopoulos
 */
#include "tokenizer.h"

#define OFFSET_7_TO_8_BIT 0x80

TokenStream* TLT_tokenize_file(FILE* ifp) {
	int i,j;
  int c;
  TokenStream* tokenStream;

	if (ifp == NULL) {
    ON_ERROR("Teletext file not found.\n");
  }

  tokenStream = (TokenStream*) malloc(sizeof(TokenStream));
  if (tokenStream == NULL) {
    ON_ERROR("Failed creating TokenStream. Memory allocation failed.\n");
  }
  tokenStream->tokenCount = 0;

  for(i=0; i < MAX_LINES; i++) {
		/* Fill line with default a0 tokens in case file includes less than 1000 tokens*/
    memset(tokenStream->tokens[i], 0xa0, MAX_LENGTH);
    j = 0;
    while (j < MAX_LENGTH && (c = getc(ifp)) != EOF) {
			tokenStream->tokens[i][j] = tokenize(c);
      tokenStream->tokenCount++;
      tokenStream->tokens[i][j]->n = tokenStream->tokenCount;
			tokenStream->tokens[i][j]->line = i+1;
      j++;
    }
  }

  return tokenStream;
}

/* Tokenize input to basic control types */
Token* tokenize(char c) {
	Token* newToken;
	unsigned char ch;

	newToken = (Token*) malloc(sizeof(Token));
	if (newToken == NULL) {
    ON_ERROR("Failed creating token. Memory allocation failed.\n");
  }

	ch = (unsigned char) c;
	/* 7bit char to 8bit */
	if (ch < 0x80) {
		ch = ch + OFFSET_7_TO_8_BIT;
	}

	/* Tokenize */
	/* Also saves token type to string for debug */
	if (ch >= AN_COLOR_S && ch<=AN_COLOR_E) {
		newToken->type = ALPHANUM_COLOR;
		strcpy(newToken->str, "ALPHANUM_COLOR");
	} else if (ch == SINGLE_HEIGHT_CODE || ch == DOUBLE_HEIGHT_CODE) {
		newToken->type = HEIGHT_MODE;
		strcpy(newToken->str, "HEIGHT_MODE");
	} else if (ch >= GRAPHICS_COLOR_S && ch <= GRAPHICS_COLOR_E) {
		newToken->type = GRAPHICS_COLOR;
		strcpy(newToken->str, "GRAPHICS_COLOR");
	} else if (ch == CONT_GRAPHICS_CODE || ch == SEP_GRAPHICS_CODE) {
		newToken->type = GRAPHICS_MODE;
		strcpy(newToken->str, "GRAPHICS_MODE");
	} else if (ch == BLACK_BG_CODE) {
		newToken->type = BLACK_BG_COLOR;
		strcpy(newToken->str, "BLACK_BG_COLOR");
	} else if (ch == NEW_BG_CODE) {
		newToken->type = NEW_BG_COLOR;
		strcpy(newToken->str, "NEW_BG_COLOR");
	} else if (ch == HOLD_MODE_CODE || ch == RELEASE_MODE_CODE) {
		newToken->type = HOLD_MODE;
		strcpy(newToken->str, "HOLD_MODE");
	} else if (ch >= SYMBOL_RANGE_S) {
		newToken->type = SYMBOL;
		strcpy(newToken->str, "SYMBOL");
	} else {
		newToken->type = UNUSED;
		strcpy(newToken->str, "UNUSED");
	}

	newToken->attribute = ch;

	return newToken;
}

void TLT_tokenstream_free(TokenStream** p) {
  int i, j;
  TokenStream* tokenStream = *p;

  if (p==NULL) {
    return;
  }

	/* Free tokens */
  for(i=0; i<MAX_LINES; i++) {
    for(j=0; j<MAX_LENGTH; j++) {
      free(tokenStream->tokens[i][j]);
    }
  }

  free(tokenStream);
  *p = NULL;
}

void debug_token(Token token) {
	printf("Token: %d\n", token.n);
	printf("tokenLine: %d\n", token.line );
	printf("tokenAttr: %x\n", token.attribute);
	printf("tokenType: %s\n", token.str);
}
