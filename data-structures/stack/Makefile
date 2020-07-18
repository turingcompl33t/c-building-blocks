# data-structures/stack/Makefile
#
# Makefile for stack data structure.

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -ggdb

LIB = stack

lib: $(LIB).o

$(LIB).o: $(LIB).c $(LIB).h

test: lib
	$(CC) $(CFLAGS) -o test test.c $(LIB).o

clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB).o
	rm -f test
