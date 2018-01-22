/*
* test.c
*
*      Author: Evan Lalopoulos
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../source/teletext.h"

/* Function to quickly load test cases */
TokenStream* TLT_load_test(unsigned char line1[MAX_LENGTH],
  unsigned char line2[MAX_LENGTH]);
  /* Tests that the correct sixels are lit */
  void test_block_graphics();
  /* A variety of cases to test translator logic */
  void test_alphanum_mode();
  void test_graphics_and_breakthrough_mode();
  void test_height_mode();
  /* Tests hold mode logic with multiple cases  */
  /* Also checks that the corrrect graphics are held each time */
  void test_hold_mode_a();
  void test_hold_mode_b();
  void test_hold_mode_c();
  void test_hold_mode_d();
  void test_hold_mode_e();
  /* Tests that basic token types are correctly identified */
  void test_tokenizer();

  int main () {

    printf("Starting Tests...\n");
    test_block_graphics();
    test_alphanum_mode();
    test_graphics_and_breakthrough_mode();
    test_height_mode();
    test_hold_mode_a();
    test_hold_mode_b();
    test_hold_mode_c();
    test_hold_mode_d();
    test_hold_mode_e();
    test_tokenizer();
    printf("Tests completed\n");

    return 0;
  }

  void test_block_graphics() {
    Block_Graphic* blockGraphic;

    blockGraphic = init_block_graphic(0xa0);
    /* first row */
    assert(blockGraphic->sixels[0][0]==0);
    assert(blockGraphic->sixels[0][1]==0);
    /* middle row */
    assert(blockGraphic->sixels[1][0]==0);
    assert(blockGraphic->sixels[1][1]==0);
    /* last row */
    assert(blockGraphic->sixels[2][0]==0);
    assert(blockGraphic->sixels[2][1]==0);
    free(blockGraphic);

    blockGraphic = init_block_graphic(0xff);
    /* first row */
    assert(blockGraphic->sixels[0][0]==1);
    assert(blockGraphic->sixels[0][1]==1);
    /* middle row */
    assert(blockGraphic->sixels[1][0]==1);
    assert(blockGraphic->sixels[1][1]==1);
    /* last row */
    assert(blockGraphic->sixels[2][0]==1);
    assert(blockGraphic->sixels[2][1]==1);
    free(blockGraphic);

    blockGraphic = init_block_graphic(0xb3);
    /* first row */
    assert(blockGraphic->sixels[0][0]==1);
    assert(blockGraphic->sixels[0][1]==1);
    /* middle row */
    assert(blockGraphic->sixels[1][0]==0);
    assert(blockGraphic->sixels[1][1]==0);
    /* last row */
    assert(blockGraphic->sixels[2][0]==1);
    assert(blockGraphic->sixels[2][1]==0);
    free(blockGraphic);

    blockGraphic = init_block_graphic(0xb3);
    /* first row */
    assert(blockGraphic->sixels[0][0]==1);
    assert(blockGraphic->sixels[0][1]==1);
    /* middle row */
    assert(blockGraphic->sixels[1][0]==0);
    assert(blockGraphic->sixels[1][1]==0);
    /* last row */
    assert(blockGraphic->sixels[2][0]==1);
    assert(blockGraphic->sixels[2][1]==0);
    free(blockGraphic);

    blockGraphic = init_block_graphic(0xee);
    /* first row */
    assert(blockGraphic->sixels[0][0]==0);
    assert(blockGraphic->sixels[0][1]==1);
    /* middle row */
    assert(blockGraphic->sixels[1][0]==1);
    assert(blockGraphic->sixels[1][1]==1);
    /* last row */
    assert(blockGraphic->sixels[2][0]==0);
    assert(blockGraphic->sixels[2][1]==1);
    free(blockGraphic);

    printf("Block Graphics Test: Passed\n");
  }

  void test_alphanum_mode() {
    Teletext* teletext;
    TokenStream* tokenStream;
    int i;
    unsigned char emptyLine[MAX_LENGTH];
    unsigned char line1[MAX_LENGTH];

    line1[0] = 0xb0; /*0 1 2 3*/
    line1[1] = 0xb1;
    line1[2] = 0xb2;
    line1[3] = 0xb3;
    line1[4] = 0x81; /* Red alphanum*/
    line1[5] = 0xc1; /* ABC */
    line1[6] = 0xc2;
    line1[7] = 0xc3;
    line1[8] = 0x84; /* Blue alphanum */
    line1[9] = 0xd7; /*wyx */
    line1[10] = 0xd8;
    line1[11] = 0xd9;

    /* Fill rest of line with 0xa0 */
    memset((line1+12), 0xa0, MAX_LENGTH - 12);
    /* Fill empty line with 0xa0 */
    memset(emptyLine, 0xa0, MAX_LENGTH);
    /* Initiliaze teletext */
    tokenStream = TLT_load_test(line1, emptyLine);
    teletext = TLT_translate(tokenStream);

    /* Check that line starts with white text and black bg*/
    for(i=0; i<3; i++) {
      assert(teletext->pixels[0][i].outputMode == TEXT);
      assert(teletext->pixels[0][i].alphanumColor == WHITE);
      assert(teletext->pixels[0][i].bgColor == BLACK);
    }

    /* Check swith to red text to rest of the line after issuing code */
    for(i=5; i<=7; i++) {
      assert(teletext->pixels[0][i].outputMode == TEXT);
      assert(teletext->pixels[0][i].alphanumColor == RED);
      assert(teletext->pixels[0][i].bgColor == BLACK);
    }

    /* Check swith to blue text to rest of the line after issuing code */
    for(i=9; i<=11; i++) {
      assert(teletext->pixels[0][i].outputMode == TEXT);
      assert(teletext->pixels[0][i].alphanumColor == BLUE);
      assert(teletext->pixels[0][i].bgColor == BLACK);
    }
    /* Uncomment to render case */
    /*TLT_SDL_render(teletext);*/

    free(tokenStream);
    TLT_free(&teletext);
    assert(teletext==NULL);

    printf("Colored Text: PASSED.\n");
  }

  void test_graphics_and_breakthrough_mode() {
    Teletext* teletext;
    TokenStream* tokenStream;
    int i;
    unsigned char emptyLine[MAX_LENGTH];
    unsigned char line1[MAX_LENGTH];

    line1[0] = 0xb0; /* 0 1 2 3 */
    line1[1] = 0xb1;
    line1[2] = 0xb2;
    line1[3] = 0xb3;
    line1[4] = 0x92; /* Green graphics */
    line1[5] = 0xb5; /* ABC */
    line1[6] = 0xff;
    line1[7] = 0xab;
    line1[8] = 0x9a; /* Seperated graphics */
    line1[9] = 0xb5; /*Graphics */
    line1[10] = 0xff;
    line1[11] = 0xab;
    line1[12] = 0xb8;
    line1[13] = 0xd7; /* WYZ must be Breaktrhough*/
    line1[14] = 0xd8;
    line1[15] = 0xd9;
    line1[16] = 0xa9; /* grapphic */

    /* Fill rest of line with 0xa0 */
    memset((line1+17), 0xa0, MAX_LENGTH - 17);
    /* Fill empty line with 0xa0 */
    memset(emptyLine, 0xa0, MAX_LENGTH);
    /* Initiliaze teletext */
    tokenStream = TLT_load_test(line1, emptyLine);
    teletext = TLT_translate(tokenStream);

    /* Check that line starts with white text*/
    for(i=0; i<=3; i++) {
      assert(teletext->pixels[0][i].outputMode == TEXT);
    }
    /* Issuing green graphics */
    /* Assert control is displayed as text */
    assert(teletext->pixels[0][4].outputMode == TEXT);

    for(i=5; i<=7; i++) {
      assert(teletext->pixels[0][i].outputMode == GRAPHICS);
      assert(teletext->pixels[0][i].graphicsMode == CONTIGUOUS);
      assert(teletext->pixels[0][i].graphicsColor == GREEN);
    }
    /* Issuing seperated graphics*/
    /* Assert control is displayed as text */
    assert(teletext->pixels[0][8].outputMode == TEXT);

    for(i=9; i<=12; i++) {
      assert(teletext->pixels[0][i].outputMode == GRAPHICS);
      assert(teletext->pixels[0][i].graphicsMode == SEPERATED);
      assert(teletext->pixels[0][i].graphicsColor == GREEN);
    }

    printf("Graphic Modes Test: PASSED.\n");

    /* Breaktrhough */
    assert(teletext->pixels[0][13].character = 'W');
    assert(teletext->pixels[0][14].character = 'Y');
    assert(teletext->pixels[0][15].character = 'Z');
    /* Breaktrhough output and color */
    for(i=13; i<=15; i++) {
      assert(teletext->pixels[0][i].outputMode == TEXT);
      assert(teletext->pixels[0][i].alphanumColor == GREEN);
    }

    /* Uncomment to render case */
    /*TLT_SDL_render(teletext);*/

    free(tokenStream);
    TLT_free(&teletext);
    assert(teletext==NULL);

    printf("Breakthrough Test: PASSED.\n");
  }

  void test_height_mode() {
    Teletext* teletext;
    TokenStream* tokenStream;
    int i;
    unsigned char line1[MAX_LENGTH];
    unsigned char line2[MAX_LENGTH];

    /* Line 1 */
    line1[0] = 0xb0; /* 0 1 2 3 */
    line1[1] = 0xb1;
    line1[2] = 0xb2;
    line1[3] = 0xb3;
    line1[4] = 0x86; /* Cyan alphanumeric */
    line1[5] = 0x8d; /* <------ Double height */
    line1[6] = 0xeb; /* k l m*/
    line1[7] = 0xec;
    line1[8] = 0xee;
    line1[9] = 0x8c; /* <------ Single Height */
    line1[10] = 0xe1; /* abc */
    line1[11] = 0xe2;
    line1[12] = 0xe3;
    /* Fill rest of line with 0xa0 */
    memset((line1+13), 0xa0, MAX_LENGTH - 13);

    /* Line 2 */
    line2[0] = 0xb0; /* 0 1 2 3 */
    line2[1] = 0xb1;
    line2[2] = 0xb2;
    line2[3] = 0xb3;
    line2[4] = 0x83; /* Yellow alphanumeric */
    line2[5] = 0xa0;
    line2[6] = 0xa0;
    line2[7] = 0x8d; /* <------ Double height */
    line2[8] = 0xf0; /* pqrs */
    line2[9] = 0xf1;
    line2[10] = 0xf3;
    line2[11] = 0xf4;
    line2[12] = 0x8c; /* <------ Single Height */
    line2[13] = 0xe1; /* abc */
    line2[14] = 0xe2;
    line2[15] = 0xe3;
    /* Fill rest of line with 0xa0 */
    memset((line2+16), 0xa0, MAX_LENGTH - 16);

    /* Initiliaze teletext */
    tokenStream = TLT_load_test(line1, line2);
    teletext = TLT_translate(tokenStream);

    /* Test 3 Check switch from single to height and double height on vertical neighboors */

    /* Check first line */
    for(i=0; i<=4; i++) {
      assert(teletext->pixels[0][i].heightMode == SINGLE);
      assert(teletext->pixels[0][i].charPart == WHOLE);
    }

    assert(teletext->pixels[0][6].heightMode == DOUBLE);
    assert(teletext->pixels[0][7].heightMode == DOUBLE);
    assert(teletext->pixels[0][8].heightMode == DOUBLE);
    assert(teletext->pixels[0][6].charPart == TOP_HALF);
    assert(teletext->pixels[0][7].charPart == TOP_HALF);
    assert(teletext->pixels[0][8].charPart == TOP_HALF);

    for(i=10; i<=12; i++) {
      assert(teletext->pixels[0][i].heightMode == SINGLE);
      assert(teletext->pixels[0][i].charPart == WHOLE);
    }

    /* Second line is tricky, because not all double heights have matching neighboors */
    for(i=0; i<=6; i++) {
      assert(teletext->pixels[1][i].heightMode == SINGLE);
      assert(teletext->pixels[1][i].charPart == WHOLE);
    }

    assert(teletext->pixels[1][8].heightMode == DOUBLE);
    assert(teletext->pixels[1][9].heightMode == DOUBLE);
    assert(teletext->pixels[1][10].heightMode == DOUBLE);
    assert(teletext->pixels[1][11].heightMode == DOUBLE);
    assert(teletext->pixels[1][8].charPart == BOTTOM_HALF); /* <- This must be printed as bottom half */
    assert(teletext->pixels[1][9].charPart == TOP_HALF); /* the rest don't have neighboors, so top half */
    assert(teletext->pixels[1][10].charPart == TOP_HALF);
    assert(teletext->pixels[1][11].charPart == TOP_HALF);

    for(i=13; i<=15; i++) {
      assert(teletext->pixels[1][i].heightMode == SINGLE);
      assert(teletext->pixels[1][i].charPart == WHOLE);
    }

    /* Uncomment to render case */
    /*TLT_SDL_render(teletext);*/

    free(tokenStream);
    TLT_free(&teletext);
    assert(teletext==NULL);

    printf("Height mode test: PASSED.\n");

  }

  void test_hold_mode_a() {
    Teletext* teletext;
    TokenStream* tokenStream;
    unsigned char emptyLine[MAX_LENGTH];
    unsigned char line1[MAX_LENGTH];
    int i,j;

    line1[0] = 0x82; /* Green alphanum*/
    line1[1] = 0xd4; /* t e s t */
    line1[2] = 0xe5;
    line1[3] = 0xf3;
    line1[4] = 0xf4;
    line1[5] = 0x93; /* Yellow graphics */
    line1[6] = 0xa6; /* Graphic <---- */
    line1[7] = 0x9e; /* Hold */
    line1[8] = 0x91; /* Green graphics */
    line1[9] = 0x94; /* Blue graphics */
    line1[10] = 0x9f; /* Relesse */
    line1[11] = 0x95; /*Magenta graphics */
    line1[12] = 0xa9; /* Graphic */

    /* Fill rest of line with 0xa0 */
    memset((line1+13), 0xa0, MAX_LENGTH - 13);
    /* Fill empty line with 0xa0 */
    memset(emptyLine, 0xa0, MAX_LENGTH);
    /* Initiliaze teletext */
    tokenStream = TLT_load_test(line1, emptyLine);
    teletext = TLT_translate(tokenStream);

    assert(teletext->pixels[0][7].holdMode == HOLD);
    assert(teletext->pixels[0][8].holdMode == HOLD);
    assert(teletext->pixels[0][9].holdMode == HOLD);
    assert(teletext->pixels[0][10].holdMode == RELEASE);
    assert(teletext->pixels[0][11].holdMode == RELEASE);
    assert(teletext->pixels[0][12].holdMode == RELEASE);

    /* Assert that the correct graphic [6] is holded */

    /*memcmp not always working for structures???
    assert(memcmp(teletext->pixels[0][6].blockGraphic,
    teletext->pixels[0][7].blockGraphic, sizeof(Block_Graphic)));
    assert(memcmp(teletext->pixels[0][6].blockGraphic,
    teletext->pixels[0][8].blockGraphic, sizeof(Block_Graphic)));
    assert(memcmp(teletext->pixels[0][6].blockGraphic,
    teletext->pixels[0][9].blockGraphic, sizeof(Block_Graphic)));
    */

    /* Anyway I'll use loops */
    /* Assert graphic blocks is the same */
    for(i=0; i<6; i++) {
      for(j=0; j<6; j++) {
        assert(teletext->pixels[0][7].blockGraphic->sixels[i][j] ==
          teletext->pixels[0][8].blockGraphic->sixels[i][j]);
          assert(teletext->pixels[0][7].blockGraphic->sixels[i][j] ==
            teletext->pixels[0][9].blockGraphic->sixels[i][j]);
          }
        }

        /* Uncomment to render case */
        /*TLT_SDL_render(teletext);*/

        free(tokenStream);
        TLT_free(&teletext);
        assert(teletext==NULL);

        printf("Hold Mode Test A: PASSED.\n");
      }

      void test_hold_mode_b() {
        Teletext* teletext;
        TokenStream* tokenStream;
        unsigned char emptyLine[MAX_LENGTH];
        unsigned char line1[MAX_LENGTH];
        int i,j;

        line1[0] = 0x82; /* Green alphanum*/
        line1[1] = 0xd4; /* t e s t */
        line1[2] = 0xe5;
        line1[3] = 0xf3;
        line1[4] = 0xf4;
        line1[5] = 0x83; /* yellow alphanum*/
        line1[6] = 0x94; /* blue graphics*/
        line1[7] = 0xe5; /* graphic */
        line1[8] = 0x9e; /* Hold */
        line1[9] = 0x91; /* red graphics */
        line1[10] = 0xb7; /* graphic */
        line1[11] = 0x92; /* green graphics */
        line1[12] = 0xfa; /* graphic */
        line1[13] = 0x9a; /* seperated */
        line1[14] = 0xfb; /* graphic */
        line1[15] = 0x9c; /* black bg */
        line1[16] = 0xba; /* Graphic */

        /* Fill rest of line with 0xa0 */
        memset((line1+17), 0xa0, MAX_LENGTH - 17);
        /* Fill empty line with 0xa0 */
        memset(emptyLine, 0xa0, MAX_LENGTH);
        /* Initiliaze teletext */
        tokenStream = TLT_load_test(line1, emptyLine);
        teletext = TLT_translate(tokenStream);

        assert(teletext->pixels[0][8].holdMode == HOLD);
        assert(teletext->pixels[0][9].holdMode == HOLD);
        assert(teletext->pixels[0][11].holdMode == HOLD);
        assert(teletext->pixels[0][13].holdMode == RELEASE); /* sepereated should released */
        assert(teletext->pixels[0][15].holdMode == RELEASE);

        /* Assert graphic blocks is the same */
        for(i=0; i<6; i++) {
          for(j=0; j<6; j++) {
            assert(teletext->pixels[0][7].blockGraphic->sixels[i][j] ==
              teletext->pixels[0][8].blockGraphic->sixels[i][j]);
              assert(teletext->pixels[0][7].blockGraphic->sixels[i][j] ==
                teletext->pixels[0][9].blockGraphic->sixels[i][j]);
                assert(teletext->pixels[0][10].blockGraphic->sixels[i][j] ==
                  teletext->pixels[0][11].blockGraphic->sixels[i][j]);
                }
              }

              /* Uncomment to render case */
              /*TLT_SDL_render(teletext);*/

              free(tokenStream);
              TLT_free(&teletext);
              assert(teletext==NULL);

              printf("Hold Mode Test B: PASSED.\n");
            }

            void test_hold_mode_c() {
              Teletext* teletext;
              TokenStream* tokenStream;
              unsigned char emptyLine[MAX_LENGTH];
              unsigned char line1[MAX_LENGTH];
              int i,j;

              line1[0] = 0x82; /* Green alphanum*/
              line1[1] = 0xd4; /* t e s t */
              line1[2] = 0xe5;
              line1[3] = 0xf3;
              line1[4] = 0xf4;
              line1[5] = 0x83; /* yellow alphanum*/
              line1[6] = 0x94; /* blue graphics*/
              line1[7] = 0xe5; /* graphic */
              line1[8] = 0x9e; /* Hold */
              line1[9] = 0x91; /* red graphics */
              line1[10] = 0xb7; /* graphic */
              line1[11] = 0x92; /* green graphics */
              line1[12] = 0xfa; /* graphic */
              line1[13] = 0x81; /* alphanum */
              line1[14] = 0xfb; /* text */
              line1[15] = 0x9c; /* black bg */
              line1[16] = 0xba; /* Graphic */

              /* Fill rest of line with 0xa0 */
              memset((line1+17), 0xa0, MAX_LENGTH - 17);
              /* Fill empty line with 0xa0 */
              memset(emptyLine, 0xa0, MAX_LENGTH);
              /* Initiliaze teletext */
              tokenStream = TLT_load_test(line1, emptyLine);
              teletext = TLT_translate(tokenStream);

              assert(teletext->pixels[0][8].holdMode == HOLD);
              assert(teletext->pixels[0][9].holdMode == HOLD);
              assert(teletext->pixels[0][11].holdMode == HOLD);
              assert(teletext->pixels[0][13].holdMode == RELEASE); /* sepereated should released */
              assert(teletext->pixels[0][15].holdMode == RELEASE);

              /* Assert graphic blocks is the same */
              for(i=0; i<6; i++) {
                for(j=0; j<6; j++) {
                  assert(teletext->pixels[0][7].blockGraphic->sixels[i][j] ==
                    teletext->pixels[0][8].blockGraphic->sixels[i][j]);
                    assert(teletext->pixels[0][7].blockGraphic->sixels[i][j] ==
                      teletext->pixels[0][9].blockGraphic->sixels[i][j]);
                      assert(teletext->pixels[0][10].blockGraphic->sixels[i][j] ==
                        teletext->pixels[0][11].blockGraphic->sixels[i][j]);
                      }
                    }

                    /* Uncomment to render case */
                    /*TLT_SDL_render(teletext);*/

                    free(tokenStream);
                    TLT_free(&teletext);
                    assert(teletext==NULL);

                    printf("Hold Mode Test C: PASSED.\n");
                  }

                  void test_hold_mode_d() {
                    Teletext* teletext;
                    TokenStream* tokenStream;
                    unsigned char emptyLine[MAX_LENGTH];
                    unsigned char line1[MAX_LENGTH];
                    int i,j;

                    line1[0] = 0x82; /* Green alphanum*/
                    line1[1] = 0xd4; /* t e s t */
                    line1[2] = 0xe5;
                    line1[3] = 0xf3;
                    line1[4] = 0xf4;
                    line1[5] = 0x83; /* yellow alphanum*/
                    line1[6] = 0x94; /* blue graphics*/
                    line1[7] = 0xe5; /* graphic */
                    line1[8] = 0x9e; /* Hold */
                    line1[9] = 0x91; /* red graphics */
                    line1[10] = 0xb7; /* graphic */
                    line1[11] = 0x92; /* green graphics */
                    line1[12] = 0xfa; /* graphic */
                    line1[13] = 0x8d; /* double height */
                    line1[14] = 0xfb; /* text */
                    line1[15] = 0x9c; /* black bg */
                    line1[16] = 0xba; /* Graphic */

                    /* Fill rest of line with 0xa0 */
                    memset((line1+17), 0xa0, MAX_LENGTH - 17);
                    /* Fill empty line with 0xa0 */
                    memset(emptyLine, 0xa0, MAX_LENGTH);
                    /* Initiliaze teletext */
                    tokenStream = TLT_load_test(line1, emptyLine);
                    teletext = TLT_translate(tokenStream);

                    assert(teletext->pixels[0][8].holdMode == HOLD);
                    assert(teletext->pixels[0][9].holdMode == HOLD);
                    assert(teletext->pixels[0][11].holdMode == HOLD);
                    assert(teletext->pixels[0][13].holdMode == RELEASE); /* sepereated should released */
                    assert(teletext->pixels[0][15].holdMode == RELEASE);

                    /* Assert graphic blocks is the same */
                    for(i=0; i<6; i++) {
                      for(j=0; j<6; j++) {
                        assert(teletext->pixels[0][7].blockGraphic->sixels[i][j] ==
                          teletext->pixels[0][8].blockGraphic->sixels[i][j]);
                          assert(teletext->pixels[0][7].blockGraphic->sixels[i][j] ==
                            teletext->pixels[0][9].blockGraphic->sixels[i][j]);
                            assert(teletext->pixels[0][10].blockGraphic->sixels[i][j] ==
                              teletext->pixels[0][11].blockGraphic->sixels[i][j]);
                            }
                          }

                          /* Uncomment to render case */
                          /*TLT_SDL_render(teletext);*/

                          free(tokenStream);
                          TLT_free(&teletext);
                          assert(teletext==NULL);

                          printf("Hold Mode Test D: PASSED.\n");
                        }

                        /* This doesn't have a graphic before */
                        void test_hold_mode_e() {
                          Teletext* teletext;
                          TokenStream* tokenStream;
                          unsigned char emptyLine[MAX_LENGTH];
                          unsigned char line1[MAX_LENGTH];

                          line1[0] = 0x82; /* Green alphanum*/
                          line1[1] = 0xd4; /* t e s t */
                          line1[2] = 0xe5;
                          line1[3] = 0xf3;
                          line1[4] = 0xf4;
                          line1[5] = 0x83; /* yellow alphanum*/
                          line1[6] = 0x94; /* blue graphics*/
                          line1[7] = 0xe5; /* graphic */
                          line1[8] = 0x9e; /* Hold */
                          line1[9] = 0x91; /* red graphics */
                          line1[10] = 0x9f; /* release */
                          line1[11] = 0x9e; /* hold <---- */
                          line1[12] = 0x82; /* Green alphanum*/
                          line1[13] = 0xd4; /* t e s t */
                          line1[14] = 0xe5;
                          line1[15] = 0xf3;
                          line1[16] = 0xf4;

                          /* Fill rest of line with 0xa0 */
                          memset((line1+17), 0xa0, MAX_LENGTH - 17);
                          /* Fill empty line with 0xa0 */
                          memset(emptyLine, 0xa0, MAX_LENGTH);
                          /* Initiliaze teletext */
                          tokenStream = TLT_load_test(line1, emptyLine);
                          teletext = TLT_translate(tokenStream);

                          assert(teletext->pixels[0][8].holdMode == HOLD);
                          assert(teletext->pixels[0][9].holdMode == HOLD);
                          assert(teletext->pixels[0][10].holdMode == RELEASE);
                          assert(teletext->pixels[0][11].holdMode == RELEASE); /* sepereated should released */
                          assert(teletext->pixels[0][12].holdMode == RELEASE);

                          /* Uncomment to render case */
                          /*TLT_SDL_render(teletext);*/

                          free(tokenStream);
                          TLT_free(&teletext);
                          assert(teletext==NULL);

                          printf("Hold Mode Test E: PASSED.\n");
                        }

                        void test_tokenizer() {
                          TokenStream* tokenStream;
                          unsigned char emptyLine[MAX_LENGTH];
                          unsigned char line1[MAX_LENGTH];

                          line1[0] = 0x81; /* alphanumColor */
                          line1[1] = 0x8c; /* single height */
                          line1[2] = 0x8d;  /* double height */
                          line1[3] = 0x94; /* graphics color */
                          line1[4] = 0x99; /* graphics mode */
                          line1[5] = 0x9a; /* graphics mode */
                          line1[6] = 0x9c; /* black background */
                          line1[7] = 0x9d;  /* new background */
                          line1[8] = 0x9e; /* hold mode */
                          line1[9] = 0x9f; /* hold mode */
                          line1[10] = 0xb2; /* symbol */
                          line1[11] = 0x80; /* unused */
                          line1[12] = 0x45; /* <--- less than 0x80. */

                          /* Fill rest of line with 0xa0 */
                          memset((line1+13), 0xa0, MAX_LENGTH - 13);
                          /* Fill empty line with 0xa0 */
                          memset(emptyLine, 0xa0, MAX_LENGTH);
                          /* Initiliaze teletext */
                          tokenStream = TLT_load_test(line1, emptyLine);

                          assert(tokenStream->tokens[0][0]->type == ALPHANUM_COLOR);
                          assert(tokenStream->tokens[0][1]->type == HEIGHT_MODE);
                          assert(tokenStream->tokens[0][2]->type == HEIGHT_MODE);
                          assert(tokenStream->tokens[0][3]->type == GRAPHICS_COLOR);
                          assert(tokenStream->tokens[0][4]->type == GRAPHICS_MODE);
                          assert(tokenStream->tokens[0][5]->type == GRAPHICS_MODE);
                          assert(tokenStream->tokens[0][6]->type == BLACK_BG_COLOR);
                          assert(tokenStream->tokens[0][7]->type == NEW_BG_COLOR);
                          assert(tokenStream->tokens[0][8]->type == HOLD_MODE);
                          assert(tokenStream->tokens[0][9]->type == HOLD_MODE);
                          assert(tokenStream->tokens[0][10]->type == SYMBOL);
                          assert(tokenStream->tokens[0][11]->type == UNUSED);
                          assert(tokenStream->tokens[0][12]->type == SYMBOL); /* must be E */
                          assert(tokenStream->tokens[0][12]->attribute == 0xc5); /* assert that 0x80 has been added*/

                          printf("Tokenizer Test: PASSED.\n");
                        }

                        TokenStream* TLT_load_test(unsigned char line1[MAX_LENGTH],
                          unsigned char line2[MAX_LENGTH]) {
                            int i,j;
                            TokenStream* tokenStream;
                            unsigned char defaultToken = 0xa0;

                            /* Init teletext and set number of lines to 0 */
                            tokenStream = (TokenStream*) malloc(sizeof(TokenStream));
                            if (tokenStream == NULL) {
                              ON_ERROR("Failed creating TokenStream. Memory allocation failed.\n");
                            }

                            tokenStream->tokenCount = 0;

                            /* Add line 1*/
                            for (i=0; i < MAX_LENGTH; i++) {
                              tokenStream->tokens[0][i] = tokenize(line1[i]);
                              tokenStream->tokenCount++;
                              tokenStream->tokens[0][i]->n = tokenStream->tokenCount;
                              tokenStream->tokens[0][i]->line = 0;
                            }
                            /* Add line 2 */
                            for (i=0; i < MAX_LENGTH; i++) {
                              tokenStream->tokens[1][i] = tokenize(line2[i]);
                              tokenStream->tokenCount++;
                              tokenStream->tokens[1][i]->n = tokenStream->tokenCount;
                              tokenStream->tokens[1][i]->line = 0;
                            }

                            /* Fill the teletext with a0 tokens */
                            for(i=2; i < MAX_LINES; i++) {
                              for (j=0; j < MAX_LENGTH; j++) {
                                tokenStream->tokens[i][j] = tokenize(defaultToken);
                                tokenStream->tokenCount++;
                                tokenStream->tokens[i][j]->n = tokenStream->tokenCount;
                                tokenStream->tokens[i][j]->line = i+1;
                              }
                            }

                            return tokenStream;
                          }
