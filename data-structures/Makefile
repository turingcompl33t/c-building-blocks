# data-structures/Makefile
#
# Makefile for data structures library.

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -ggdb

OBJS = deque/deque.o list/list.o queue/queue.o stack/stack.o
LIB = structures.a

# build the data structures library
$(LIB): $(OBJS)
	ar cr $@ $^

deque:
	$(MAKE) -C deque

list:
	$(MAKE) -C list

queue:
	$(MAKE) -C queue

stack:
	$(MAKE) -C stack

# recursively clean each directory
clean:
	cd deque; $(MAKE) clean
	cd list; $(MAKE) clean
	cd queue; $(MAKE) clean
	cd stack; $(MAKE) clean

.PHONY: clean