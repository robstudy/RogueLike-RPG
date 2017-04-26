#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;


class Warrior {
	public:
		Warrior();
		int getHealth();
		int maxHealth();
		int getExp();
		int expToNextLevel();
		int getLevel();
		int daggerAttackDamage();
		int swordAttackDamage();
	//	int shieldBlockDamage(); - Future update - Block machanic
		void addExp(int);
		void takeDamage(int);
		void recoverHealth();
	private:
		const int MAX_LEVEL = 10;
		int level;
		int health;
		int mHealth;
		int nextLevel;
		int exp;
		int daggerDamage;
		int swordDamage;
	//	int shieldBlock; - wanted to implement a block machanic, but ran out of time 
		void levelUp();
};

Warrior::Warrior(){
	level = 1;
	health = 180;
	mHealth = 180;
	exp = 0;
	nextLevel = 100;
	daggerDamage = 20;
	swordDamage = 50;
//	shieldBlock = 30;
};

//Get Stats
int Warrior::getHealth() { return health; }
int Warrior::getExp() { return exp; }
int Warrior::getLevel() { return level; }
int Warrior::daggerAttackDamage() { return daggerDamage; }
int Warrior::swordAttackDamage() { return swordDamage; }
//int Warrior::shieldBlockDamage() { return shieldBlock; }

//Return max stats
int Warrior::maxHealth(){
	return mHealth;
};

int Warrior::expToNextLevel(){
	return nextLevel;
};

//Add exp. If exp exeeds nextLevel then subtract nextLevel from exp, raise nextLevel by *2
void Warrior::addExp(int x){ 
	exp += x;
	if(exp >= MAX_LEVEL && level == MAX_LEVEL) exp = nextLevel;
	if(exp >= nextLevel && level <= MAX_LEVEL) {
		levelUp();
		exp -= nextLevel;
		nextLevel *= 2;
	} 
};

//Increase stats on lvel up. Health is restored
void Warrior::levelUp() {
	level++;
	mHealth += 35;
	health = mHealth;
	daggerDamage += 10;
	swordDamage += 15;
//	shieldBlock += 10;
};

//send Monster damage to Warrior

void Warrior::takeDamage(int x){
	health -= x;
	if(health <= 0) health = 0;
};

void Warrior::recoverHealth() {
	health += 3;
	if(health >= mHealth) health = mHealth;
};
