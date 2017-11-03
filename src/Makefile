CC = g++
CFLAGS = -c -g -Wall -std=c++11

all: minimax random_player alpha-beta greedy genetic

minimax.o: minimax.cpp
	$(CC) $(CFLAGS) minimax.cpp

alpha-beta.o: alpha-beta.cpp
	$(CC) $(CFLAGS) alpha-beta.cpp

random_player.o: random_player.cpp
	$(CC) $(CFLAGS) random_player.cpp

greedy.o: greedy.cpp
	$(CC) $(CFLAGS) greedy.cpp

genetic.o: genetic.cpp
	$(CC) $(CFLAGS) genetic.cpp

genetic: genetic.o greedy.o
	$(CC) genetic.o -o genetic

clean:
	rm -f *.o
	rm -f minimax
	rm -f random_player
	rm -f alpha-beta
	rm -f genetic
	rm -f greedy