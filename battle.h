#ifndef BATTLE_H
#define BATTLE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "defs.h"

extern int critDamage;
extern int attackBuff;

// typedef enum {
//     NONE, // does nothing
//     POISON, // damaged every turn (-15% hp)
//     BURN, // damaged every turn (-5 hp), attack reduced (-25%)
//     FEAR, // attack is halved
//     FROZEN, // 50% chance to not attack
//     BLEED, // damaged every turn (-25% hp), attack doubled
// } StatusType;

extern StatusType playerStatus;
extern StatusType enemyStatus;

int runBattle(Monster enemy, int difficultyLevel, int patternLength, int alignment, double sec_to_wait, int tutorial);

void addCoins(int amount, char message[]);

/* ================= MONSTERS ================= */
// Using structs to hold monsters and stats
// https://www.youtube.com/watch?v=dqa0KMSMx2w
// typedef struct {
//     char name[30];
//     int difficultyLevel;
//     int patternLength;
//     int hp;
//     int atk;
//     int alignment;
//     char drop[30];
// } Monster;

void addItem(char* itemName, int startingItems);
void removeItem(char* itemName);
void weaponUpgrade(char weaponName[], int weaponDamage);

void grantKarma(int addOrSubtract, int amount, char message[]);
void encounter(Monster area[], int count, int tutorial, int special);
int questGauntlet(Monster area[], int count, char groupName[], char locationName[]);

void fireArrow(int* enemyHP);

void applyStatus(StatusType* status, StatusType newStatus);
void processStatus(StatusType status, int* hp, int maxHP);
int canAct(StatusType status);
int modifyDamage(int baseDamage, StatusType status);
char* statusText(StatusType status);

#define FOREST_COUNT sizeof(forest)/sizeof(forest[0])
#define PLAINS_COUNT sizeof(plains)/sizeof(plains[0])
#define LAKE_COUNT sizeof(lake)/sizeof(lake[0])
#define CAVES_COUNT sizeof(caves)/sizeof(caves[0])
#define MOUNTAINS_COUNT sizeof(mountains)/sizeof(mountains[0])
#define FINAL_COUNT sizeof(final)/sizeof(final[0])

#endif