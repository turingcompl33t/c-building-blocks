# data-structures/ring-buffer/Makefile
#
# Makefile for ring buffer data structure.

CC = gcc
CFLAGS = -Wall -Werror -std=c11 -ggdb

CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

LIB = ring_buffer

lib: $(LIB).o

$(LIB).o: $(LIB).c $(LIB).h

driver: lib
	$(CC) $(CFLAGS) check.c $(LIB).o -o check $(CHECK_FLAGS)

check: driver
	./check

clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB).o
	rm -f check
