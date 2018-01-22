CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
SRCPATH = ../source/

all: teletext_test

teletext_test:	teletext_test.c $(SRCPATH)teletext.c $(SRCPATH)tokenizer.c $(SRCPATH)decoder.c $(SRCPATH)translator.c $(SRCPATH)SDLrenderer.c $(SRCPATH)neillsdl2.c $(SRCPATH)teletext.h
	$(CC) teletext_test.c $(SRCPATH)teletext.c $(SRCPATH)tokenizer.c $(SRCPATH)decoder.c $(SRCPATH)translator.c $(SRCPATH)SDLrenderer.c $(SRCPATH)neillsdl2.c -o teletext_test -g -lm -O3 $(CFLAGS) `sdl2-config --cflags --libs`

run: all
	./teletext_test

clean:
	rm -f teletext_test

.PHONY: clean run all
