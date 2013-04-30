CC = icc -O3 -std=c++0x
cc = icc -O3

TARGETS = main

all: $(TARGETS)

main: main.cpp delaunay.o edge.o triangles.o library.o mypred.o predicates.o
	$(CC) $(CFLAGS) $(LIB) main.cpp delaunay.o edge.o triangles.o library.o mypred.o predicates.o -o main

%.o: %.c
	$(cc) $(CFLAGS) -c $<

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGETS)