#ifndef EVENTS_H
#define EVENTS_H
#include "defs.h"
#include <string>

void loreTablet(string text);
void genericLoreResponse();
void riddle(string message, string correct, int *search, Item reward);
void chest(string key, Item item, string chestType, string chestColor, int *search);
int dice();
string questAlignment(string goodOption, string evilOption);
void questRewards(Item* rewards[], int count, int money);

int puzzleDoor(string correct[]);

void dialougeBox(string name, string color, string tag);
int dialouge();
void searchArea();
void shop(string items[], int prices[], int shop_count);

void upgradeSword(Item item);
void upgradeBow(Item item);
void upgradeArmor(Item newArmor);

#endif
