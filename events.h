#ifndef EVENTS_H
#define EVENTS_H

void loreTablet(char text[]);
void genericLoreResponse();
void riddle(char message[], char correct[], int *search, char reward[]);
void chest(char key[], char item[], char chestType[], char chestColor[], int *search);
int dice();
char* questAlignment(char goodOption[], char evilOption[]);
void questRewards(char* rewards[], int count, int money);

void dialougeBox(char* name, char* color, char* tag);
int dialouge();
void searchArea();

#endif