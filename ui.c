#include "battle.h"
#include "main.h"
#include "ui.h"
#include "events.h"


char* areaColor();
char* changeColor(StatusType status);
StatusType enemyStatus = NONE;
StatusType playerStatus = NONE;

char currentLoc[32] = "Forest";
/* ================= ITEM DEFINITIONS ================= */

// currently not implimented into the game at all
// probably gonna have to be a future addition lol

// Swords ------------------------------------------------------------------
Item ironSword = {"Iron Sword ( 5 )", "A basic, chipped sword found in a cave.", "Weapon", "Sword", WHITE, 5};
Item steelSword = {"Steel Sword ( 6 )", "A sturdy bow forged with iron and carbon.", "Weapon", "Sword", WHITE, 6};
Item goldSword = {"Gold Sword ( 8 )", "A heavy, oriental sword unfit for battle, though it still works.", "Weapon", "Sword", GREEN, 8};
Item grassBlade = {"Grass Blade ( 10 )", "A strange sword enchanted with natural magic.", "Weapon", "Sword", GREEN, 10};

// Bows ------------------------------------------------------------------
Item woodenBow = {"Wooden Bow ( 4 )", "A simple, useable bow found in a cave.", "Weapon", "Bow", WHITE, 4};
Item steelBow = {"Steel Bow ( 5 )", "A wooden bow reinforced with steel.", "Weapon", "Bow", WHITE, 5};
Item knightBow = {"Knight Bow ( 6 )", "The offical bow used by Knights of NAMEOFTHEKINGDOM.", "Weapon", "Bow", GREEN, 6, BLEED};
Item rimegrassBow = {"Rimegrass Bow ( 6 )", "A strange bow enchanted with natural magic.", "Weapon", "Bow", GREEN, 6, POISON};
Item flameBow = {"Flame Bow ( 7 )", "An enchanted bow that lights arrows on fire", "Weapon", "Bow", GREEN, 7, BURN};
Item frostBow = {"Frost Bow ( 9 )", "An enchanted bow that freezes the air around an arrow", "Weapon", "Bow", GREEN, 7, FROZEN};
Item ancientBow = {"Ancient Bow ( 14 )", "A bow forged in a age long past, by a civilization long forgotten", "Weapon", "Bow", PURPLE, 14, FEAR};

// Armor ------------------------------------------------------------------
// hp starts at 25, the number next to the name is how much it adds to your health, and the int at the end is your health after addition
Item chainArmor = {"Chain Armor ( 5 )", "A flimsy set of armor found in a cave.", "Armor", "Armor", WHITE, 25};
Item steelArmor = {"Steel Armor ( 7 )", "A set of armor forged with iron and carbon.", "Armor", "Armor", WHITE, 27};
Item knightArmor = {"Knight Armor ( 9 )", "The offical set of armor used by Knights of NAMEOFTHEKINGDOM.", "Armor", "Armor", GREEN, 29};

// Drops ------------------------------------------------------------------
Item gel = {"Gel", "An oozing mass with little use", "Drop", "NULL", WHITE, 0};
Item lumoraWing = {"Lumora Wing", "The fragile wing of a Lumora", "Drop", "NULL", WHITE, 0};
Item leather = {"Leather", "A supple material useful for crafting", "Drop", "NULL", WHITE, 0};
Item groblinTooth = {"Groblin Tooth", "A blunt tooth used for crushing prey", "Drop", "NULL", WHITE, 0};
Item emberScale = {"Ember Scale", "A firey scale with magical properties", "Drop", "NULL", GREEN, 0};

Item beastlyTooth[] = {"Beastly Tooth", "A sharp, serated tooth that causes infection", "Drop", "NULL", WHITE, 0};
Item nimoraWing[] = {"Nimora Wing", "The fragile wing of a Lumora", "Drop", "NULL", WHITE, 0};
Item trollLeather[] = {"Troll Leather", "Leather that has been hardened by Troll engineering", "Drop", "NULL", WHITE, 0};
Item fossilizedMoss[] = {"Fossilized Moss", "Hardened moss from the back of an ancient creature", "Drop", "NULL", GREEN, 0};
Item greatAntlers[] = {"Great Antlers", "Proof you took down a Great Stag", "Drop", "NULL", WHITE, 0};

Item shellShard[] = {"Shell Shard", "A sturdy shard of a Mega Turtle that is the base of Tide Armor", "Drop", "NULL", WHITE, 0};
Item venomVial[] = {"Venom Vial", "A drop of venom collected from a Lake Serpent", "Drop", "NULL", WHITE, 0};
Item krakenTentacle[] = {"Kraken Tentacle", "The arm of a Kraken useful for potions", "Drop", "NULL", WHITE, 0};
Item shinyScale[] = {"Shiny Scale", "A gleaming scale that can be used to craft charms", "Drop", "NULL", WHITE, 0};
Item gelatinousMass[] = {"Gelatinous Mass", "A large, oozing mass with little use", "Drop", "NULL", WHITE, 0};

