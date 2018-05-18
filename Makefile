
DESTDIR=/usr/local

all: inkwave

inkwave: main.c
	gcc -O3 -o inkwave main.c

install: inkwave
	mkdir -p $(DESTDIR)/bin
	install -m 0755 inkwave $(DESTDIR)/bin/inkwave

clean:
	rm -f inkwave
