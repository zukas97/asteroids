build:
	gcc src/main.c -ggdb -lSDL2 -lSDL2_image -o game
run:
	make
	./game
	rm game
debug:
	make 
	gdb game
	rm game
