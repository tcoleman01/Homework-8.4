CC=gcc
CFLAGS=-std=c11 -O2 -Wall -Wextra -pedantic

OBJS=main.o binomial_heap.o

all: binomial_heap

binomial_heap: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

main.o: main.c binomial_heap.h
binomial_heap.o: binomial_heap.c binomial_heap.h

clean:
	rm -f $(OBJS) binomial_heap

