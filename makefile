CC = gcc
CFLAGS = -Wall -Wextra 
# For Windows
windows: clay-windows.c
	$(CC) $(CFLAGS) -o clay_executable_windows clay-windows.c -lssh -luser32 ./lib/pdcurses.a
# For Linux
linux: clay-linux.c
	$(CC) $(CFLAGS) -o clay_executable_linux clay-linux.c -lssh -lncurses
# Test program
test: test.c
	$(CC) $(CFLAGS) -o test test.c -lssh
clean:
	rm -f clay_executable_windows clay_executable_linux test