/*
 * Written by Evan Lalopoulos <evan.lalopoulos.2017@my.bristol.ac.uk>
 * Copyright (C) 2018 - All rights reserved.
 * Unauthorized copying of this file is strictly prohibited.
 */

#include "SDLrenderer.h"

void TLT_SDL_render(Teletext* teletext) {
  SDL_Simplewin sw;
  char fontPath[] = "../source/m7fixed.fnt";
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  int i,j;
  int ox;
  int oy;

  Neill_SDL_Init(&sw);
  Neill_SDL_ReadFont(fontdata, fontPath);

  ox = 0;
  oy = 0;

  for(i=0; i<MAX_LINES; i++) {
    ox = 0;
    for(j=0; j<MAX_LENGTH; j++) {
      switch (teletext->pixels[i][j].outputMode) {
        case TEXT:
        TLT_SDL_DrawChar(&sw, &(teletext->pixels[i][j]), fontdata, ox, oy);
        break;
        case GRAPHICS:
        TLT_SDL_Draw(&sw, &(teletext->pixels[i][j]), ox, oy);
        break;
      }
      ox += FNTWIDTH;
    }
    oy += FNTHEIGHT;
  }

  Neill_SDL_UpdateScreen(&sw);

  do {
    Neill_SDL_Events(&sw);
  } while(!sw.finished);

  atexit(SDL_Quit);
}

void TLT_SDL_DrawChar(SDL_Simplewin *sw, Pixel* pixel,
  fntrow fontdata[FNTCHARS][FNTHEIGHT], int ox, int oy)
  {
    unsigned x, y, z;
    int scaleFactor;
    SDL_Color fgColor;
    SDL_Color bgColor;

    /* Colors to actual RGB */
    color_to_rgb(&fgColor, pixel->alphanumColor);
    color_to_rgb(&bgColor, pixel->bgColor);

    /* Determine Font Height */
    switch (pixel->heightMode) {
      case SINGLE:
      scaleFactor = 1;
      break;
      case DOUBLE:
      scaleFactor = DOUBLE_SCALE_FACTOR;
      break;
      default:
      scaleFactor = 1;
      break;
    }

    /* Determine which part of the character will be displayed */
    switch (pixel->charPart) {
      case WHOLE:
      case TOP_HALF:
      z = 0;
      break;
      case BOTTOM_HALF:
      z = FNTHEIGHT/DOUBLE_SCALE_FACTOR;
      break;
      default:
      z=0;
      break;
    }

    /* z is used to keep track which "row" of the font is rendered.
    * The idea is that depending on the scale factor each row is repeated
    * nth times.
    */
    for(y = 0; y < FNTHEIGHT; y++) {
      for(x = 0; x < FNTWIDTH; x++) {
        if(fontdata[pixel->character - FNT1STCHAR][z] >> (FNTWIDTH - 1 - x) & 1){
          Neill_SDL_SetDrawColour(sw, fgColor.r, fgColor.g, fgColor.b);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
        }
        else{
          Neill_SDL_SetDrawColour(sw, bgColor.r, bgColor.g, bgColor.b);
          SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
        }
      }
      /* Here's where it happens */
      if (y%scaleFactor == 0) {
        z++;
      }
    }
  }

  void TLT_SDL_Draw(SDL_Simplewin *sw, Pixel* pixel,
    int ox, int oy) {
      SDL_Rect rectangle;
      SDL_Color graphicsColor;
      int i, j, offset_x, offset_y;

      if(pixel->blockGraphic == NULL) {
        return;
      }

      color_to_rgb(&graphicsColor, pixel->graphicsColor);

      Neill_SDL_SetDrawColour(sw, graphicsColor.r, graphicsColor.g, graphicsColor.b);

      /* Each sixel is a rectangle */
      /* Seperated sixels are smaller to have space between each other */
      switch (pixel->graphicsMode) {
        case CONTIGUOUS:
        rectangle.w = FNTWIDTH_CON_SIXEL;
        rectangle.h = FNTHEIGHT_CON_SIXEL;
        break;
        case SEPERATED:
        rectangle.w = FNTWIDTH_SEP_SIXEL;
        rectangle.h = FNTHEIGHT_SEP_SIXEL;
        break;
      }

      offset_y = oy;
      for(i=0; i<SIXELS_Y; i++) {
        offset_x = ox;
        for (j=0; j<SIXELS_X; j++) {
          rectangle.x = offset_x;
          rectangle.y = offset_y;
          if (pixel->blockGraphic->sixels[i][j]) {
            SDL_RenderFillRect(sw->renderer, &rectangle);
          }
          offset_x += FNTWIDTH_CON_SIXEL;
        }
        offset_y += FNTHEIGHT_CON_SIXEL;
      }
    }

    void color_to_rgb(SDL_Color* rgbColor, color clr) {
      switch(clr) {
        case RED:
        rgbColor->r = RED_R;
        rgbColor->g = RED_G;
        rgbColor->b = RED_B;
        break;
        case GREEN:
        rgbColor->r = GREEN_R;
        rgbColor->g = GREEN_G;
        rgbColor->b = GREEN_B;
        break;
        case YELLOW:
        rgbColor->r = YELLOW_R;
        rgbColor->g = YELLOW_G;
        rgbColor->b = YELLOW_B;
        break;
        case BLUE:
        rgbColor->r = BLUE_R;
        rgbColor->g = BLUE_G;
        rgbColor->b = BLUE_B;
        break;
        case MAGENTA:
        rgbColor->r = MAGENTA_R;
        rgbColor->g = MAGENTA_G;
        rgbColor->b = MAGENTA_B;
        break;
        case CYAN:
        rgbColor->r = CYAN_R;
        rgbColor->g = CYAN_G;
        rgbColor->b = CYAN_B;
        break;
        case WHITE:
        rgbColor->r = WHITE_R;
        rgbColor->g = WHITE_G;
        rgbColor->b = WHITE_B;
        break;
        case BLACK:
        rgbColor->r = BLACK_R;
        rgbColor->g = BLACK_G;
        rgbColor->b = BLACK_B;
        break;
        default:
        rgbColor->r = BLACK_R;
        rgbColor->g = BLACK_G;
        rgbColor->b = BLACK_B;
        break;
      }
    }
