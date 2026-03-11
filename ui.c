#include "battle.h"
#include "main.h"
#include "ui.h"

/*
    >>>>>>>>>>>>>>>> TODO LIST <<<<<<<<<<<<<<<<<

    - Finish STAT screen
    - Add more events
    - Better travelling system (only to area you alread went to)
    - (You guys can create new features if you're up for it)
*/

char* areaColor();

char currentLoc[32] = "Forest";

/* ================= UI FUNCTIONS ================= */
void printUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment, char* playerName,int playerHP,int playerMaxHP)
{
    char* locColor = areaColor();
    printf("It's the %s%s%s's turn...\n\n", RED, enemyName, NORMAL);
    printf("══════════════════════════════════════════════════════════════════════\n\n");
    printf("  [ %s%s%s ] [ %s%s%s ] [ %s%s%s ]\n", RED, enemyName, NORMAL, (alignment==GOOD?CYAN:RED), (alignment==GOOD?"GOOD":"EVIL"), NORMAL, locColor, currentLoc, NORMAL);
    printf("  Health: [ %02d/%d ] Damage: [ %02d ] Drop: [ %s%s%s ]\n\n", enemyHP, enemyMaxHP, currentEnemyATK, YELLOW, currentEnemyDrop, NORMAL);
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                    ║\n");
    printf("║ [ %s%s%s ]                                                    ║\n",BLUE, playerName, NORMAL);
    printf("║ Health: [ %02d/%d ] Arrow: [ %02d ] Sword: [ %02d ]                      ║\n", playerHP, playerMaxHP, maxPlayerTurnDamage, maxTurnDamage);
    printf("║                                                                    ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
}

void printPlayerUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment, char* playerName,int playerHP,int playerMaxHP)
{
    char* locColor = areaColor();
    if(battleStart == 0){
        char* alignmentText = (alignment == GOOD) ? "GOOD" : "EVIL";
        char* alignmentColor = (alignment == GOOD) ? CYAN : RED;

        printf("You encountered a %s [%s%s%s]\n\n", enemyName, alignmentColor, alignmentText, NORMAL);

        battleStart=1;
    }
    else if (battleStart == 1){
        printf("Its your turn...\n\n");
    }
    printf("══════════════════════════════════════════════════════════════════════\n\n");
    printf("  [ %s%s%s ] [ %s%s%s ] [ %s%s%s ]\n", RED, enemyName, NORMAL, (alignment==GOOD?CYAN:RED), (alignment==GOOD?"GOOD":"EVIL"), NORMAL, locColor, currentLoc, NORMAL);
    printf("  Health: [ %02d/%d ] Damage: [ %02d ] Drop: [ %s%s%s ]\n\n", enemyHP, enemyMaxHP, currentEnemyATK, YELLOW, currentEnemyDrop, NORMAL);
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                    ║\n");
    printf("║ [ %s%s%s ]                                                    ║\n",BLUE, playerName, NORMAL);
    printf("║ Health: [ %02d/%d ] Arrow: [ %02d ] Sword: [ %02d ]                      ║\n", playerHP, playerMaxHP, maxPlayerTurnDamage, maxTurnDamage);
    printf("║                                                                    ║\n");
    printf("╠════════════════════════════════════════════════════════════════════╣\n");
    printf("║ [ 1 | FIRE ARROW ]     [ 2 | USE ITEM ]     [ 3 | SPARE CREATURE ] ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
}

void openInventory(int inBattle, int *playerHP, int playerMaxHP) // instead of 2 inventorys, use 1 for both battle and overworld
{
    system("cls");
    if (battleStart == 1)
    {
        printf("╔═════════════════════════════════════╗\n");
        printf("[#] | INVENTORY                       ║\n");
        printf("╚═════════════════════════════════════╝\n");
        for(int i = 0; i < inventoryCount; i++)
        {
            if (strcmp(inventory[i].name, "Health Potion") == 0) {
                printf("[%d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
            }
            else if (strcmp(inventory[i].name, "Health Elixer") == 0) {
                printf("[%d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
            }
            else if (strcmp(inventory[i].name, "Focus Charm") == 0) {
                printf("[%d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
            }
            if (strcmp(inventory[i].name, "Berzerker Potion") == 0) {
                printf("[%d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
            }
        }        
        printf("╔═════════════════════════════════════╗\n");             				 
        printf("╚═════════════════════════════════════╝\n");
        printf("Select item number to use (0 to exit): ");

        int choice;
        scanf("%d", &choice);
        if(choice == 0){
            return;
        }
        choice--; // < this genuinley took me an hour to figure out
        if (choice > inventoryCount){
            printf("Invalid choice.\n");
            return;
        }
        if(strcmp(inventory[choice].name, "Health Potion") == 0)
        {
            system("cls");
            *playerHP += 5;
            if (*playerHP > playerMaxHP){
                *playerHP = playerMaxHP;
            }
            return;
        }
        if(strcmp(inventory[choice].name, "Health Elixer") == 0){
            system("cls");
            removeItem("Health Elixer");
            *playerHP += 10;
            if (*playerHP > playerMaxHP){
                *playerHP = playerMaxHP;
            }
            return;
        }
        if(strcmp(inventory[choice].name, "Focus Charm") == 0){
            system("cls");
            removeItem("Focus Charm");
            critDamage = 2;
            return;
        }
        if(strcmp(inventory[choice].name, "Berzerker Potion") == 0){
            system("cls");
            removeItem("Berzerker Potion");
            attackBuff = 2;
            return;
        }
    }
    else {
        printf("╔═════════════════════════════════════╗\n");
        printf("[#] | INVENTORY                       ║\n");
        printf("╚═════════════════════════════════════╝\n");
        for(int i = 0; i < inventoryCount; i++)
        {
            printf("[%d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
        }                     				 
        printf("╔═════════════════════════════════════╗\n");             				 
        printf("╚═════════════════════════════════════╝\n");
        printf("Select item number to use (0 to exit): ");

        int choice;
        scanf("%d", &choice);
    }
}

void statsPage() {
    char* karmaColor;
    if (karma == 0) {
        karmaColor = RED;
    }
    else if (karma == 100){
        karmaColor = BLUE;
    }
    else {
        karmaColor = NORMAL;
    }
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║  CHARACTER STATS                          ║\n");
    printf("╚═══════════════════════════════════════════╝\n");
    printf("Name:        %s%s%s\n", BLUE, playerName, NORMAL);
    printf("%sKarma:       %d%s\n", karmaColor, karma, NORMAL);
    printf("Coins:       %d\n", coins);
    printf("\n");
    printf("╔═══════════════════════════════════════════╗\n");             				 
    printf("╚═══════════════════════════════════════════╝\n");
    printf("\nPress ENTER to continue...");
    getchar();
    getchar();
    // Add other stats
    // Should look like this when done
    // Karma between 0-33 -> EVIL, 34-66 -> NEUTRAL, 67(lol)-100 -> GOOD;
    /*
    <===========================>
    CHARACTER STATS
    <===========================>

    Name:        The Paladin
    Location:    Forest
    Karma:       65
    Coins:       100

    Armor: (name of armor) -> +X health 
    Sword: (name of sword) -> +X damage
    Bow: (name of bow) -> +X damage

    Alignment:   NEUTRAL                // <- would be cool to change color (cyan, white, red)

    <===========================>

    Press ENTER to return...
    
    */
}

void healthBar(int enemyHP,int enemyMaxHP, int alignment,int playerHP,int playerMaxHP) {
    // looks like this: HP: 20/25 [####################-----]
    // idk maybe it's unecessary but it would be cool
}

char* areaColor() {
    switch (location)
    {
    case 1:
        strcpy(currentLoc, "Forest");
        return GREEN;

    case 2:
        strcpy(currentLoc, "Plains");
        return LIME;

    case 3:
        strcpy(currentLoc, "Lake");
        return CYAN;

    case 4:
        strcpy(currentLoc, "Caves");
        return DARKBLUE;

    case 5:
        strcpy(currentLoc, "Mountains");
        return BLACK;

    case 6:
        strcpy(currentLoc, "Acention");
        return LILAC;

    default:
        strcpy(currentLoc, "Unknown");
        return NORMAL;
    }
}

/*================= EVENT FUNCTIONS ================= */

// In game events found by searching, quests, etc.
// The ascii art for these were made by chatgpt by the way (except the chest & dice)
void loreTablet(char text[]) {
    printf("╔══════════════╗\n");
    printf("║              ║\n");
    printf("║   ═ ══ ═══   ║\n");
    printf("║   ═══ ════   ║\n");
    printf("║   ═══  ═══   ║\n");
    printf("║   ════════   ║\n");
    printf("║   ═══  ═══   ║\n");
    printf("║   ══ ═════   ║\n");
    printf("║              ║\n");
    printf("╠══════════════╣\n");
    printf("╚══════════════╝\n\n");
    printf("You found a stone tablet!\n");
    printf("Its words are thus...\n\n");

    printf("\"%s%s%s\"\n", ITALIC, text, UNTALIC);
}
// Some lore tablets have specific replies, but most don't
// This is to save time so I dont have to type responses ever time theres a new tablet
void genericLoreResponse() {
    char messages[5][64] = {
    "\nYou ponder it's meaning...\n",
    "\nThe words make you think...\n",
    "\nIt makes you question what came before...\n",
    "\nYou contemplate its significance...\n",
    "\nThe tablet sparks a quiet curiosity...\n"
    };

    int messagesIndex = rand() % 5;
    printf("%s\n", messages[messagesIndex]);
}

void riddle(char message[], char correct[], int *search, char reward[]) {
    printf("          ╔════════════╗\n");
    printf("         ╔══════════════╗\n");
    printf("       ╔══════════════════╗\n");
    printf("       ║      ╔════╗      ║\n");
    printf("       ║      ║    ║      ║\n");
    printf("       ║    ╔═╩════╩═╗    ║\n");
    printf("       ║    ║        ║    ║\n");
    printf("       ║    ║        ║    ║\n");
    printf("       ╠════╩════════╩════╣\n");
    printf("       ║                  ║\n");
    printf("       ╚══════════════════╝\n\n");
    printf("You come across an Ancient Shrine...\n");
    printf("This is the riddle etched into its surface...\n\n");
    printf("%s%s%s\n", GREEN, message, NORMAL);
    printf("What is your answer?\n> ");
    char answer[99];
    fgets(answer, 99, stdin);
    fgets(answer, 99, stdin);
    // doing fgets twice made it work for some reason????
    // it had some strange behavior where it sent previous inputs as the answer automatically
    // no idea why this works

    for (int i = 0; answer[i] != '\0'; i++) {
        answer[i] = tolower(answer[i]);
    }
    time_t start_time, current_time;
    printf("Your answer: %s", answer);

    system("cls");

    for (int i = 0; i < 2; i++)
    {
        printf("          ╔════════════╗\n");
        printf("         ╔══════════════╗\n");
        printf("       ╔══════════════════╗\n");
        printf("   o   ║      ╔════╗      ║   o\n");
        printf("  o  · ║      ║    ║      ║ ·  ·\n");
        printf("    ·  ║    ╔═╩════╩═╗    ║  o\n");
        printf("     o ║    ║        ║    ║   ·\n");
        printf("  ·  . ║    ║        ║    ║    o\n");
        printf("       ╠════╩════════╩════╣\n");
        printf("       ║                  ║\n");
        printf("       ╚══════════════════╝\n\n");

        printf("The stone tablet atop the Shrine pedestal vibrates...\n");

        time(&start_time);
        do {
            time(&current_time);
        } while (difftime(current_time, start_time) < .5);

        system("cls");

        printf("          ╔════════════╗\n");
        printf("         ╔══════════════╗\n");
        printf("    o  ╔══════════════════╗\n");
        printf(" ·     ║      ╔════╗      ║ .    ·\n");
        printf("    ·  ║      ║    ║      ║  o\n");
        printf(" o     ║    ╔═╩════╩═╗    ║    ·\n");
        printf("   ·   ║    ║        ║    ║ o\n");
        printf("  .  o ║    ║        ║    ║   ·\n");
        printf("       ╠════╩════════╩════╣\n");
        printf("       ║                  ║\n");
        printf("       ╚══════════════════╝\n\n");

        printf("The stone tablet atop the Shrine pedestal vibrates...\n");

        time(&start_time);
        do {
            time(&current_time);
        } while (difftime(current_time, start_time) < .5);

        system("cls");
    }

    system("cls");

    printf("          ╔════════════╗\n");
    printf("         ╔══════════════╗\n");
    printf("       ╔══════════════════╗\n");
    printf("       ║      ╔════╗      ║\n");
    printf("       ║      ║····║      ║\n");
    printf("       ║    ╔═╩════╩═╗    ║\n");
    printf("       ║    ║  ·  ·  ║    ║\n");
    printf("       ║    ║  ·  ·  ║    ║\n");
    printf("       ╠════╩════════╩════╣\n");
    printf("       ║                  ║\n");
    printf("       ╚══════════════════╝\n\n");
    // https://www.youtube.com/watch?v=V32VSYVd1Ro
    // https://stackoverflow.com/questions/26821468/strstr-null-doesnt-work < THANK YOU to these dudes i was stuck here for 2 hours

    if (strstr(answer, correct) != NULL)
    {
        printf("The Shrine accepts your answer...\n");
        printf("It rewarded you with a %s%s%s\n\n", YELLOW, reward, NORMAL);
        (*search)++;
        addItem(reward, 1);
    }
    else {
        printf("The Shrine doesn't accept your answer...\n\n");
    }
}

void chest(char key[], char item[], char chestType[], char chestColor[], int *search) {
    printf("╔══════════════════╗\n");
    printf("╠═════╠══════║═════╣\n");
    printf("║                  ║\n");
    printf("║    [        ]    ║\n");
    printf("╚══════════════════╝\n\n");
    int hasFoundKey = -1;
    printf("You find a %s%s%s chest.\n", chestColor, chestType, NORMAL);
    
    for(int i = 0; i < inventoryCount; i++) {
        if(strcmp(inventory[i].name, key) == 0) {
            hasFoundKey = 1;
            break;
        }
        else {
            hasFoundKey = 0;
        }
    }

    if (hasFoundKey == 1)
    {
        printf("You use your %s%s%s to open the chest...\n\n", chestColor, key, NORMAL);
        (*search)++;
        addItem(item, 0);
    }
    else {
        printf("Find a %s%s%s to open the chest.\n\n", chestColor, key, NORMAL);
    }
}

int dice()
{
    int roll = rand() % 6 + 1;
    
    // I know it looks strange, but this is an array of dice
    char faces[6][200] = {
        "           ╔═════════╗\n           ║         ║\n           ║    ○    ║\n           ║         ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║       ● ║\n           ║         ║\n           ║ ●       ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║       ● ║\n           ║    ○    ║\n           ║ ●       ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║ ●     ● ║\n           ║         ║\n           ║ ●     ● ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║ ●     ● ║\n           ║    ○    ║\n           ║ ●     ● ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║ ●     ● ║\n           ║ ●     ● ║\n           ║ ●     ● ║\n           ╚═════════╝"
    };

    system("cls");
    printf("%s\n", faces[roll-1]);

    return roll;
}

char* questAlignment(char goodOption[], char evilOption[]) {
    printf("\nAs The Paladin, it is your job to decide the fate of this world.\n");
    printf("In this moment, do you choose to be %sGOOD%s, or %sEVIL%s?\n\n", CYAN, NORMAL, RED, NORMAL);
    printf("[ GOOD | %s ]     [ EVIL | %s ]\n", goodOption, evilOption);
    printf("( Type GOOD or EVIL )\n");
    printf("> ");

    int chosen = 0;
    char choice[32] = "";
    scanf(" %s", choice);

    for(int i=0;i<(sizeof(choice)/sizeof(choice[0]));i++) {
        choice[i]=toupper(choice[i]);
    }

    if (strcmp(choice, "GOOD") == 0)
    {
        system("cls");
        printf("You have chosen the path of %sGOOD%s\n", CYAN, NORMAL);
        int chosen = 1;
        return "GOOD";
    }
    else if (strcmp(choice, "EVIL") == 0)
    {
        system("cls");
        printf("You have chosen the path of %sEVIL%s\n", RED, NORMAL);
        int chosen = 1;
        return "EVIL";
    }
    else {
        system("cls");
        printf("The path you seek does not exist...\n");
        return "bruh";
    }

}

void questRewards(char* rewards[], int count, int money) {
    for(int i = 0; i < count; i++)
    {
        addItem(rewards[i], 0);
    }
    addCoins(money, "battle");
}