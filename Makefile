build:
	#gcc  src/main.c -ggdb -lSDL2 -lSDL2_image -o game
	gcc -c src/main.c -ggdb -lSDL2 -lSDL2_image
	gcc -c src/actions.c -lSDL2
	gcc main.o actions.o -ggdb -lSDL2 -lSDL2_image -o game
	rm *.o
run:
	make
	./game
	rm game
debug:
	make 
	gdb game
	rm game

