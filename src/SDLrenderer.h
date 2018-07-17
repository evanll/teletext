/*
 * Written by Evan Lalopoulos <evan.lalopoulos.2017@my.bristol.ac.uk>
 * Copyright (C) 2018 - All rights reserved.
 * Unauthorized copying of this file is strictly prohibited.
 */

#ifndef SDLRENDERER_H_
#define SDLRENDERER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <assert.h>

#include "teletext.h"
#include "rgb_colors.h"
#include "neillsdl2.h"

#define DOUBLE_SCALE_FACTOR 2
#define FNTWIDTH_CON_SIXEL ((sizeof(fntrow)*8)/2)
#define FNTHEIGHT_CON_SIXEL (18/3)
/* Seperated sixels must be smaller to render correctly */
#define FNTWIDTH_SEP_SIXEL (((sizeof(fntrow)*8)/2)-1)
#define FNTHEIGHT_SEP_SIXEL ((18/3) -1)

/* Renders telextext using SDL */
void TLT_SDL_render(Teletext* teletext);
/* Based on neillsdl2 functions */
void TLT_SDL_DrawChar(SDL_Simplewin *sw, Pixel* pixel,
	fntrow fontdata[FNTCHARS][FNTHEIGHT], int ox, int oy);
void TLT_SDL_Draw(SDL_Simplewin *sw, Pixel* pixel,
  	int ox, int oy);
/* Nothing special, maps colors to rgb values  */
void color_to_rgb(SDL_Color* rgbColor, color clr);


#endif /* SDLRENDERER_H_ */
