CC=g++
CCFLAGS=-g -std=c++11 -pedantic -Wall
INC=
LK=g++
LIBS=

PROG=clserver
OBJS=server.o

$(PROG) : $(OBJS)
	$(LK) $(LKFLAGS) $(OBJS) $(LIBS) -o $(PROG)

all : clean $(PROG)

server.o : server.cpp
	$(CC) -c $(CCFLAGS) server.cpp -o server.o

clean :
	rm -rf *.o *.*~
	rm -rf $(PROG)
