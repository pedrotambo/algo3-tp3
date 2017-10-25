CC = g++
CFLAGS = -c -g -Wall -std=c++11
OBJECTS = minimax.o


all: minimax random_player alpha-beta

minimax: minimax.o
	$(CC) minimax.o -o minimax

minimax.o: minimax.cpp
	$(CC) $(CFLAGS) minimax.cpp

alpha-beta: alpha-beta.o
	$(CC) alpha-beta.o -o alpha-beta

alpha-beta.o: alpha-beta.cpp
	$(CC) $(CFLAGS) alpha-beta.cpp	

random_player: random_player.o
	$(CC) random_player.o -o random_player

random_player.o: random_player.cpp
	$(CC) $(CFLAGS) random_player.cpp



clean:
	rm -f *.o
	rm -f minimax
	rm -f random_player
	rm -f alpha-beta