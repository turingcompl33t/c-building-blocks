# data-structures/Makefile
#
# Makefile for data structures library.

MAKE = make

CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c11 -ggdb

TARGETS = hashmap intrusive-list set stack

OBJS = hashmap/hashmap.o intrusive-list/intrusive_list.o queue/queue.o set/set.o stack/stack.o
LIB = structures.a

# build the data structures library
$(LIB): $(TARGETS)
	ar cr $@ $(OBJS)

hashmap:
	$(MAKE) -C hashmap

intrusive-list:
	$(MAKE) -C intrusive-list

set:
	$(MAKE) -C set

stack:
	$(MAKE) -C stack

# recursively clean each directory
clean:
	cd hashmap; $(MAKE) clean
	cd intrusive-list; $(MAKE) clean
	cd queue; $(MAKE) clean
	cd set; $(MAKE) clean
	cd stack; $(MAKE) clean

.PHONY: clean