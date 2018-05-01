CC=gcc
CFLAGS=-std=c99
DBFLAGS=-g

make: Leaf_Counter.c Aggregate_Votes.c Vote_Counter.c util.h
	make Leaf_Counter
	make Aggregate_Votes
	make Vote_Counter

debug: Leaf_Counter.c Aggregate_Votes.c Vote_Counter.c util.h
	make Leaf_Counter_debug
	make Aggregate_Votes_debug
	make Vote_Counter_debug

Leaf_Counter: Leaf_Counter.c util.h
	$(CC) $(CFLAGS) -o Leaf_Counter Leaf_Counter.c util.h
	
Leaf_Counter_debug: Leaf_Counter.c util.h
	$(CC) $(CFLAGS) $(DBFLAGS) -o Leaf_Counter Leaf_Counter.c util.h

Aggregate_Votes: Aggregate_Votes.c util.h
	$(CC) $(CFLAGS) -o Aggregate_Votes Aggregate_Votes.c util.h
	
Aggregate_Votes_debug: Aggregate_Votes.c util.h
	$(CC) $(CFLAGS) $(DBFLAGS) -o Aggregate_Votes Aggregate_Votes.c util.h
	
Vote_Counter: Vote_Counter.c util.h
	$(CC) $(CFLAGS) -o Vote_Counter Vote_Counter.c util.h
	
Vote_Counter_debug: Vote_Counter.c util.h
	$(CC) $(CFLAGS) $(DBFLAGS) -o Vote_Counter Vote_Counter.c util.h

clean:
	if [ -a Leaf_Counter ] ; then rm Leaf_Counter ; fi;
	if [ -a Aggregate_Votes ] ; then rm Aggregate_Votes ; fi;
	if [ -a Vote_Counter ] ; then rm Vote_Counter ; fi;
