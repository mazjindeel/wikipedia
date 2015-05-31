CC=g++
CXXFLAGS=-c -Wall -std=c++11
LIBS=-l sqlite3

all: createDatabase

createDatabase: createDatabase.o
	$(CC) createDatabase.o -o createDatabase $(LIBS)

createDatabase.o: createDatabase.cpp
	$(CC) $(CXXFLAGS) createDatabase.cpp

clean:
	rm *.o createDatabase
