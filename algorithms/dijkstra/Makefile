# algorithms/dijkstra/Makefile
#
# Makefile for Dijkstra's algorithm graph search.

CC = gcc
CFLAGS = -Wall -Werror -std=c11 -ggdb -fmax-errors=1

CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

OBJS = dijkstra.o graph.o intrusive_list.o

dijkstra.o: dijkstra.c dijkstra.h
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