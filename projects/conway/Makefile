# data-structures/conway/Makefile
#
# Makefile for Conway's Game of Life project.

CC = gcc
CFLAGS = -Wall -Werror -std=c11 -ggdb

CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

LIB = conway

lib: $(LIB).o

$(LIB).o: $(LIB).c $(LIB).h

driver: lib
	$(CC) $(CFLAGS) driver.c $(LIB).o -o driver $(CHECK_FLAGS)

run: driver
	./driver 10 10

clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB).o
	rm -f driver
