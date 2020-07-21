# data-structures/hashmap/Makefile
#
# Makefile for hashmap data structure.

CC = gcc
CFLAGS = -Wall -Werror -std=c11 -ggdb

CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

OBJS = hashmap.o intrusive_list.o

lib: $(OBJS)

hashmap.o: hashmap.c hashmap.h 
intrusive_list.o: intrusive_list.c intrusive_list.h

driver: lib
	$(CC) $(CFLAGS) check.c $(OBJS) -o check $(CHECK_FLAGS)

check: driver
	./check

clean:
	rm -f *~
	rm -f *.o
	rm -f hashmap.o
	rm -f intrusive_list.o
	rm -f check
