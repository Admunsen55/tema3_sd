CC = gcc
CFLAGS = -Wall -g

build: tema3

tema3: main.o ndgraph.o dgraph.o
	$(CC) -o tema3 $^ 

main.o: main.c
	$(CC) $(CFLAGS) -c $^

ndgraph.o: ndgraph.c
	$(CC) $(CFLAGS) -c $^

dgraph.o: dgraph.c
	$(CC) $(CFLAGS) -c $^

run: build
	./tema3 $(arg1) $(arg2)

clean:
	rm -f *.o tema3
