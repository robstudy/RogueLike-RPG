#pragma once
#include <ncurses.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "Warrior.h"
#include "Wizzard.h"
#include "Monster.h"
using namespace std;

void wait_tick(int ticks) {
	clock_t start = clock();
	while((clock() - start) < ticks) //do nothing
		;
}

//Class handles the Battle Screen when player runs into a Monster
//Builds up a heap for attacks

class Battle {
	public:
		Battle();
		void loadBattleWithMonster(string, Monster);
		void playerMove(Player &);
		void print();
		void playerStats(Player);
		bool inBattle();
	private:
		vector<string> monsterPic;
		vector<int> attackHeap;
		vector<string> battleText;
		Monster enemy;
		bool battle;
		bool playerChoice;
		bool warriorTurn;
		bool frozenEnemy;
		bool playerDead;
		void heapBattle(Player &);
		void wonBattle(Player &);
		void gameOver();
};

Battle::Battle(){
	
};

//Load the Ascii files with the monster images

void Battle::loadBattleWithMonster(string monsterFile, Monster monster) {
	clear();
	battle = true;
	playerChoice = true;
	warriorTurn = true;
	playerDead = false;
	monsterPic.clear();
	enemy = monster;
	//Load Pic
	ifstream file;
	file.open(monsterFile);
	if(file.fail()) {
		perror(monsterFile.c_str());
		system("PAUSE");
		exit(1);
	}

	string line;
	while(getline(file, line)) {
		monsterPic.push_back(line);
	}
	file.close();
};

//Print Monster Information - Name/Pic/Health

void Battle::print(){
	if(playerDead) {
		gameOver();
		return;
	}
	//Adjust for certain MonsterPics
	int atX = 0, adjust = 5;
	if(enemy.name() == "Orc") adjust = 15;
	if(enemy.name() == "Goblin") atX = 10;
	attron(COLOR_PAIR(1));
	string text = "You ran into a " + enemy.name();
	string monsterHealth = "    Health: " + to_string(enemy.mHealth());
	mvprintw(0, atX+adjust, text.c_str());
	mvprintw(1, atX+adjust, monsterHealth.c_str());
	attron(COLOR_PAIR(2));
	for(int i = 0; i < monsterPic.size(); i++) {
		mvprintw(i+2, atX, monsterPic.at(i).c_str());
	}
	attroff(COLOR_PAIR(1));
	attroff(COLOR_PAIR(2));
};

//Print the battle Screen - Player Choices - 3 phases - 1) Warrior Choice 2) Wizzard Choice 3) BattleText display

void Battle::playerStats(Player player){
	if(playerDead){
		gameOver();
		return;
	}
	//Get Y value so there is no text overlapp with picture
	int atY = monsterPic.size();
	Warrior war = player.getWarrior();
	Wizzard wiz = player.getWizzard();

	attron(COLOR_PAIR(1));

	//Warrior Turn Screen
	if(playerChoice && warriorTurn){
		string warHealth = "Health: " + to_string(war.getHealth()) + " / " + to_string(war.maxHealth());
		string dagger = "(D)agger: " + to_string(war.daggerAttackDamage()) + " damage, Hit Chance(90%), Fast Attack Speed";
		string sword = "(S)word: " + to_string(war.swordAttackDamage()) + " damage, Hit Chance(75%), Slow Attack Speed";
		mvprintw(atY + 3, 20, "Choose your move");
		mvprintw(atY+5, 20, "    Warrior");
		mvprintw(atY+6, 20, " --------------");
		mvprintw(atY+7, 20, warHealth.c_str());
		mvprintw(atY+8, 5, dagger.c_str());
		mvprintw(atY+9, 5, sword.c_str());
	}
	//Wizzard Turn
	else if (playerChoice && !warriorTurn){
		string wizHealth = "Health: " + to_string(wiz.getHealth()) + " / " + to_string(wiz.maxHealth());
		string wizMana = "Mana: " + to_string(wiz.getMana()) + " / " + to_string(wiz.maxMana());
		string lightning = "(L)ightning: " + to_string(wiz.lightningDamage()) + " damage, Hit Chance (90%), Fast Attack Speed -  (0) Mana";
		string frost = "F(R)ost: " + to_string(wiz.frostDamage()) + " damage, Hit Chance (80%), Fast Attack Speed, (25%) chance to Freeze - (20) Mana";
		string fire = "(F)ireball: " + to_string(wiz.fireDamage()) + " damage, Hit Chance (70%), Slow Attack Speed - (30) Mana";
		mvprintw(atY + 3, 20, "Choose your move");
		mvprintw(atY+5, 20, "    Wizzard");
		mvprintw(atY+6, 20, " --------------");
		mvprintw(atY+7, 20, wizHealth.c_str());
		mvprintw(atY+8, 20, wizMana.c_str());
		mvprintw(atY+9, 1, lightning.c_str());
		mvprintw(atY+10, 1, frost.c_str());
		mvprintw(atY+11, 1, fire.c_str());
	}
	//Else show battleText from heapBattle()
	else {
		for(int i = 0; i < battleText.size(); i++) {
			mvprintw(atY+3+i, 10, battleText.at(i).c_str());
		}
		mvprintw(atY+battleText.size()+7, 10, "Press 'c' to continue");
	}
	attroff(COLOR_PAIR(1));
};

