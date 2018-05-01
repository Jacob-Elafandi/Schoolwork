CC=gcc
CFLAGS=-std=c99 -pthread
DBFLAGS=-g
HEADERS=util.h dag.h pqueue.h votelist.h
	
make: votecounter.c $(HEADERS)
	$(CC) $(CFLAGS) -o votecounter votecounter.c $(HEADERS)
	
debug: votecounter.c $(HEADERS)
	$(CC) $(CFLAGS) $(DBFLAGS) -o votecounter votecounter.c $(HEADERS)

clean:
	rm votecounter