// Special ------------------------------------------------------------------
Item verdentKey[] = {"Verdent Key", "An emerald key found in the Plains. It has to open something...", "Item", "Key", GREEN, 0};
Item frostKey[] = {"Frost Key", "A saffire key found in the Caves. It has to open something...", "Item", "Key", CYAN, 0};

// Potions/Charms --------------------------------------------------------------
Item healthPotion[] = {"Health Potion", "A potion that will heal most injuries.", "Potion", "Healing", WHITE, 5};
Item healthElixer[] = {"Health Potion", "A magical potion made with the blessing of Astra.", "Potion", "Healing", GREEN, 10};
Item berzerkerPotion[] = {"Berzerker Potion", "An unstable potion that draws out your deep power.", "Potion", "Attack+", GREEN, 2};
Item focusCharm[] = {"Focus Charm", "An artifact that brings with it a calming force.", "Charm", "Crit+", CYAN, 2};


/* ================= UI FUNCTIONS ================= */
void printUI(char* turn, char* enemyName, int enemyHP, int enemyMaxHP, int alignment, int playerHP, int playerMaxHP){
    char* enStatus = statusText(enemyStatus);
    char* enStatusColor = changeColor(enemyStatus);
    char* plStatus = statusText(playerStatus);
    char* plStatusColor = changeColor(playerStatus);
    char* locColor = areaColor();

    // Intro/ Turn message thing
    if (strcmp(turn, "enemy") == 0) {
        printf("It's the %s%s%s's turn...\n\n", RED, enemyName, NORMAL);
    }
    else if (strcmp(turn, "player") == 0) {
        if (battleStart == 0) {
            char* alignmentText = (alignment == GOOD) ? "GOOD" : "EVIL";
            char* alignmentColor = (alignment == GOOD) ? CYAN : RED;

            printf("A %s%s%s [ %s%s%s ] stands before you...\n\n", BOLD, enemyName, UNBOLD, alignmentColor, alignmentText, NORMAL);
            battleStart = 1;
        } else {
            printf("Its your turn...\n\n");
        }
    }
    // ENEMY BOX ===================================
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("                                                                      \n");
    printf("  [ %s%s%s ] [ %s%s%s ]\n", RED, enemyName, NORMAL,(alignment==GOOD?CYAN:RED),(alignment==GOOD?"GOOD":"EVIL"), NORMAL);
    printf("                                                                      \n");
    printf("  Status: [ %s%s%s ] Drop: [ %s%s%s ]\n",enStatusColor, enStatus, NORMAL, YELLOW, currentEnemyDrop, NORMAL);
    printf("                                                                      \n");
    printf("  HP: ");
    healthBar(enemyHP, enemyMaxHP, "enemy");
    printf(" Damage: [ %02d ]                           \n", currentEnemyATK);
    printf("                                                                      \n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");

    // PLAYER BOX =====================================================
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                    ║\n");
    printf("║ [ %sThe Paladin%s ] [ %s%s%s ]                                           ║\n", BLUE, NORMAL,
           ((strcmp(playerAlignment, "GOOD") ? CYAN :
            strcmp(playerAlignment, "EVIL") ? RED : NORMAL)),
           (strcmp(playerAlignment, "GOOD") ? "GOOD" :
            (playerAlignment == "EVIL" ? "EVIL" : "NEUT")), NORMAL);
    printf("║                                                                    ║\n");
    printf("║ Status: [ %s%s%s ]                                                   ║\n", plStatusColor, plStatus, NORMAL);
    printf("║                                                                    ║\n");
    printf("║ HP: [ %02d / %d ] ", playerHP, playerMaxHP);
    healthBar(playerHP, playerMaxHP, "player");
    printf(" Arrow: [ %02d ] Sword: [ %02d ]    ║\n",maxPlayerTurnDamage, maxTurnDamage);
    printf("║                                                                    ║\n");

    // PLAYER ACTION MENU (ONLY ON PLAYER TURN) ==================================
    if (strcmp(turn, "player") == 0) {
        printf("╠════════════════════════════════════════════════════════════════════╣\n");
        printf("║ [ 1 | FIRE ARROW ]     [ 2 | USE ITEM ]     [ 3 | SPARE CREATURE ] ║\n");
    }
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
}

