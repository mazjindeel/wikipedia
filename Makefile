CC=g++
CXXFLAGS=-c -Wall -std=c++11

all: removeForbidden

removeForbidden: removeForbidden.o
	$(CC) removeForbidden.o -o removeForbidden

removeForbidden.o: removeForbidden.cpp
	$(CC) $(CXXFLAGS) removeForbidden.cpp

clean:
	rm *.o removeForbidden
