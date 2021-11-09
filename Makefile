CC=g++

CFLAGS=-O3

all: lsh cube cluster clean

lsh: LSH.o
	$(CC) LSH.o -o lsh $(CFLAGS)

LSH.o: LSH.cpp utilities.hpp LSH_ui.hpp hashing.hpp
	$(CC) -c LSH.cpp $(CFLAGS)

cube: Cube.o
	$(CC) Cube.o -o cube $(CFLAGS)

Cube.o: Cube.cpp utilities.hpp Cube_ui.hpp hashing.hpp
	$(CC) -c Cube.cpp $(CFLAGS)

cluster: Clustering.o
	$(CC) Clustering.o -o cluster $(CFLAGS)

Clustering.o: Clustering.cpp utilities.hpp Clustering_ui.hpp
	$(CC) -c Clustering.cpp $(CFLAGS)

clean:
	rm *.o
	ls | grep -v "\." | grep -v Makefile | grep -v LICENSE | grep -v datasets | grep -v draft_files | xargs rm
#	rm *.exe*