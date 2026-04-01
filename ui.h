#ifndef UI_H
#define UI_H
#include <windows.h>
#include "defs.h"

extern char currentLoc[];
//extern char* statusColor;

void printUI(char* turn, char* enemyName, int enemyHP, int enemyMaxHP, int alignment, int playerHP, int playerMaxHP);
void openInventory(int inBattle, int *playerHP, int playerMaxHP);
void statsPage();
void healthBar(int currentHP, int maxHP, char* isEnemy);
char* areaColor();
char* changeColor(StatusType status);


// typedef struct Item_struct{
//     char name[64];
//     char description[500];
//     char itemCatagorey[16]; // weapon, armor, potion etc
//     char itemType[16]; // sword, bow, healing etc
//     char color[16]; // WHITE, GREEN, CYAN, PURPLE, GOLD, YELLOW
//     int value; // defense, healing, damage etc. 
//     int amount;
//     StatusType status; // additional effects
// } Item;


extern Item ironSword;
extern Item steelSword;
extern Item goldSword;
extern Item grassBlade;
extern Item ancientSword;
 
 // Bows ------------------------------------------------------------------
extern Item woodenBow;
extern Item steelBow;
extern Item knightBow;
extern Item rimegrassBow;
extern Item flameBow;
extern Item ancientBow;

 // Armor ------------------------------------------------------------------
 // hp starts at 25, the number next to the name is how much it adds to your health, and the int at the end is your health after addition
extern Item chainArmor;
extern Item steelArmor;
extern Item knightArmor;
 

 // Drops ------------------------------------------------------------------
extern Item gel;
extern Item lumoraWing;
extern Item leather;
extern Item groblinTooth;
extern Item emberScale;

extern Item beastlyTooth;
extern Item nimoraWing;
extern Item trollLeather;
extern Item fossilizedMoss;
extern Item greatAntlers;
 
extern Item shellShard;
extern Item venomVial;
extern Item krakenTentacle;
extern Item shinyScale;
extern Item gelatinousMass;

 // Special ------------------------------------------------------------------
extern Item verdentKey;
extern Item frostKey;
extern Item ancientCoin;
 
// Potions/Charms --------------------------------------------------------------
extern Item healthPotion;
extern Item healthElixer;
extern Item berzerkerPotion;
extern Item focusCharm;




#endif 