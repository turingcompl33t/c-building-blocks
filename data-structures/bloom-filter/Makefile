# data-structures/bloom-filter/Makefile
#
# Makefile for bloom-filter data structure.

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -ggdb

CHECK_LIBS = -pthread -lcheck_pic -pthread -lrt -lm -lsubunit

LIB = bloom_filter

lib: $(LIB).o murmur3.o

$(LIB).o: $(LIB).c $(LIB).h
murmur3.o: murmur3.c murmur3.h

driver: lib
	$(CC) $(CFLAGS) check.c $(LIB).o murmur3.o -o check $(CHECK_LIBS)

check: driver
	./check

clean:
	rm -f *~
	rm -f *.o
	rm -f $(LIB).o
	rm -f check
