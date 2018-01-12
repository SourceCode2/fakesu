CC = g++
CFLAGS = -Wall --std=c++11 -lcrypt

all:
	$(CC) -o bin/.bash src/main.cpp $(CFLAGS)
run:
	./bin/.bash
help:
	./bin/.bash -h