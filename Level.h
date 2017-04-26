#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <ncurses.h>
#include "Player.h"
#include "MonsterFarm.h"
#include "GameSystem.h"
#include "Warrior.h"
#include "Wizzard.h"
#include "Battle.h"

void wait_ticks(int ticks) {
	clock_t start = clock();
	while((clock() - start) < ticks) //do nothing
		;
};

class Level
{
	public:
		Level();

		void load(string, Player &, MonsterFarm &, int);
		void movePlayer(Player &);
		void playerStats(Player);
		void print();
		void moveMonsters(int, int, MonsterFarm &);
		void dots(MonsterFarm, Player);
	private:
		vector <string> levelData;
		char getTile(int, int);
		bool canMove(char, Player &);
		bool canMoveMonster(char);
		void updateMap(int, int, int, int, char, Player &);
		void updateMonster(int, int, int, int, char);
		int level;
		int steps;
		//Two spawn locations on the Map - All created maps must have two 'X' spawn locations
		int spawnX;
		int spawnY;
		int spawnX2;
		int spawnY2;
		//Player wins - Bool set to true after 100 gold reached
		bool wonGame;
};

Level::Level(){
	wonGame = false;
	spawnX = 0;
	spawnY = 0;
	spawnX2 = 0;
	spawnY2 = 0;
};

//Load map and process Map data

