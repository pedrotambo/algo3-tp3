CC = g++
CFLAGS = -c -g -Wall -std=c++11

all: minimax random_player alpha-beta greedy_player genetic

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

greedy_player: greedy_player.o game.o greedy.o
	$(CC) greedy_player.o game.o greedy.o -o greedy_player

greedy_player.o: greedy_player.cpp
	$(CC) $(CFLAGS) greedy_player.cpp

greedy.o: greedy.cpp
	$(CC) $(CFLAGS) greedy.cpp

game.o: game.cpp
	$(CC) $(CFLAGS) game.cpp

genetic: genetic.o game.o greedy.o
	$(CC) genetic.o game.o greedy.o -o genetic

genetic.o: genetic.cpp
	$(CC) $(CFLAGS) genetic.cpp

# g++ -std=c++11 greedy_player.cpp greedy.cpp game.cpp -o greedy_player

# g++ -std=c++11 genetic.cpp greedy.cpp game.cpp -o genetic

clean:
	rm -f *.o
	rm -f minimax
	rm -f random_player
	rm -f alpha-beta
	rm -f genetic
	rm -f greedy_player
