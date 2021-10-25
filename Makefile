CC=g++

CFLAGS=-O3

all: final clean

final: LSH.o LSH_ui.o
	$(CC) LSH.o LSH_ui.o -o final $(CFLAGS)

LSH.o: LSH.cpp utilities.hpp
	$(CC) -c LSH.cpp

LSH_ui.o: LSH_ui.cpp LSH_ui.h
	$(CC) -c LSH_ui.cpp

clean:
	rm *.o