
all: inkwave

inkwave: main.c
	gcc -O3 -o inkwave main.c


clean:
	rm -f inkwave