//GameOver Screen

void Battle::gameOver() {
	attron(COLOR_PAIR(1));
	mvprintw(5, 0, "               Our Heroes have fallen!");
	mvprintw(6, 0, "               DISHONOR ON YOUR FAMILY");
	mvprintw(10, 0, "                  Press 'q' to quit");
	attroff(COLOR_PAIR(1));
};

/*
 * Attack Speed Key (Higher Speeds will pop off heap first)
 *--------------------------------------
 * 10 - Sword Swing 
 * 15 - FireBall
 * 20 through 29 - Monster Swings
 * 30 - Lightning
 * 35 - Frost
 * 40 - Dagger
 */
void Battle::playerMove(Player &player){
	int input = getch();
	if(warriorTurn && playerChoice) {
		switch(input){
			case 'D':
			case 'd':
				attackHeap.push_back(40); //Dagger swing
				attackHeap.push_back(20); //Monster Swing
				warriorTurn = false;
				break;
			case 'S':
			case 's':
				attackHeap.push_back(10); //Sword Swing
				attackHeap.push_back(21); //Monster Swing
				warriorTurn = false;
				break;
			default:
				break;
		}
		clear();
	} else if (!warriorTurn && playerChoice) {
		switch(input) {
			case 'L':
			case 'l':
				attackHeap.push_back(30); //Lightning
				attackHeap.push_back(22); //Monster Swing
				playerChoice = false;
				break;
			case 'R':
			case 'r':
				attackHeap.push_back(35); //Frost
				attackHeap.push_back(23); //MonsterSwing
				playerChoice = false;
				break;
			case 'F':
			case 'f':
				attackHeap.push_back(15); //Fireball
				attackHeap.push_back(24); //MonsterSwing
				playerChoice = false;
				break;
			default:
				break;
		}
		//If done selecting start heapBattle(Player &), recover some wizzard mana
		if(!warriorTurn && !playerChoice) {
			player.getWizzard().recoverMana();
			heapBattle(player);
			clear();
		}
	} else if (playerDead){
		switch(input) {
			case 'Q':
			case 'q':
				refresh();
				wait_tick(300000);
				endwin();
				exit(EXIT_SUCCESS);
			default:
				break;
		}
	} else{
		//Can only be reached at battleText screen.
		switch(input) {
			case 'c':
			case 'C':
				if(player.getWarrior().getHealth() == 0 && player.getWizzard().getHealth() == 0){
					playerDead = true;
					clear();
					break;
				}
				playerChoice = true;
				warriorTurn = true;
				clear();
				refresh();
				if (enemy.mHealth() == 0){
					wonBattle(player);
					battle = false;
					clear();
				}
				break;
			default:
				break;
		}
	}
};

/* 
 * Battle Hit Chance Guid
 * -----------------------
 * Characters
 * -----------------------   ------------------------
 *  Warrior							Wizzard
 *  ----------------------   -----------------------
 *  Dagger - 90% 				Lightning -  90%  - 0 Mana
 *	Sword  - 75%				Frost     -	 80%  - 20 Mana - 25% chance to freeze
 *								Fire      -  70%  - 30 Mana
 * -----------------------
 *  Monsters
 *  ---------------------
 *  Goblin - 90%
 *  Orc    - 80%
 *  DragonKin  - 70%
 *  DragonKin Fireball - 50%
 *  Dragon     - 75%
 *  Dragon Fireball - 70%
 */

