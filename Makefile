
all: inkwave

inkwave: main.c
	gcc -o inkwave main.c


clean:
	rm -f inkwave
