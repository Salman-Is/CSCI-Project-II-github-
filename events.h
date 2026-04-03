#ifndef EVENTS_H
#define EVENTS_H
#include "defs.h"

void loreTablet(char text[]);
void genericLoreResponse();
void riddle(char message[], char correct[], int *search, Item reward);
void chest(char key[], Item item, char chestType[], char chestColor[], int *search);
int dice();
char* questAlignment(char goodOption[], char evilOption[]);
void questRewards(Item* rewards[], int count, int money);

int puzzleDoor(char* correct[]);

void dialougeBox(char* name, char* color, char* tag);
int dialouge();
void searchArea();
void shop(char* items[], int prices[], int shop_count);

void upgradeSword(Item item);
void upgradeBow(Item item);
void upgradeArmor(Item newArmor);

#endif
