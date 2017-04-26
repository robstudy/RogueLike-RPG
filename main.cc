#include <ncurses.h>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include "GameSystem.h"

/*
 * Game Loop located in GameSystem.h
 * Level data and Overworld located in Level.h
 * Monsters are handled in MonsterFarm.h - Utilizes linked list for Monsters
 * Heap is located in Battle.h - Class handles battle screen and inputs
 * Player.h - Handles gold, keys kept, and Party (Wizzard and Warrior)
 *
 */

using namespace std;
int main() {
	//Player Input for Name	
	string userName = "", level = "level1";
	int map = 0;
	cout << "Enter your name: \n";
	cin >> userName;
	cout << "Map (1) or Map (2) ?\n";
	cin >> map;
	if(!cin) exit(1);
	if(map == 2) level = "level2";

	//Init ncurses screen
	initscr();
	//enable special key characters
	keypad(stdscr, TRUE);
	//do not echo key inputs
	noecho();
	//The Game Loop runs int he GameSystem class
	GameSystem gameSystem(level, userName);
	gameSystem.playGame();
	
	return 0;
}

