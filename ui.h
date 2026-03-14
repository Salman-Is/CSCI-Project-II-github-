#ifndef UI_H
#define UI_H

extern char currentLoc[];

void printUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment,int playerHP,int playerMaxHP);
void printPlayerUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment,int playerHP,int playerMaxHP);
void openInventory(int inBattle, int *playerHP, int playerMaxHP);
void statsPage();
char* areaColor();

#endif 