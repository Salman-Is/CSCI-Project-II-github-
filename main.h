
#ifndef MAIN_H
#define MAIN_H
#include "defs.h"


#define GOOD 1
#define EVIL -1

void pressEnter();
void playerAl();
void randomEncounter();


// the storyProgress variable tracks where the player is in the story
// storyProgress = 0 means you are at the tutorial area, 1 means you are in area 1 etc.
extern int storyProgress;
extern int location; // 1=forest, 2=plains, 3=lake 

extern int battleStart;

// ik it's lazy to do this but ill do it anyway lol
extern int currentEnemyHP;
extern int currentEnemyATK;
extern int currentEnemyALIGNMENT;
extern char currentEnemyDrop[32];

/* ================= PLAYER STATS ================= */
extern int karma;
// extern int playerMaxHP;
extern int coins;
extern int karmaAtkBoost;
extern int karmaHpBoost;

extern int trueSight;
extern int dialougeSpeed;

extern char playerAlignment[];

extern Inventory inventory[100];
extern int inventoryCount;

extern int worldState;

/* ================= BASIC MONSTERS =====================*/
extern Monster forest[];
extern Monster plains[];
extern Monster lake[];
extern Monster caves[];
extern Monster mountains[];
extern Monster final[];

#endif 
