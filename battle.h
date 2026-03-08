#ifndef BATTLE_H
#define BATTLE_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>



int runBattle(char* enemyName, int difficultyLevel, int patternLength, int alignment, double sec_to_wait);
void printUI(char* enemyName, int enemyHP, int enemyMaxHP, int alignment, char* playerName, int playerHP, int playerMaxHP);
void printPlayerUI(char* enemyName, int enemyHP, int enemyMaxHP, int alignment, char* playerName, int playerHP, int playerMaxHP);
void openInventory(int inBattle, int *playerHP, int playerMaxHP);

void addCoins(int amount, char message[]);
void statsPage();

/* ================= MONSTERS ================= */
// Using structs to hold monsters and stats
// https://www.youtube.com/watch?v=dqa0KMSMx2w
typedef struct {
    char name[30];
    int difficultyLevel;
    int patternLength;
    int hp;
    int atk;
    int alignment;
    char drop[30];
} Monster;

void addItem(char* itemName, int startingItems);
void removeItem(char* itemName);
void grantKarma(int addOrSubtract, int amount, char message[]);
void encounter(Monster area[], int count);
int questGauntlet(Monster area[], int count, char groupName[], char locationName[]);

#define FOREST_COUNT sizeof(forest)/sizeof(forest[0])
#define PLAINS_COUNT sizeof(plains)/sizeof(plains[0])
#define LAKE_COUNT sizeof(lake)/sizeof(lake[0])
#define CAVES_COUNT sizeof(caves)/sizeof(caves[0])
#define MOUNTAINS_COUNT sizeof(mountains)/sizeof(mountains[0])
#define TEST sizeof(test)/sizeof(test[0])

#endif 