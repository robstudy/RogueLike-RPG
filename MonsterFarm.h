#pragma once
#include <iostream>
#include <list>
#include "Monster.h"
using namespace std;

//Class handles linked list of monsters

class MonsterFarm {
	public:
		MonsterFarm();
		void addMonster(int, int, int);
		int size();
		int monsterXat(int);
		int monsterYat(int);
		void setMonsterPosition(int, int, int);
		bool monsterEncounter(int, int);
		Monster returnMonster(int, int);
		void deleteMonster(int, int);
		void spawnPoint(int, int);
	private:
		list<Monster> monsterFarm;
		const int MAX_SIZE = 8;
		int xcord;
		int xcord1;
		int ycord;
		int ycord1;
		int spawnCount;
		int mLevel;
};

MonsterFarm::MonsterFarm(){
	spawnCount = 0;
	xcord = 0;
	xcord1 = 0;
	ycord = 0;
	ycord1 = 0;
};

//Adds Monster to Farm, keeps track of level (Goblin Village or Dragon Den

void MonsterFarm::addMonster(int x, int y, int level){
	mLevel = level;
	if(monsterFarm.size() < MAX_SIZE){
		int num = monsterFarm.size();
		monsterFarm.push_back(Monster(x,y,num,level));
	}
};

int MonsterFarm::size(){
	return monsterFarm.size();
};

//Return Monster Coordinate at X

int MonsterFarm::monsterXat(int pos){
	for(auto it = monsterFarm.begin(); it != monsterFarm.end(); it++) {
		if(pos == it->getID()) return it->getX();
	}
};

//Return Monster Coordinate at Y

int MonsterFarm::monsterYat(int pos){
	for(auto it = monsterFarm.begin(); it != monsterFarm.end(); it++){
		if(pos == it->getID()) return it->getY();
	}
};

//Set Position of Monster

void MonsterFarm::setMonsterPosition(int x, int y, int pos){
	std::list<Monster>::iterator it;
	for(it = monsterFarm.begin(); it != monsterFarm.end(); it++){
		if(pos == it->getID()) it->setPosition(x, y);
	}
};

//Did player/monster run into eachother

bool MonsterFarm::monsterEncounter(int playerX, int playerY){
	std::list<Monster>::iterator it;
	for(it = monsterFarm.begin(); it != monsterFarm.end(); it++){
		if(playerX == it->getX() && playerY == it->getY()) return true;
	}
	return false;
};

//Get the Monster information

Monster MonsterFarm::returnMonster(int atX, int atY){
	std::list<Monster>::iterator it;
	for(it = monsterFarm.begin(); it != monsterFarm.end(); it++){
		if(atX == it->getX() && atY == it->getY()){
			return *it;
		}
	}
};

//Does not really delete, but rerolls mosnter into different type and returns it to spawn point.

void MonsterFarm::deleteMonster(int atX, int atY) {
	for(auto i = monsterFarm.begin(); i != monsterFarm.end(); i++) {
		if(atX == i->getX() && i->getY() == atY) {
			if(spawnCount%2==0) i->setPosition(xcord, ycord);
			else i->setPosition(xcord1, ycord1);
			spawnCount++;
		}
	}
};

//Get the initial spawn coordinates from Level.h

void MonsterFarm::spawnPoint(int atX, int atY) {
	if(xcord == 0) xcord = atX;
	else xcord1 = atX;
	if(ycord == 0) ycord = atY;
	else ycord1 = atY;
};
