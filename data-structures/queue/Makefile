# data-structures/queue/Makefile
#
# Makefile for queue data structure.

CC = gcc
CFLAGS = -Wall -Werror -std=c11 -ggdb

CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

LIB = queue

lib: $(LIB).o

$(LIB).o: $(LIB).c $(LIB).h

driver: lib
	$(CC) $(CFLAGS) check.c $(LIB).o -o check $(CHECK_FLAGS)

check: driver
	./check

clean:
	rm -f *~
	rm -f *.o
	rm -f check
