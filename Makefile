a.out: main.cc Player.h GameSystem.h Level.h Monster.h MonsterFarm.h Warrior.h Wizzard.h Battle.h
	g++ -g -std=c++11 main.cc -lncurses
clean:
	rm a.out
