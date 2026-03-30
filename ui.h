#ifndef UI_H
#define UI_H
#include <windows.h>

extern char currentLoc[];
//extern char* statusColor;

void printUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment,int playerHP,int playerMaxHP);
void printPlayerUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment,int playerHP,int playerMaxHP);
void openInventory(int inBattle, int *playerHP, int playerMaxHP);
void statsPage();
void healthBar(int currentHP, int maxHP);
char* areaColor();
void timer(int milliseconds);

typedef struct {
    char name[64];
    char description[500];
    char itemCatagorey[16]; // weapon, armor, potion etc
    char itemType[16]; // sword, bow, healing etc
    char color[16]; // WHITE, GREEN, CYAN, PURPLE, GOLD, YELLOW
    int value; // defense, healing, damage etc. 
    int amount;
} Item;

<<<<<<< HEAD
#endif 
=======


#endif 
>>>>>>> ff1ec958b1dc107bc1b7a743a372636add2a868d
