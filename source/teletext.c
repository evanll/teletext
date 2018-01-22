/*
* teletext.c
*
*      Author: Evan Lalopoulos
*/

#include "teletext.h"

Teletext* TLT_init(FILE* ifp) {
  TokenStream* tokenStream;
  Teletext* teletext;

  if (ifp == NULL) {
    ON_ERROR("Teletext file not found.\n");
  }

  /* First tokenize teletext file */
  tokenStream = TLT_tokenize_file(ifp);

  /* Then translate tokenstream to teletext*/
  teletext = TLT_translate(tokenStream);

  /* Tokenstream no longer needed. */
  TLT_tokenstream_free(&tokenStream);

  return teletext;
}

void TLT_free(Teletext** p) {
  int i, j;
  Teletext* teletext = *p;

  if (p==NULL) {
    return;
  }

  for(i=0; i<MAX_LINES; i++) {
    for(j=0; j<MAX_LENGTH; j++) {
      /* Free block graphics if they exist */
      if(teletext->pixels[i][j].blockGraphic) {
        free(teletext->pixels[i][j].blockGraphic);
      }
    }
  }

  free(teletext);
  *p = NULL;
}
