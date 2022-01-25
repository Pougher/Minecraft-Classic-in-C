CC=gcc
CFLAGS=-O2 -Llib -lglfw3 -lGLEW  -framework Cocoa -framework IOKit -framework OpenGL -o bin/game -ferror-limit=69420
bin/minecraft: src/main.c
	$(CC) src/main.c $(CFLAGS)
run:
	cd bin && ./game