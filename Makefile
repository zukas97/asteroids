LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf
.PHONY: build clean
build: build_objects build_main clean
build_objects:
	gcc -c src/main.c -ggdb $(LIBS)
	gcc -c src/actions.c $(LIBS)
.ONESHELL:
.NOTPARALLEL: build_main clean
build_main:
	gcc main.o actions.o -ggdb $(LIBS) -o game
clean:
	rm *.o
run:
	make
	./game
	rm game
debug:
	make 
	gdb game
	rm game

