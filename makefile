#
#This file is part of game called CURSNAKE.
#
#Cursnake is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#Cursnake is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with Cursnake.  If not, see <http://www.gnu.org/licenses/>.

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


