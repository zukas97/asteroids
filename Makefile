LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf 
CFLAGS = -O2 -pipe -ggdb
WINCLUDE = -I/usr/x86_64-w64-mingw32/include hello.c -L/usr/x86_64-w64-mingw32/lib
all: obj main clean
obj:
	gcc -c src/main.c  $(CFLAGS) $(LIBS)
	gcc -c src/actions.c $(CFLAGS) $(LIBS)
main:
	gcc main.o actions.o $(CFLAGS) $(LIBS) -o game
run:
	make
	./game
	rm game
debug:
	make 
	gdb game
	rm game
clean:
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

