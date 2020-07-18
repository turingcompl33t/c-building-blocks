# data-structures/stack/Makefile
#
# Makefile for stack data structure.

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -ggdb

CHECK_LIBS = -pthread -lcheck_pic -pthread -lrt -lm -lsubunit

LIB = stack

lib: $(LIB).o

$(LIB).o: $(LIB).c $(LIB).h

driver: lib
	$(CC) $(CFLAGS) check.c $(LIB).o -o check $(CHECK_LIBS)

check: driver
	./check

clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB).o
	rm -f check
