CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi
INCS = teletext.h token.h tokenizer.h decoder.h translator.h SDLrenderer.h neillsdl2.h rgb_colors.h

all: teletext

teletext:	main.c teletext.c tokenizer.c decoder.c translator.c SDLrenderer.c neillsdl2.c $(INCS)
	$(CC) main.c teletext.c tokenizer.c decoder.c translator.c SDLrenderer.c neillsdl2.c -o teletext -g -lm -O3 $(CFLAGS) `sdl2-config --cflags --libs`

run: all
	./teletext samples/test.m7

lfc: teletext
	./teletext samples/lfc.m7

panda: teletext
	./teletext samples/panda.m7

clean:
	rm -f teletext

.PHONY: clean run all lfc panda
