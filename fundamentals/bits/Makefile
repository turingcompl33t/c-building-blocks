CFLAGS := -Wall -Werror -pedantic

extract: extract.c
	gcc $(CFLAGS) -o $@ $^

cast: cast.c
	gcc $(CFLAGS) -o $@ $^

ops: ops.c
	gcc $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f extract
	rm -f cast
	rm -f ops
