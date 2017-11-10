CC = g++
CFLAGS = -c -g -Wall -std=c++11

all: minimax random_player random_blocker_player alpha-beta greedy_player genetic grid exp_arena minimax_greedy_player minimax_player alfabeta_player


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

genetic: genetic.o game.o greedy.o random_blocker.o
	$(CC) genetic.o game.o greedy.o random_blocker.o -o genetic

genetic.o: genetic.cpp
	$(CC) $(CFLAGS) genetic.cpp

random_blocker_player: random_blocker_player.o game.o greedy.o random_blocker.o
	$(CC) random_blocker_player.o random_blocker.o game.o greedy.o -o random_blocker_player

random_blocker_player.o: random_blocker_player.cpp
	$(CC) $(CFLAGS) random_blocker_player.cpp

random_blocker.o: greedy_player.cpp random_blocker.cpp
	$(CC) $(CFLAGS) random_blocker.cpp

grid.o: grid.cpp
	$(CC) $(CFLAGS) grid.cpp

grid: grid.o
	$(CC) grid.o game.o greedy.o -o grid

exp_arena: exp_arena.o game.o greedy.o random_blocker.o opt_players.o
	$(CC) exp_arena.o random_blocker.o game.o greedy.o opt_players.o -o exp_arena

exp_arena.o: exp_arena.cpp
	$(CC) $(CFLAGS) exp_arena.cpp

opt_players.o: opt_players.cpp
	$(CC) $(CFLAGS) opt_players.cpp

minimax_greedy_player: minimax_greedy_player.o game.o greedy.o opt_players.o
	$(CC) minimax_greedy_player.o opt_players.o game.o greedy.o -o minimax_greedy_player

minimax_greedy_player.o: minimax_greedy_player.cpp
	$(CC) $(CFLAGS) minimax_greedy_player.cpp

minimax_player: minimax_player.o game.o greedy.o opt_players.o
	$(CC) minimax_player.o opt_players.o game.o greedy.o -o minimax_player

minimax_player.o: minimax_player.cpp
	$(CC) $(CFLAGS) minimax_player.cpp

alfabeta_player: alfabeta_player.o game.o greedy.o opt_players.o
	$(CC) alfabeta_player.o opt_players.o game.o greedy.o -o alfabeta_player

alfabeta_player.o: alfabeta_player.cpp
	$(CC) $(CFLAGS) alfabeta_player.cpp


clean:
	rm -f *.o
	rm -f minimax
	rm -f random_player
	rm -f random_blocker_player
	rm -f alpha-beta
	rm -f genetic
	rm -f greedy_player
	rm -f grid
	rm -f exp_arena
	rm -f minimax_greedy_player
	rm -f minimax_player
	rm -f alfabeta_player