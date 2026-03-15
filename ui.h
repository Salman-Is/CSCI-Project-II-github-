#ifndef UI_H
#define UI_H

extern char currentLoc[];

void printUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment,int playerHP,int playerMaxHP);
void printPlayerUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment,int playerHP,int playerMaxHP);
void openInventory(int inBattle, int *playerHP, int playerMaxHP);
void statsPage();
void healthBar(int currentHP, int maxHP);
char* areaColor();

typedef struct {
    char name[64];
    char description[500];
    char color[16]; // WHITE, GREEN, BLUE, PURPLE, GOLD, YELLOW
} Item;

#endif 