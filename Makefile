CC = tcc

all: nav

nav: main.c
	$(CC) -o $@ `pkg-config gtk+-3.0 --cflags --libs` $^

install: nav
	mv $^ /usr/local/bin

.PHONY: clean

clean:
	rm -f nav
