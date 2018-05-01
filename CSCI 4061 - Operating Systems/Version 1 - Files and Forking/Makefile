CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g
FILENAME=votecounter.c
EXECNAME=votecounter

make: $(FILENAME) makeargv.h open_read.o
	$(CC) $(CFLAGS) -o $(EXECNAME) $(FILENAME) open_read.o

debug: $(FILENAME) makeargv.h
	$(CC) $(CFLAGS) $(DBFLAGS) -o $(EXECNAME) $(FILENAME) open_read.o

clean: 
	rm $(EXECNAME)
