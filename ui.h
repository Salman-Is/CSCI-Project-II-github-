#ifndef UI_H
#define UI_H
#include "defs.h"

extern string currentLoc;
//extern string statusColor;

void printUI(string turn, string enemyName, int enemyHP, int enemyMaxHP, int alignment, int playerHP, int playerMaxHP);
void openInventory(int inBattle, int *playerHP);
void statsPage();
void healthBar(int currentHP, int maxHP, string isEnemy);
string areaColor();
string changeColor(StatusType status);
void specialPrintf(string text);

extern Item ironSword;
extern Item steelSword;
extern Item goldSword;
extern Item knightSword;
extern Item grassBlade;
extern Item flameBlade;
extern Item staffOfRemnant;
extern Item ancientSword;
 
 // Bows ------------------------------------------------------------------
extern Item woodenBow;
extern Item steelBow;
extern Item knightBow;
extern Item rimegrassBow;
extern Item flameBow;
extern Item frostBow;
extern Item ancientBow;

 // Armor ------------------------------------------------------------------
 // hp starts at 25, the number next to the name is how much it adds to your health, and the int at the end is your health after addition
extern Item chainArmor;
extern Item steelArmor;
extern Item knightArmor;
extern Item swordmasterArmor;

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
extern Item lichPhial;
 
extern Item shellShard;
extern Item venomVial;
extern Item krakenTentacle;
extern Item shinyScale;
extern Item gelatinousMass;

 // Special ------------------------------------------------------------------
extern Item verdentKey;
extern Item frostKey;
extern Item ancientCoin;
extern Item elderiteGemstone;
extern Item kingdomCrest;
extern Item ancientMap;

extern Item septre;
 
// Potions/Charms --------------------------------------------------------------
extern Item healthPotion;
extern Item healthElixer;
extern Item mysticSalve;
extern Item berzerkerPotion;
extern Item focusCharm;

// Debugging ---------------------------------------------------------------------
extern Item ultraSword;
extern Item ultraBow;
extern Item ultraArmor;

#endif 