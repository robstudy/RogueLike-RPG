#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;


class Wizzard {
	public:
		Wizzard();
		int getHealth();
		int maxHealth();
		int getMana();
		int maxMana();
		void recoverMana();
		int getExp();
		int expToNextLevel();
		int getLevel();
		//Wizzards three spells
		int lightningDamage();
		int fireDamage();
		int frostDamage();
		//Future update add block machanics
	//	int blockDamage();
	//	void manaShieldToggle();
	//	bool shieldUp();
		void addExp(int);
		void takeDamage(int);
		bool useMana(int);
		void recoverHealth();
	private:
		const int MAX_LEVEL = 10;
		int level;
		int nextLevel;
		int health;
		int mHealth;
		int mana;
		int mMana;
		int exp;
		int lightningBolt;
		int fireBall;
		int frost;
//		bool manaShield;
//		int block;
		void levelUp();
};

Wizzard::Wizzard(){
	level = 1;
	health = 150;
	mHealth = 150;
	mana = 100;
	mMana = 100;
	exp = 0;
	nextLevel = 100;
	lightningBolt = 20;
	fireBall = 50;
	frost = 40;
//	manaShield = false;
//	block = 30;
};

//Get Stats
int Wizzard::getHealth() { return health; }
int Wizzard::getMana() { return mana; }
int Wizzard::getLevel() { return level; }
int Wizzard::getExp() { return exp; }
int Wizzard::expToNextLevel() { return nextLevel;}
int Wizzard::lightningDamage() { return lightningBolt; }
int Wizzard::fireDamage() { return fireBall; }
int Wizzard::frostDamage() { return frost; }
//int Wizzard::blockDamage() { return block; }

/*void Wizzard::manaShieldToggle() {
	if(manaShield) manaShield = false;
	else manaShield = true;
};

bool Wizzard::shieldUp() { return manaShield; }
*/

//Get max stats
int Wizzard::maxHealth() { return mHealth; }
int Wizzard::maxMana() { return mMana; }

//Next level resets Exp to and adds to next level
//Wizzard levels up faster than the Warrior
void Wizzard::addExp(int x) {
	exp += x;
	if(exp >= nextLevel && level <= MAX_LEVEL) {
		levelUp();
		exp -= nextLevel;
		nextLevel += nextLevel - 25;
	}
};

//Add stats on level up. Can only be reached in addExp(int)

void Wizzard::levelUp() {
	level++;
	mHealth += 25;
	health = mHealth;
	mMana += 30;
	mana = mMana;
	lightningBolt += 10;
	fireBall += 20;
	frost += 15;
};

//Take damage from Monster

void Wizzard::takeDamage(int x) {
	health -= x;
	if(health <= 0) health = 0;
};

//Can adjust this for mana recovery

void Wizzard::recoverMana() {
	mana += 5;
	if(mana >= mMana) mana = mMana;
};

//Returns bool, if no mana return false, cannot use spell

bool Wizzard::useMana(int x) {
	if((mana-x) > 0) {
		mana -= x;
		return true;
	} else {
		return false;
	}
};

//Used in the overworld to recover health
void Wizzard::recoverHealth(){
	health += 2;
	if(health >= mHealth) health = mHealth;
};
