CC=g++
CXXFLAGS=-c -Wall -std=c++11
LIBS=-l sqlite3

all: removeForbidden

removeForbidden: removeForbidden.o
	$(CC) removeForbidden.o -o removeForbidden $(LIBS)

removeForbidden.o: removeForbidden.cpp
	$(CC) $(CXXFLAGS) removeForbidden.cpp

clean:
	rm *.o removeForbidden
