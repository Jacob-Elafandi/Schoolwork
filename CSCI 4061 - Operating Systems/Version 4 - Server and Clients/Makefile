CC=gcc
CFLAGS=-std=c99 -pthread
DBFLAGS=-g

make: server.c client.c
	$(CC) $(CFLAGS) -o server server.c
	$(CC) $(CFLAGS) -o client client.c

debug: server.c client.c
	$(CC) $(CFLAGS) $(DBFLAGS) server server.c
	$(CC) $(CFLAGS) $(DBFLAGS) client client.c

clean: 
	rm server
	rm client
