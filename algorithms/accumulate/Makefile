# algorithms/accumulate/Makefile
#
# Makefile for accumulate algorithm.

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -ggdb

CHECK_LIBS = -pthread -lcheck_pic -pthread -lrt -lm -lsubunit

LIB = accumulate

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
