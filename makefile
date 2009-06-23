CFLAGS=-std=c99 -Wall -pedantic -W -g 
PROG=had
LOG=log
DEBUG=
DEBUG=-DDEBUG
SRC=main.c main.h snake.h snake.c arena.h arena.c common.c common.h

all: $(PROG)

main.o: main.c	main.h snake.h makefile
	gcc -c $(CFLAGS) $(DEBUG) main.c

snake.o: snake.c snake.h makefile
	gcc -c $(CFLAGS) $(DEBUG) snake.c

arena.o: arena.c arena.h makefile
	gcc -c $(CFLAGS) $(DEBUG) arena.c

common.o: common.c common.h makefile
	gcc -c $(CFLAGS) $(DEBUG) common.c


had: snake.o main.o arena.o common.o makefile
	gcc -o $(PROG) -lcurses snake.o main.o arena.o common.o

clean:
	rm -vf $(PROG) $(LOG) ./*.o $(PROG).tar.gz

tgz: $(SRC) makefile credits.txt
	echo making source tarball
	tar -cvzf $(PROG).tar.gz $(SRC) makefile credits.txt .git COPYING


