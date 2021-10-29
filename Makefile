CC=g++

CFLAGS=-O3

all: lsh clean

lsh: LSH.o
	$(CC) LSH.o -o lsh $(CFLAGS)

LSH.o: LSH.cpp utilities.hpp LSH_ui.hpp
	$(CC) -c LSH.cpp $(CFLAGS)

clean:
	rm *.o