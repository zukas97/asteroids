build:
	gcc src/main.c -lSDL2 -lSDL2_image -o game
run:
	make
	./game
	rm game
