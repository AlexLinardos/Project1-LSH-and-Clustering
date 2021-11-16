CC=g++

CFLAGS=-O3

all: LSH HC CLUST clean1

LSH: LSH.o
	$(CC) LSH.o -o bin/LSH $(CFLAGS)

LSH.o: ./LSHashing/LSH.cpp misc/utilities.hpp LSHashing/LSH_ui.hpp misc/hashing.hpp LSHashing/LSH.hpp
	$(CC) -c ./LSHashing/LSH.cpp $(CFLAGS)

HC: HC.o
	$(CC) HC.o -o bin/HC $(CFLAGS)

HC.o: Hypercube/HC.cpp misc/utilities.hpp Hypercube/HC_ui.hpp misc/hashing.hpp Hypercube/HC.hpp
	$(CC) -c Hypercube/HC.cpp $(CFLAGS)

CLUST: CLUST.o
	$(CC) CLUST.o -o bin/CLUST $(CFLAGS)

CLUST.o: Clustering/CLUST.cpp misc/utilities.hpp Clustering/CLUST_ui.hpp Clustering/CLUST.hpp
	$(CC) -c Clustering/CLUST.cpp $(CFLAGS)

clean:
	rm bin/*
	rm *.txt

clean1:
	rm *.o


