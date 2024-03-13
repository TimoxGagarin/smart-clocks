CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L

all: clocks

clocks: main.o utils.o
	$(CC) -o build/debug/clocks build/debug/main.o build/debug/utils.o -lncurses -lm

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o build/debug/main.o

utils.o: src/utils.c
	$(CC) $(CFLAGS) -c src/utils.c -o build/debug/utils.o

clean:
	rm -f build/debug/clocks build/debug/main.o build/debug/utils.o