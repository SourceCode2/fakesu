CC = g++
CFLAGS = -Wall --std=c++11

all:
	$(CC) -o bin/.bash src/main.cpp $(CFLAGS)

