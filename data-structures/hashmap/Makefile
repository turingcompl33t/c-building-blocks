# data-structures/hashmap/Makefile
#
# Makefile for hashmap data structure.

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -ggdb

CHECK_LIBS = -pthread -lcheck_pic -pthread -lrt -lm -lsubunit

OBJS = hashmap.o ../intrusive-list/intrusive_list.o

lib: $(OBJS)

hashmap.o: hashmap.c hashmap.h 

intrusive_list.o: ../intrusive-list/intrusive_list.c ../intrusive-list/intrusive_list.h

driver: lib
	$(CC) $(CFLAGS) check.c $(OBJS) -o check $(CHECK_LIBS)

check: driver
	./check

clean:
	rm -f *~
	rm -f *.o
	rm -f hashmap.o
	rm -f ../intrusive-list/intrusive_list.o
	rm -f check
