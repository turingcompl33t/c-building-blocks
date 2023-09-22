PROG := main

build: $(PROG)

$(PROG):
	gcc -Wall -Werror -pedantic -o $(PROG) main.c lib.c

.PHONY: clean
clean:
	rm -f $(PROG)

