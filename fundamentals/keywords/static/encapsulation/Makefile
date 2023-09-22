PROG := main

# Compiling without -Werror to avoid 'unused function' error
$(PROG): lib.c main.c
	gcc -Wall -pedantic -o main lib.c main.c

.PHONY: clean
clean:
	rm -f $(PROG)
