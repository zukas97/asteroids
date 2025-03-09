LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf -ggdb
CFLAGS = -O2 -pipe
WINCLUDE = -I/usr/x86_64-w64-mingw32/include hello.c -L/usr/x86_64-w64-mingw32/lib
build:
	gcc -c src/main.c -ggdb $(CFLAGS) $(LIBS)
	gcc -c src/actions.c $(CFLAGS) $(LIBS)
	gcc main.o actions.o $(CFLAGS) $(LIBS) -o game
	rm *.o
run:
	make
	./game
	rm game
debug:
	make 
	gdb game
	rm game
clean:
	rm game
	rm *.o
windows:
	x86_64-w64-mingw32-gcc -c src/main.c $(WINCLUDE) $(CFLAGS) $(LIBS)
	x86_64-w64-mingw32-gcc -c src/actions.c $(WINCLUDE) $(CFLAGS) $(LIBS)
	x86_64-w64-mingw32-gcc main.o actions.o $(WINCLUDE) $(CFLAGS) $(LIBS) -ggdb -o game
	rm *.o
pkg-linux:
	tar -cJf asteroids-linux.tar.xz game images
	make clean
pkg-windows:
	Compress-Archive -DestinationPath asteroids-windows.zip -Path game images
	make clean

