CFLAGS=-std=c99 -Wall -pedantic -W -g 
PROG=had
DEBUG=
DEBUG=-DDEBUG

all: $(PROG)

main.o: main.c	main.h snake.h
	gcc -c $(CFLAGS) $(DEBUG) main.c

snake.o: snake.c snake.h
	gcc -c $(CFLAGS) $(DEBUG) snake.c

arena.o: arena.c arena.h
	gcc -c $(CFLAGS) $(DEBUG) arena.c

common.o: common.c common.h
	gcc -c $(CFLAGS) $(DEBUG) common.c


had: snake.o main.o arena.o common.o makefile
	gcc -o $(PROG) -lcurses snake.o main.o arena.o common.o

clean:
	rm -vf $(PROG) ./*.o

