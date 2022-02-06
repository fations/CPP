CC = g++
CFLAGS = -c -g
LINK = g++
LNFLAGS = -g

all:	rtx rx tx

rtx : rtx.o set.o
	$(LINK) $(LNFLAGS) -o $@ rtx.o set.o 

rx : rx.cpp
	$(LINK) -o $@ rx.cpp set.o 

tx : tx.cpp
	$(LINK) -o $@ tx.cpp set.o  

rtx.o: rtx.cpp 
	$(CC) $(CFLAGS) rtx.cpp 

set.o : set.s
	$(CC) $(CFLAGS) set.s

clean:
	rm rtx.o set.o