/*
 * teletext.h
 *
 *      Author: Evan Lalopoulos
 */

#ifndef TELETEXT_H_
#define TELETEXT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "decoder.h"
#include "translator.h"
#include "SDLrenderer.h"

#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)

Teletext* TLT_init(FILE* ifp);
void TLT_free(Teletext** p);

#endif /* TELETEXT_H_ */
