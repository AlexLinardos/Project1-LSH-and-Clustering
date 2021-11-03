CC=g++

CFLAGS=-O3

all: lsh cube clean

lsh: LSH.o
	$(CC) LSH.o -o lsh $(CFLAGS)

LSH.o: LSH.cpp utilities.hpp LSH_ui.hpp hashing.hpp
	$(CC) -c LSH.cpp $(CFLAGS)

cube: Cube.o
	$(CC) Cube.o -o cube $(CFLAGS)

Cube.o: Cube.cpp utilities.hpp Cube_ui.hpp hashing.hpp
	$(CC) -c Cube.cpp $(CFLAGS)

clean:
	rm *.o