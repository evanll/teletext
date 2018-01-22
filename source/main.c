/*
* main.c
*
*      Author: Evan Lalopoulos
*/

#include <stdio.h>
#include <stdlib.h>
#include "teletext.h"

int main (int argc, char* argv[]) {
  FILE* ifp;
  Teletext* teletext;

  ifp = fopen(argv[1], "r");

  if (ifp == NULL) {
    printf("Argc %d\n", argc );
    ON_ERROR("Teletext file not found.\n");
  }

  teletext = TLT_init(ifp);
  fclose(ifp);
  
  /* Render teletext with SDL */
  TLT_SDL_render(teletext);
  TLT_free(&teletext);

  return 0;
}
