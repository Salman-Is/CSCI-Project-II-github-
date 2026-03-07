#ifndef UI_H
#define UI_H

extern char currentLoc[];

void printUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment, char* playerName,int playerHP,int playerMaxHP);
void printPlayerUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment, char* playerName,int playerHP,int playerMaxHP);
void openInventory(int inBattle, int *playerHP, int playerMaxHP);
void statsPage();
void loreTablet(char text[]);
void genericLoreResponse();
void riddle(char message[], char correct[], int *iterate, char reward[]);
void chest(char key[], char item[], char chestType[], char chestColor[], int search);
char* areaColor();
char* questAlignment(char goodOption[], char evilOption[]);

#endif 