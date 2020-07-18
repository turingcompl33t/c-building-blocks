# data-structures/Makefile
#
# Makefile for data structures library.

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -ggdb

TARGETS = deque list queue set stack

OBJS = deque/deque.o list/list.o queue/queue.o set/set.o stack/stack.o
LIB = structures.a

# build the data structures library
$(LIB): $(TARGETS)
	ar cr $@ $(OBJS)

deque:
	$(MAKE) -C deque

list:
	$(MAKE) -C list

queue:
	$(MAKE) -C queue

set:
	$(MAKE) -C set

stack:
	$(MAKE) -C stack

# recursively clean each directory
clean:
	cd deque; $(MAKE) clean
	cd list; $(MAKE) clean
	cd queue; $(MAKE) clean
	cd set; $(MAKE) clean
	cd stack; $(MAKE) clean

.PHONY: clean