void openInventory(int inBattle, int *playerHP, int playerMaxHP) // instead of 2 inventorys, use 1 for both battle and overworld
{
    system("cls");
    if (battleStart == 1)
    {
        printf("╔═════════════════════════════════════╗\n");
        printf("[#] | BATTLE ITEMS                    ║\n");
        printf("╚═════════════════════════════════════╝\n");
        for(int i = 0; i < inventoryCount; i++)
        {
            if (strcmp(inventory[i].name, "Health Potion") == 0) {
                printf("[%02d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
            }
            else if (strcmp(inventory[i].name, "Health Elixer") == 0) {
                printf("[%02d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
            }
            else if (strcmp(inventory[i].name, "Focus Charm") == 0) {
                printf("[%02d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
            }
            if (strcmp(inventory[i].name, "Berzerker Potion") == 0) {
                printf("[%02d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
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
        printf("╔══════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("[#] | INVENTORY                                                                    ║\n");
        printf("╚══════════════════════════════════════════════════════════════════════════════════╝\n");
        
        for(int i = 0; i < inventoryCount; i++){
            
            printf("[%02d] %-16s x%d   ", (i + 1), inventory[i].name, inventory[i].quantity);
            
                
            if ((i + 1) % 3 == 0) {
                printf("\n");
            }      
        }                     				 
        printf("\n╔══════════════════════════════════════════════════════════════════════════════════╗");             				 
        printf("\n╚══════════════════════════════════════════════════════════════════════════════════╝\n");
        printf("Select item number to use (0 to exit): ");

        int choice;
        scanf("%d", &choice);
        system("cls");
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
    printf("Armor:       %s\n\n", currentArmor);
    printf("Alignment:   NEUTRAL\n");
    printf("\n");
    printf("╔═══════════════════════════════════════════╗\n");             				 
    printf("╚═══════════════════════════════════════════╝\n");
    pressEnter();
    system("cls");
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

    Title:       [Something Cool]
    

    Armor: (name of armor) -> +X health 
    Sword: (name of sword) -> +X damage
    Bow: (name of bow) -> +X damage

    Alignment:   NEUTRAL                // <- would be cool to change color (cyan, white, red)

    <===========================>

    Press ENTER to return...
    
    */
}

char* areaColor() {
    switch (location)
    {
    case 1:
        strcpy(currentLoc, "Forest of Echoes");
        return GREEN;

    case 2:
        strcpy(currentLoc, "Verdent Plains");
        return LIME;

    case 3:
        strcpy(currentLoc, "Blue Lake");
        return CYAN;

    case 4:
        strcpy(currentLoc, "Crystal Caves");
        return DARKBLUE;

    case 5:
        strcpy(currentLoc, "Celestial Mountains");
        return BLACK;

    case 6:
        strcpy(currentLoc, "Acention");
        return LILAC;

    default:
        strcpy(currentLoc, "Unknown");
        return NORMAL;
    }
}

char* changeColor(StatusType status){
    switch(status){
        case POISON: 
            return PURPLE;
        case BURN: 
            return ORANGE;
        case FEAR: 
            return DARKBLUE;
        case FROZEN:
            return CYAN; 
        case BLEED: 
            return RED;
        default:
            return NORMAL; 
    }
}

/**
* This function creates an hp bar compatible with the player and enemy
* It runs an equation
* x/10 = currentHP/maxHP to find a sutible ratio to represent the current HP of whatever
* we put in the parameters of the function. x is basically the amount of filled bars 
* that should be printed, aka currentBars.
*/
void healthBar(int currentHP, int maxHP, char* isEnemy) {
    int maxBars = 15;
    int cuurentBars = (currentHP * maxBars) / maxHP;

    char* currentColor;
    if (cuurentBars >= 10){
        currentColor = BLUE;
    }
    else if (cuurentBars >= 5) {
        currentColor = GOLD;
    }
    else if (cuurentBars < 5){
        currentColor = DEEPRED;
    }
    if (strcmp(isEnemy, "enemy"))
    {
        if (trueSight == 1)
        {
            printf("HP: [ %d / %d ] ", currentHP, maxHP);
        }
        
    }
    printf("[ ");
    for (int i = 0; i < maxBars; i++) {
        if (i <= cuurentBars)
            printf("%s■%s", currentColor, NORMAL);  
        else
            printf("%s■%s", GRAY, NORMAL);
    }
    printf(" ]");
}

// i found out the sleep() function is better then the time() do while loop for the cpu. and its actually way simpler. 
// also using millesecods can allow us to use partial seconds. ex 500 milleseconds is .5 seconds. 1000 milleseconds is 1 sec
// https://www.geeksforgeeks.org/c/sleep-function-in-c/
