CC=gcc
CFLAGS=-I/usr/include -L/usr/lib -lstdc++ -lncurses

still-alive: still-alive.cpp draw.h output.h
	$(CC) -o $@ $< $(CFLAGS)

