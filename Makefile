LIBS = -lSDL2 -lSDL2_image 
CFLAGS = -O2 -pipe
build:
	gcc -c src/main.c -ggdb $(CFLAGS) $(LIBS)
	gcc -c src/actions.c $(CFLAGS) $(LIBS)
	gcc main.o actions.o $(CFLAGS) $(LIBS) -ggdb -o game
	rm *.o
run:
	make
	./game
	rm game
debug:
	make 
	gdb game
	rm game
pkg:
	tar -cJf asteroids-linux.tar.xz game images