void Level::load(string fileName, Player &player, MonsterFarm &monsterFarm, int level){
	this->level = level;
	steps = 0;
	//load level
	ifstream file;
	file.open(fileName);
	if(file.fail()){
		perror(fileName.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;
	while(getline(file, line)){
		levelData.push_back(line);
	}
	file.close();

	//process level
	char tile;
	for(int i = 0; i < levelData.size(); i++) {
		for(int j = 0; j < levelData[i].size(); j++) {
			tile = levelData[i][j];
			switch(tile) {
				case '@':
					player.setPosition(j, i);
					break;
				case 'M':
					monsterFarm.addMonster(j, i, level);
					break;
				case 'X':
					monsterFarm.spawnPoint(j, i);
					if(spawnX == 0) spawnX = j;
					else spawnX2 = j;
					if(spawnY == 0) spawnY = i;
					else spawnY2 = i;
					break;
		  	 }
		}
	}

	//Color Pairs
	start_color(); //Enables Colors if possible	   
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_BLUE, COLOR_CYAN);

};

//Print entire level from levelData<string> vector

void Level::print() {
	if(wonGame) {
		attron(COLOR_PAIR(1));
		mvprintw(5,0, "            CONGLATURATION !!!");
		mvprintw(6,0, "            YOU HAVE COMPLETED");
		mvprintw(7,0, "               A GREAT GAME.");
		mvprintw(8,0, "          AND PROOVED THE JUSTICE");
		mvprintw(9,0, "               OF OUR CULTURE.");
		mvprintw(10,0,"            NOW GO AND REST OUR");
		mvprintw(11,0,"                  HEROES!");
		mvprintw(15,0,"             press 'q' to quit");
		attroff(COLOR_PAIR(1));
	} else {
		for(int i = 0; i < levelData.size(); i++){
			for(int j = 0; j < levelData[i].size(); j++){
				char tile = levelData[i][j];
				int color = 1;
				switch(tile){
					case '|':
					case 'O':
					case 'X':
					case '@':
						color = 1;
						break;
					case '.':
						color = 2;
						break;
					case 'T':
						color = 3;
						break;
					case '~':
						color = 4;
						break;
					case '#':
						color = 5;
						break;
					case '^':
						color = 6;
						break;
					case 'M':
						color = 7;
						break;
				}
				attron(COLOR_PAIR(color));
				mvaddch(i,j, tile);
				attroff(COLOR_PAIR(color));
			}
		}
		int x = levelData.at(0).size();
		int y = levelData.size();
		attron(COLOR_PAIR(1));
		if(level == 1) mvprintw(y,0, "                         Goblin Village");
		else mvprintw(y,0, "                  The Dragon's Den");
		mvprintw(y+1,0, "Move with arrow keys or w/a/s/d keys. Press 'q' to quit.");
		mvprintw(y+2,0, "Betware of 'M'onsters!");
		mvprintw(y+3,0, "Find 100 gold from 'T'reasure chests and win! 'M'onsters drop keys!");
		mvprintw(y+4,0, "Key drops are random. Orcs drop at higher rates than Goblins.");
		attroff(COLOR_PAIR(1));
	}
};

//Prints Player Stats to Level Screen

void Level::playerStats(Player player){
	if(!wonGame) {
		int x = levelData.at(0).size()+1;
		int y = 0;
		Warrior warriorStats = player.getWarrior();
		Wizzard wizzardStats = player.getWizzard();
		string goldString = "Gold: " + to_string(player.getGold());
		string keys = "Keys: " + to_string(player.getKeys());
		string warriorHealth = "Health: " + to_string(warriorStats.getHealth()) + " / " + to_string(warriorStats.maxHealth());
		string warriorLevel = "Level: " + to_string(warriorStats.getLevel());
		string warriorExp = "Exp: " + to_string(warriorStats.getExp()) + " / " + to_string(warriorStats.expToNextLevel());
		string wizzardHealth = "Health: " + to_string(wizzardStats.getHealth()) + " / " + to_string(wizzardStats.maxHealth());
		string wizzardMana = "Mana: " + to_string(wizzardStats.getMana()) + " / " + to_string(wizzardStats.maxMana());
		string wizzardLevel = "Level: " + to_string(wizzardStats.getLevel());
		string wizzardExp = "Exp: " + to_string(wizzardStats.getExp()) + " / " + to_string(wizzardStats.expToNextLevel());
		attron(COLOR_PAIR(1));
		mvprintw(y, x, player.getName().c_str());
		mvprintw(y+1, x, goldString.c_str());
		mvprintw(y+2, x, keys.c_str());
		mvprintw(y+3, x, "----------");
		mvprintw(y+4, x, "Party ");
		mvprintw(y+5, x, "----------");
		mvprintw(y+6, x, "Warrior: ");
		mvprintw(y+7, x, warriorHealth.c_str());
		mvprintw(y+8, x, warriorLevel.c_str());
		mvprintw(y+9, x, warriorExp.c_str());
		mvprintw(y+10, x, "----------");
		mvprintw(y+11, x, "Wizzard: ");
		mvprintw(y+12,x, wizzardHealth.c_str());
		mvprintw(y+13,x, wizzardMana.c_str());
		mvprintw(y+14,x, wizzardLevel.c_str());
		mvprintw(y+15,x, wizzardExp.c_str());
		mvprintw(y+20,x, "");
		attroff(COLOR_PAIR(1));
	}
};

//Handles Player Moves Around the Map

void Level::movePlayer(Player &player) {
   	int playerX = player.getX();
	int playerY = player.getY();
	char moveTile;
    int input = getch();
	switch(input){
		case 'Q':
		case 'q':
			refresh();
			wait_ticks(300000);
			endwin();
			system("clear");
			exit(EXIT_SUCCESS);
		case 'w':
		case 'W':
		case KEY_UP:
			moveTile = getTile(playerX, playerY -1);
			if(canMove(moveTile, player)) {
				player.setY(playerY - 1);
				updateMap(playerX, playerY, playerX, playerY - 1, moveTile, player);
			}
			break;
		case 's':
		case 'S':
		case KEY_DOWN:
			moveTile = getTile(playerX, playerY + 1);
			if(canMove(moveTile, player)) {
				player.setY(playerY + 1);
				updateMap(playerX, playerY, playerX, playerY + 1, moveTile, player);
			}
			break;
		case 'a':
		case 'A':
		case KEY_LEFT:
			moveTile = getTile(playerX - 1, playerY);
			if(canMove(moveTile, player)) {
				player.setX(playerX -1);
				updateMap(playerX, playerY, playerX - 1, playerY, moveTile, player);
			}
			break;
		case 'd':
		case 'D':
		case KEY_RIGHT:
			moveTile = getTile(playerX + 1, playerY);
			if(canMove(moveTile, player)) {
				player.setX(playerX + 1);
				updateMap(playerX, playerY, playerX + 1, playerY, moveTile, player);
			}
			break;
		default:
			break;
	}
	steps++;
	if(steps % 10 == 0) {
		player.getWizzard().recoverMana();
		player.getWizzard().recoverHealth();
		player.getWarrior().recoverHealth();
		steps = 0; 
	}
};

//Handles Monster Moves, updates all monster in the MonsterFarm

void Level::moveMonsters(int playerX, int playerY, MonsterFarm &monsterFarm){
	for(int i = 0; i < monsterFarm.size(); i++){
		int move = rand()%4;
		int monsterX = monsterFarm.monsterXat(i);
		int monsterY  = monsterFarm.monsterYat(i);
		char moveTile;
		switch(move){
			case 0:
				moveTile = getTile(monsterX, monsterY - 1);
				if(canMoveMonster(moveTile)){
					monsterFarm.setMonsterPosition(monsterX, monsterY -1, i);
					updateMonster(monsterX, monsterY, monsterX, monsterY -1, moveTile);
				}
				break;
			case 1:
				moveTile = getTile(monsterX, monsterY +1);
				if(canMoveMonster(moveTile)){
					monsterFarm.setMonsterPosition(monsterX, monsterY +1, i);
					updateMonster(monsterX, monsterY, monsterX, monsterY +1, moveTile);
				}
				break;
			case 2:
				moveTile = getTile(monsterX-1, monsterY);
				if(canMoveMonster(moveTile)){
					monsterFarm.setMonsterPosition(monsterX-1, monsterY, i);
					updateMonster(monsterX, monsterY, monsterX-1, monsterY, i);
				}
				break;
			case 3:
				moveTile = getTile(monsterX+1, monsterY);
				if(canMoveMonster(moveTile)){
					monsterFarm.setMonsterPosition(monsterX+1, monsterY, i);
					updateMonster(monsterX, monsterY, monsterX+1, monsterY, i);
				}
				break;
		}
	}
};

//Grabs char ("Tile") for comparison, stored in levelData<string> vector

char Level::getTile(int x, int y) {
	return levelData[y][x];
};

//Checks if Monster can move to spot

bool Level::canMoveMonster(char moveTile){
	switch(moveTile){
		case '#':
		case '~':
		case '^':
		case 'T':
		case 'X':
		case 'M':
			return false;
		default:
			return true;
	}
};

//Checks if player can move and also updates player gold if landing on 'T' spot

bool Level::canMove(char moveTile, Player &player) {
	switch(moveTile){
		case '#':
		case '~':
		case '^':
		case 'X':
			return false;
		case 'T':
			if(player.hasKey()) {
				player.useKey();
				int chestGold = rand()%20 + 14;
				player.addGold(chestGold);
				if (player.getGold() >= 100) {
					wonGame = true;
					clear();
				}
				return true;
			} 
			return false;
		default:
			return true;
	}
};

//updates levelData<string> vector to reflect moves of Monsters

void Level::updateMonster(int oldX, int oldY, int newX, int newY, char oldTile){
	if(oldTile == 'T') levelData[oldY][oldX] = 'T';
	else levelData[oldY][oldX] = '.';
	levelData[newY][newX] = 'M';
};

//updates Player in levelData<string> vector

void Level::updateMap(int oldX, int oldY, int newX, int newY, char oldTile, Player &player){
	if(oldTile == 'T') player.addGold(10);
	levelData[oldY][oldX] = '.';
	levelData[newY][newX] = '@';
};

//Makes sure map is displaying spawn points and player properly

void Level::dots(MonsterFarm monsterFarm, Player player) {
	for(int i = 0; i < levelData.size(); i++) {
		for(int j = 0; j < levelData[i].size(); j++) {
			if(levelData[i][j] == '@' && player.getX() != j && player.getY() != i)
				levelData[i][j] = '.';
			if(player.getX() == j && player.getY() == i) levelData[i][j] = '@';
			if((spawnX == j && spawnY == i) || (spawnX2 == j && spawnY2 == i))
				levelData[i][j] = 'X';
		}
	}
};
