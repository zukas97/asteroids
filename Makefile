LIBS = -lSDL2 -lSDL2_image 
build:
	gcc -c src/main.c -O3 -ggdb -march=native $(LIBS)
	gcc -c src/actions.c -O3 -march=native $(LIBS)
	gcc main.o actions.o -O3 -march=native $(LIBS) -ggdb -o game
	rm *.o
run:
	make
	./game
	rm game
debug:
	make 
	gdb game
	rm game

