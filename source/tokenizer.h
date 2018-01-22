/*
 * tokenizer.h
 *
 *      Author: Evan Lalopoulos
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "token.h"

#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

TokenStream* TLT_tokenize_file(FILE* ifp);
Token* tokenize(char c);
void TLT_tokenstream_free(TokenStream** p);
void debug_token(Token token);

#endif /* TOKENIZER_H_ */
