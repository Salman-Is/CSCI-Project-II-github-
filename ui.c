#include "battle.h"
#include "main.h"
#include "ui.h"
#include "events.h"

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
void printUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment,int playerHP,int playerMaxHP)
{
    char* locColor = areaColor();
    printf("It's the %s%s%s's turn...\n\n", RED, enemyName, NORMAL);
    printf("══════════════════════════════════════════════════════════════════════\n\n");
    printf("  [ %s%s%s ] [ %s%s%s ] [ %s%s%s ]\n", RED, enemyName, NORMAL, (alignment==GOOD?CYAN:RED), (alignment==GOOD?"GOOD":"EVIL"), NORMAL, locColor, currentLoc, NORMAL);
    printf("  Health: [ %02d/%d ] Damage: [ %02d ] Drop: [ %s%s%s ]\n\n", enemyHP, enemyMaxHP, currentEnemyATK, YELLOW, currentEnemyDrop, NORMAL);
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                    ║\n");
    printf("║ [ %sThe Paladin%s ]                                                    ║\n",BLUE, NORMAL);
    printf("║ Health: [ %02d/%d ] Arrow: [ %02d ] Sword: [ %02d ]                      ║\n", playerHP, playerMaxHP, maxPlayerTurnDamage, maxTurnDamage);
    printf("║                                                                    ║\n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
}

void printPlayerUI(char* enemyName,int enemyHP,int enemyMaxHP, int alignment,int playerHP,int playerMaxHP)
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
    printf("║ [ %sThe Paladin%s ]                                                    ║\n",BLUE, NORMAL);
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
    printf("Name:        %sThe Paladin%s\n", BLUE, NORMAL);
    printf("%sKarma:       %d%s\n", karmaColor, karma, NORMAL);
    printf("Coins:       %d\n", coins);
    printf("\n");
    printf("Sword:       %s\n", currentSword);
    printf("Bow:         %s\n", currentBow);
    printf("Armor:       %s\n", currentArmor);
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


