CC = gcc
CFLAGS = -Wall -Wextra 
LIBS = ./lib/pdcurses.a -lssh

tui: clay.c
	$(CC) $(CFLAGS) -o clay_executable clay.c $(LIBS)

test: test.c
	$(CC) $(CFLAGS) -o test test.c $(LIBS)

clean:
	rm -f clay_executable

cleantest:
	rm -f test
