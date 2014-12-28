all: read

read:   read.c
	gcc -Wall `pkg-config --cflags --libs libmodbus hiredis` -o read read.c

clean:
	rm -rf *.o read
