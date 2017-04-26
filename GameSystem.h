#pragma once
#include <ncurses.h>
#include "Player.h"
#include "Level.h"
#include "Battle.h"
#include "MonsterFarm.h"
#include "Monster.h"
#include <string>
#include <ctime>
#include <cmath>

using namespace std;

class GameSystem
{
	public:
		GameSystem(string, string);
		void playGame();
	private: 
		//Level Class Handles Level Inputs
		Level level;
		//Player Class handles Player data such as gold, and party members - Warrior/Wizzard
		Player player;
		//Battle Arena and level
		Battle battleArena;
		//Monsters
		MonsterFarm monsterFarm;
		void playerMove();
		void monsterMove();
		bool inBattle;
};

//Take a file name and level name - Set Player Name - Load Level file

GameSystem::GameSystem(string levelFileName, string playerName){
	player.setName(playerName);
	level.load(levelFileName, player, monsterFarm, 1);
	inBattle = false;
};

//Game Loop
/*
 * Must call refresh() to refresh screen - From ncurses library
 * timeout(200) allows the loop to run without player input - enables monster movement
 * level handles all screen data when not inBattle
 * when inBattle, refer to Battle.h class
 */

void GameSystem::playGame(){
	while(true) {
		if(!inBattle) {
			refresh();
			timeout(200);
			level.dots(monsterFarm, player);
			level.print();
			level.playerStats(player);
			playerMove();
			monsterMove();
		} else {
			refresh();
			battleArena.print();
			battleArena.playerStats(player);
			battleArena.playerMove(player);
			//If the battle ends Battle.h returns boolean and inBattle is changed
			inBattle = battleArena.inBattle();
			if(!inBattle) {
				//redue tile map coordinates in case it does draw properly
				level.dots(monsterFarm, player);
			}
		}
	}
};

//playerMove() and monsterMove() are used to check for monster encounters
//player key inputs are handled in Level.h - class Level for level inputs
//player key inputs handled in Battle.h - class Battle for battle inputs

void GameSystem::playerMove() {
	level.movePlayer(player);
	if(monsterFarm.monsterEncounter(player.getX(), player.getY())){
		inBattle = true;
		Monster m = monsterFarm.returnMonster(player.getX(),player.getY());
		battleArena.loadBattleWithMonster(m.name(), m);
		monsterFarm.deleteMonster(player.getX(), player.getY());
	}
};

void GameSystem::monsterMove() {
	int x = player.getX(), y = player.getY();
	level.moveMonsters(x, y, monsterFarm);
	if(monsterFarm.monsterEncounter(x, y)){
		inBattle = true;
		Monster m = monsterFarm.returnMonster(x, y);
		battleArena.loadBattleWithMonster(m.name(), m);
		monsterFarm.deleteMonster(x, y);
	}
};
