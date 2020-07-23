# algorithms/quicksort/Makefile
#
# Makefile for quicksort algorithm.

CC = gcc
CFLAGS = -Wall -Werror -std=c11 -ggdb

CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

LIB = quicksort

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
