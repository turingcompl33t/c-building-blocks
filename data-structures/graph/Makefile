# data-structures/graph/Makefile
#
# Makefile for weighted graph data structure.

CC = gcc
CFLAGS = -Wall -Werror -std=c11 -ggdb -fmax-errors=1

CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

OBJS = graph.o intrusive_list.o

graph.o: graph.c graph.h
intrusive_list.o: intrusive_list.c intrusive_list.h

driver: $(OBJS)
	$(CC) $(CFLAGS) check.c $(OBJS) -o check $(CHECK_FLAGS)

check: driver
	./check

clean:
	rm -f *~
	rm -f *.o
	rm -f check