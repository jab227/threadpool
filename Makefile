CFLAGS=-Wall -Werror -Wconversion -Wpedantic -Wextra -O1 -g -std=c11 -fsanitize=thread
LDFLAGS=-fsanitize=thread
LDLIBS=-lpthread

objects=main.o pool.o queue.o

all: main

main: $(objects)

main.o: main.c pool.h

pool.o: pool.c pool.h queue.h

queue.o: queue.c queue.h

.PHONY: clean
clean:
	rm -rfv $(objects)