//The higher number attacks get pushed off the heap first
//Add attack text to a battleText vector to be displayed after heap is empty
void Battle::heapBattle(Player &player) {
	std::make_heap(attackHeap.begin(), attackHeap.end());
	int attack = 0, hitChance = 0;
	//Bools to make sure no extra messages pushed into battleText
	bool warriorDeath = false, wizzardDeath = false, frozenMessage = false;
	string attackText = "";
	battleText.clear();
	frozenEnemy = false;
	while(!attackHeap.empty()) {
		attackText = "";
		attack = attackHeap.front();
		//Player Attack Chance
		hitChance = (rand()%100)+1;
		
		//Warrior Attacks
		if((attack == 10 || attack == 40) && player.getWarrior().getHealth() != 0 ) {
			//Handle Sword Attack
			if(attack == 10 && hitChance <=75) {
				attackText = "The Warrior swung his sword, hitting the " + enemy.name() + " for " + to_string(player.getWarrior().swordAttackDamage()) + " damage.";
				enemy.takeDamage(player.getWarrior().swordAttackDamage());
			} else if(attack == 10) {
				attackText = "The Warrior swung his sword and missed!";
			}
			
			//Handle Dagger Attack
			if(attack == 40 && hitChance <= 90) {
				attackText = "The Warrior stabbed swiftly with his dagger! " +  to_string(player.getWarrior().daggerAttackDamage()) + " damage!";
				enemy.takeDamage(player.getWarrior().daggerAttackDamage());
			} else if (attack == 40){
				attackText = "Enemy avoided the swift dagger stab!";
			}
		} else if (player.getWarrior().getHealth() == 0 && !warriorDeath) {
			attackText = "The Warrior has collapsed and cannot attack!";
			warriorDeath = true;
		}

		//Wizzard Attacks
		if((attack == 15 || attack == 30 || attack == 35) && player.getWizzard().getHealth() != 0){
			//Handle FireBall
			if(attack == 15 && player.getWizzard().useMana(30)) {
				if(hitChance <= 75) {
					attackText = "The Wizzard cast a fireball! " + enemy.name() + " burned for " + to_string(player.getWizzard().fireDamage()) + ".";
					enemy.takeDamage(player.getWizzard().fireDamage());
				} else {
					attackText = enemy.name() + " evaded the Wizzard's fireball!";
				}
			} else if (attack == 15){
				attackText = "Not enough mana for a fireball!";
			}

			//Handle Lightning Bolt
			if(attack == 30 && hitChance <= 90) {
				attackText = "Lightning Bolt! Lightning Bolt! Enemy took " + to_string(player.getWizzard().lightningDamage()) + " damage.";
				enemy.takeDamage(player.getWizzard().lightningDamage());
			} else if (attack == 30) {
				attackText = enemy.name() + " was too swift for that lightning.";
			}

			//Handle Frost Attack
			if(attack == 35 && player.getWizzard().useMana(20)) {
				if(hitChance <= 80) {
					attackText = "Frost bolt hit " + enemy.name() + ". " + to_string(player.getWizzard().frostDamage()) + " damage.";
					if(hitChance <= 25) {
						attackText += " Enemy frozen!";
						frozenEnemy = true;
					}
					enemy.takeDamage(player.getWizzard().frostDamage());
				} else {
					attackText = "Frost attack missed!";
				}
			} else if(attack == 35) {
				attackText = "Not enough mana for frost attack!";
			}
		} else if(player.getWizzard().getHealth() == 0 && !wizzardDeath) {
			attackText = "The Wizzard has fainted and can no longer battle!";
			wizzardDeath = true;
		}

		//Store Battle Text
		if(attackText.size() > 0) battleText.push_back(attackText);

		attackText = "";
		//Monster Attack

		//Roll hit chance for monster
		hitChance = (rand()%100)+1;

		if(attack >= 20 && attack <= 29 && !frozenEnemy && enemy.mHealth() != 0) {
			//Goblin
			if(enemy.name() == "Goblin") {
				if(hitChance <= 90) {
					if(hitChance%2 == 0 || player.getWarrior().getHealth() == 0) {
						attackText = "Golbin took a swing at the Wizzard! " + to_string(enemy.attackDamage()) + " damage.";
						player.getWizzard().takeDamage(enemy.attackDamage());
					} else {
						attackText = "Goblin bit the Warrior! " + to_string(enemy.attackDamage()) + " damage.";
						player.getWarrior().takeDamage(enemy.attackDamage());
					}
				} else {
					attackText = "The clumbsy goblin swiped and missed!";
				}
			}
			//Orc
			else if (enemy.name() == "Orc") {
				if(hitChance <= 80) {
					if(hitChance%2 == 0 || player.getWarrior().getHealth() == 0) {
						attackText = "Orc charged the Wizzard! " + to_string(enemy.attackDamage()) + " damage.";
						player.getWizzard().takeDamage(enemy.attackDamage());
					} else {
						attackText = "Orc slammed his hammer into the Warrior! " + to_string(enemy.attackDamage()) + " damage.";
						player.getWarrior().takeDamage(enemy.attackDamage());
					}
				} else {
					attackText = "Orc missed his attack!";
				}
			}
			//TODO make a case for DragonKin and Dragon
		}
		//Check for Frozen Monster
		else if (frozenEnemy && enemy.mHealth() != 0 && !frozenMessage) {
			//Enemy is Frozen this round!
			attackText  = "Enemy is frozen solid and cannot attack!";
			frozenMessage = true;
		}
		//If enemy is dead end the battle
		else if (enemy.mHealth() == 0) {
			attackText = "The " + enemy.name() + " was slain! Your party received " + to_string(enemy.giveExp()) + " exp points.";
			if(attackText.length() > 0) battleText.push_back(attackText);
			attackHeap.clear();
			break;
		}

		//Store Battle Text
		if(attackText.length() > 0) battleText.push_back(attackText);
			
		//Remove item from heap
		std::pop_heap(attackHeap.begin(), attackHeap.end()); attackHeap.pop_back();
	}
};

//Used to return battle bool to GameSystem.h. 
bool Battle::inBattle() {
	return battle;
};

//Give exp and a key if monster has one
void Battle::wonBattle(Player &player) {
	player.getWarrior().addExp(enemy.giveExp());
	player.getWizzard().addExp(enemy.giveExp());
	if(enemy.key()) player.addKey();
};
