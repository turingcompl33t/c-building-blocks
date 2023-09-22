CFLAGS := -Wall -Werror -pedantic

extract: extract.c
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f extract
