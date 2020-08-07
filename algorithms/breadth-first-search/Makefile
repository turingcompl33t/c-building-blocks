# data-structures/breadth-first-search/Makefile
#
# Makefile for breadth-first graph search algorithm.

CC = gcc
CFLAGS = -Wall -Werror -std=c11 -ggdb -fmax-errors=1

CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

OBJS = bfs.o graph.o intrusive_list.o spanning_tree.o

bfs.o: bfs.c bfs.h
graph.o: graph.c graph.h
intrusive_list.o: intrusive_list.c intrusive_list.h
spanning_tree.o: spanning_tree.c spanning_tree.h

driver: $(OBJS)
	$(CC) $(CFLAGS) check.c $(OBJS) -o check $(CHECK_FLAGS)

check: driver
	./check

clean:
	rm -f *~
	rm -f *.o
	rm -f check