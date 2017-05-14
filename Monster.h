#pragma once
#include <iostream>
#include <string>

using namespace std;

//Ran out of Time to implement dragons, but it would be easy to add them to the game by adjusting
//Level sent from GameSystem to 2 - Wanted to work out a better fireball machanic

class Monster {
	public:
		Monster();
		Monster(int, int, int, int);
		void setPosition(int,int);
		int getX();
		int getY();
		int getID();
		int attackDamage();
		string name();
		bool key();
		int mHealth();
		void takeDamage(int);
		int giveExp();
	private:
		//standard monster stats
		int monsterID;
		int health;
		int exp;
		bool hasKey;
		int damage;
		int x;
		int y;
		string monsterName;
		//special stats for dragons
		bool isDragon;
		int fireBallDamage;
		void rollMonsterType();
};

Monster::Monster(){
}

Monster::Monster(int x, int y, int n, int level){
	monsterID = n;
	this->x = x;
	this->y = y;
	if(level == 1) isDragon = false;
	else isDragon = true;
	rollMonsterType();
}

void Monster::setPosition(int x, int y){
	this->x = x;
	this->y = y;
	rollMonsterType();
}

int Monster::getX(){
	return x;
}

int Monster::getY(){
	return y;
}

int Monster::getID(){
	return monsterID;
}

void Monster::rollMonsterType(){
	int roll = rand()%10;
	//Levels are 1 and 2, different monsters depending on first or second map
	if(isDragon){
		if(roll%2==0){
			monsterName = "Dragon";
			health = (rand()%100) + 350;
			fireBallDamage = 120;
			damage = 80;
			exp = 150;
			hasKey = true;
		} else {
			monsterName = "DragonKin";
			health = (rand()%100) + 200;
			fireBallDamage = 80;
			damage = 60;
			exp = 100;
			hasKey = false;
		}
	} else {
		if(roll%2==0){
			monsterName = "Orc";
			health = (rand()%75) + 200;
			damage = (rand()%30) + 30;
			exp = 100;
			roll = rand()%20;
			if(roll > 11) hasKey = true;
			else hasKey = false;
		} else {
			monsterName = "Goblin";
			health = (rand()%50) + 100;
			damage = (rand()%30) + 15;
			exp = 50;
			roll = rand()%20;
			if(roll < 5) hasKey = true;
			else hasKey = false;
		}
	}
}

string Monster::name(){
	return monsterName;
};

int Monster::mHealth(){
	return health;
};

bool Monster::key(){
	return hasKey;
};

void Monster::takeDamage(int x) {
	health -= x;
	if(health <= 0) health = 0;
};

int Monster::attackDamage() {
	return damage;
};

int Monster::giveExp() {
	return exp;
};
