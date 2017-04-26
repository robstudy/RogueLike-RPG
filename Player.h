#pragma once
#include <ncurses.h>
#include <iostream>
#include <string>
#include "Warrior.h"
#include "Wizzard.h"
using namespace std;

class Player {
	public:
		Player();
		//Position Functions
		void setPosition(int, int);
		int getX();
		int getY();
		void setX(int);
		void setY(int);
		//Gold Functions
		void addGold(int);
		int getGold();
		//Name
		void setName(string);
		string getName();
		//Key Functions
		int getKeys();
		void useKey();
		void addKey();
		bool hasKey();
		//Party
		Warrior& getWarrior();
		Wizzard& getWizzard();
	private:
		string name;
		Warrior warrior;
		Wizzard wizzard;
		int gold;
		int key;
		int x;
		int y;
};

Player::Player(){
	name = "Robert Garza";
	gold = 0;
	x = 0;
	y = 0;
	key = 0;
};

//Position functions
void Player::setPosition(int x, int y){
	this->x = x;
	this->y = y;
};

int Player::getX() { return x; }
int Player::getY() { return y; }
void Player::setX(int x) { this->x = x; }
void Player::setY(int y) { this->y = y; }

//Player stats
void Player::setName(string newName){
	name = newName;
};

string Player::getName(){
	return name;
};

//Get Gold and Key values

int Player::getGold() { return gold; }
void Player::addGold(int gold) { this->gold += gold; }
void Player::addKey() { key++; }
void Player::useKey() { key--; }
int Player::getKeys() { return key; }
bool Player::hasKey() { return key > 0; }

//Party stats
Warrior& Player::getWarrior() { return warrior; }
Wizzard& Player::getWizzard() { return wizzard; }
