#include "battle.h"
#include "main.h"
#include "ui.h"
#include "events.h"
#include "defs.h"

string areaColor();
string changeColor(StatusType status);
StatusType enemyStatus = NONE;
StatusType playerStatus = NONE;

string currentLoc = "Forest";
/* ================= ITEM DEFINITIONS ================= */

// currently not implimented into the game at all
// probably gonna have to be a future addition lol

// Swords ------------------------------------------------------------------
Item ironSword = {"Iron Sword", "A basic, chipped sword found in a cave.", "Weapon", "Sword", WHITE, 5, NONE};
Item steelSword = {"Steel Sword", "A sturdy bow forged with iron and carbon.", "Weapon", "Sword", WHITE, 6, NONE};
Item goldSword = {"Gold Sword", "A heavy, oriental sword unfit for battle, though it still works.", "Weapon", "Sword", GREEN, 8, NONE};
Item knightSword = {"Knight Sword", "The offical sword used by Knights of Aureveil.", "Weapon", "Sword", GREEN, 8, BLEED};
Item grassBlade = {"Grass Blade", "A strange sword enchanted with natural magic.", "Weapon", "Sword", GREEN, 10, POISON};
Item flameBlade = {"Grass Blade", "An enchanted sword, lit ablaze.", "Weapon", "Sword", GREEN, 11, BURN};
Item ancientSword = {"Ancient Sword", "A strange sword enchanted with ancient magic.", "Weapon", "Sword", PURPLE, 19, FEAR};
Item staffOfRemnant = {"Staff of Kyra's Remnant", "A cursed spear created by Racher the Lich", "Weapon", "Sword", CYAN, 12, FEAR};

Item ultraSword = {"Ultra Sword", "An OP sword used for testing and nothing else", "Weapon", "Sword", RED, 999, FEAR};

// Bows ------------------------------------------------------------------
Item woodenBow = {"Wooden Bow", "A simple, useable bow found in a cave.", "Weapon", "Bow", WHITE, 4, NONE};
Item steelBow = {"Steel Bow", "A wooden bow reinforced with steel.", "Weapon", "Bow", WHITE, 5, NONE};
Item knightBow = {"Knight Bow", "The offical bow used by Knights of Aureveil.", "Weapon", "Bow", GREEN, 6, BLEED};
Item rimegrassBow = {"Rimegrass Bow", "A strange bow enchanted with natural magic.", "Weapon", "Bow", GREEN, 6, POISON};
Item flameBow = {"Flame Bow", "An enchanted bow that lights arrows on fire", "Weapon", "Bow", GREEN, 7, BURN};
Item frostBow = {"Frost Bow", "An enchanted bow that freezes the air around an arrow", "Weapon", "Bow", GREEN, 7, FROZEN};
Item ancientBow = {"Ancient Bow", "A bow forged in a age long past, by a civilization long forgotten", "Weapon", "Bow", PURPLE, 14, FEAR};

Item ultraBow = {"Ultra Bow", "An OP bow used for testing and nothing else", "Weapon", "Bow", RED, 999, FEAR};

// Armor ------------------------------------------------------------------
// hp starts at 25, the number next to the name is how much it adds to your health, and the int at the end is your health after addition
Item chainArmor = {"Chain Armor", "A flimsy set of armor found in a cave.", "Armor", "Armor", WHITE, 25};
Item steelArmor = {"Steel Armor", "A set of armor forged with iron and carbon.", "Armor", "Armor", WHITE, 27};
Item knightArmor = {"Knight Armor", "The offical set of armor used by Knights of Aureveil.", "Armor", "Armor", GREEN, 29};
Item swordmasterArmor = {"Swordmaster Armor", "A set of armor forged by the famous Swordmaster Lorel.", "Armor", "Armor", GREEN, 32};

Item ultraArmor = {"Ultra Armor", "An OP set of armor used for testing and nothing else", "Armor", "Armor", RED, 999};

// Drops ------------------------------------------------------------------
Item gel = {"Gel", "An oozing mass with little use", "Drop", "NULL", WHITE, 0};
Item lumoraWing = {"Lumora Wing", "The fragile wing of a Lumora", "Drop", "NULL", WHITE, 0};
Item leather = {"Leather", "A supple material useful for crafting", "Drop", "NULL", WHITE, 0};
Item groblinTooth = {"Groblin Tooth", "A blunt tooth used for crushing prey", "Drop", "NULL", WHITE, 0};
Item emberScale = {"Ember Scale", "A firey scale with magical properties", "Drop", "NULL", GREEN, 0};

Item beastlyTooth = {"Beastly Tooth", "A sharp, serated tooth that causes infection", "Drop", "NULL", WHITE, 0};
Item nimoraWing = {"Nimora Wing", "The fragile wing of a Lumora", "Drop", "NULL", WHITE, 0};
Item trollLeather = {"Troll Leather", "Leather that has been hardened by Troll engineering", "Drop", "NULL", WHITE, 0};
Item fossilizedMoss = {"Fossilized Moss", "Hardened moss from the back of an ancient creature", "Drop", "NULL", GREEN, 0};
Item greatAntlers = {"Great Antlers", "Proof you took down a Great Stag", "Drop", "NULL", WHITE, 0};
Item lichPhial = {"Lich's Phial", "Proof you took down a Great Stag", "Drop", "NULL", WHITE, 0};

Item shellShard = {"Shell Shard", "A sturdy shard of a Mega Turtle that is the base of Tide Armor", "Drop", "NULL", WHITE, 0};
Item venomVial = {"Venom Vial", "A drop of venom collected from a Lake Serpent", "Drop", "NULL", WHITE, 0};
Item krakenTentacle = {"Kraken Tentacle", "The arm of a Kraken useful for potions", "Drop", "NULL", WHITE, 0};
Item shinyScale = {"Shiny Scale", "A gleaming scale that can be used to craft charms", "Drop", "NULL", WHITE, 0};
Item gelatinousMass = {"Gelatinous Mass", "A large, oozing mass with little use", "Drop", "NULL", WHITE, 0};

// Special ------------------------------------------------------------------
Item verdentKey = {"Verdent Key", "An emerald key found in the Plains. It has to open something...", "Item", "Key", GREEN, 0};
Item frostKey = {"Frost Key", "A saffire key found in the Caves. It has to open something...", "Item", "Key", CYAN, 0};
Item ancientCoin = {"Ancient Coin", "", "Item", "NULL", PURPLE, 50};
Item elderiteGemstone = {"Elderite Gemstone", "", "Item", "NULL", CYAN, 50};
Item kingdomCrest = {"Kingdom Crest", "", "Item", "NULL", CYAN, 50};
Item ancientMap = {"Ancient Map", "", "Item", "NULL", CYAN, 0};

Item septre = {"Septre", "An artifact that grants the holder Divinity.", "Charm", "Status", GOLD, 0};

// Potions/Charms --------------------------------------------------------------
Item healthPotion = {"Health Potion", "A potion that will heal most injuries.", "Potion", "Healing", WHITE, 5};
Item healthElixer = {"Health Elixer", "A magical potion made with the blessing of Astra.", "Potion", "Healing", GREEN, 10};
Item mysticSalve = {"Mystic Salve", "An oinment imbued with the power of Regeneration.", "Potion", "Status", CYAN, 0};
Item berzerkerPotion = {"Berzerker Potion", "An unstable potion that draws out your deep power.", "Potion", "Attack+", GREEN, 2};
Item focusCharm = {"Focus Charm", "An artifact that brings with it a calming force.", "Charm", "Crit+", CYAN, 2};

/* ================= UI FUNCTIONS ================= */
void printUI(string turn, string enemyName, int enemyHP, int enemyMaxHP, int alignment, int playerHP, int playerMaxHP){
    string enStatus = statusText(enemyStatus);
    string enStatusColor = changeColor(enemyStatus);
    string plStatus = statusText(playerStatus);
    string plStatusColor = changeColor(playerStatus);
    string locColor = areaColor();

    // Intro/ Turn message thing
    if (turn == "enemy") {
        printf("It's the %s%s%s's turn...\n\n", RED, enemyName, NORMAL);
    }
    else if (turn == "player") {
        if (battleStart == 0) {
            string alignmentText = (alignment == GOOD) ? "GOOD" : "EVIL";
            string alignmentColor = (alignment == GOOD) ? CYAN : RED;

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
        ((playerAlignment == "GOOD") ? CYAN :(playerAlignment == "EVIL") ? RED : NORMAL), 
        ((playerAlignment == "GOOD") ? "GOOD" : (playerAlignment == "EVIL") ? "EVIL" : "NEUT"), NORMAL);
    printf("║                                                                    ║\n");
    printf("║ Status: [ %s%s%s ]                                                   ║\n", plStatusColor, plStatus, NORMAL);
    printf("║                                                                    ║\n");
    printf("║ HP: [ %02d / %d ] ", playerHP, playerMaxHP);
    healthBar(playerHP, playerMaxHP, "player");
    printf(" Arrow: [ %02d ] Sword: [ %02d ]    ║\n",currentBow.value, currentSword.value);
    printf("║                                                                    ║\n");

    // PLAYER ACTION MENU (ONLY ON PLAYER TURN) ==================================
    if (turn == "player") {
        printf("╠════════════════════════════════════════════════════════════════════╣\n");
        printf("║      [ 1 | ATTACK ]        [ 2 | ITEMS ]        [ 3 | SPARE ]      ║\n");
    }
    printf("╚════════════════════════════════════════════════════════════════════╝\n");
}

/**
 * Prints out inventory
 * Handles calculations for when you choose items in battle (for now)
 */
void openInventory(int inBattle, int *playerHP) // instead of 2 inventorys, use 1 for both battle and overworld
{
    system("cls");
    if (battleStart == 1){
        printf("╔═════════════════════════════════════╗\n");
        printf("[#] | BATTLE ITEMS                    ║\n");
        printf("╚═════════════════════════════════════╝\n");
        string battleItems[32] = {"Health Potion", "Health Elixer", "Focus Charm", "Berzerker Potion", "Mystic Salve"};
        int battleItemCount = sizeof(battleItems) / sizeof(battleItems[0]);
        for(int i = 0; i < inventoryCount; i++)
        {
            for (int j = 0; j < battleItemCount; j++)
            {
                if (inventory[i].name == battleItems[j]) {
                    printf("[%02d] %s x%d\n", (i + 1), inventory[i].name, inventory[i].quantity);
                    break;
                }
            }
        }              
        printf("╔═════════════════════════════════════╗\n");             				 
        printf("╚═════════════════════════════════════╝\n");
        printf("Select item number to use (0 to exit): ");

        int choice;
        scanf("%d", &choice);
        system("cls");
        if(choice == 0){
            return;
        }
        choice--; // < this genuinley took me an hour to figure out
        if (choice > inventoryCount){
            printf("Invalid choice.\n");
            return;
        }
        if(inventory[choice].name == "Health Potion")
        {
            system("cls");
            *playerHP += 5;
            if (*playerHP > currentArmor.value){
                *playerHP = currentArmor.value;
            }
            return;
        }
        if(inventory[choice].name == "Health Elixer"){
            system("cls");
            removeItem("Health Elixer");
            *playerHP += 10;
            if (*playerHP > currentArmor.value){
                *playerHP = currentArmor.value;
            }
            return;
        }
        if(inventory[choice].name == "Mystic Salve"){
            system("cls");
            removeItem("Berzerker Potion");
            applyStatus(&playerStatus, REGENERATION);
            return;
        }
        if(inventory[choice].name == "Focus Charm"){
            system("cls");
            removeItem("Focus Charm");
            critDamage = 2;
            return;
        }
        if(inventory[choice].name == "Berzerker Potion"){
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
    string karmaColor;
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
    printf("Armor:       %s -> x%d health\n", currentArmor.name, currentArmor.value);
    printf("Sword:       %s -> x%d damage\n", currentSword.name, currentSword.value);
    printf("Bow:         %s -> x%d damage\n\n", currentBow.name, currentBow.value);
    printf("Alignment:   %s\n", playerAlignment);
    printf("\n");
    printf("╔═══════════════════════════════════════════╗\n");             				 
    printf("╚═══════════════════════════════════════════╝\n");
    pressEnter();
    system("cls");
}

string areaColor() {
    switch (location)
    {
    case 1:
        currentLoc = "Forest of Echoes";
        return GREEN;

    case 2:
        currentLoc = "Verdent Plains";
        return LIME;

    case 3:
        currentLoc = "Blue Lake";
        return CYAN;

    case 4:
        currentLoc = "Crystal Caves";
        return DARKBLUE;

    case 5:
        currentLoc = "Celestial Mountains";
        return BLACK;

    case 6:
        currentLoc = "Acention";
        return LILAC;

    default:
        currentLoc = "Unknown";
        return NORMAL;
    }
}

string changeColor(StatusType status){
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
        case BLIND:
            return BLACK;
        case REGENERATION: 
            return PINK;
        case DIVINE: 
            return GOLD;
        default:
            return NORMAL; 
    }
}

// cool typewriter effect thing for story intro's
void specialPrintf(string text) {
    for (int i = 0; text[i] != '\0'; i++) {
        printf("%c", text[i]);
        fflush(stdout);
    if (text[i] != ' ')
        if (dialougeSpeed == 0) {
            Sleep(12);
        }
        else {
            Sleep(0);
        }
    }
}

/**
* This function creates an hp bar compatible with the player and enemy
* It runs an equation
* x/10 = currentHP/maxHP to find a sutible ratio to represent the current HP of whatever
* we put in the parameters of the function. x is basically the amount of filled bars 
* that should be printed, aka currentBars.
*/
void healthBar(int currentHP, int maxHP, string isEnemy) {
    int maxBars = 15;
    int cuurentBars = (currentHP * maxBars) / maxHP;

    string currentColor;
    if (cuurentBars >= 10){
        currentColor = BLUE;
    }
    else if (cuurentBars >= 5) {
        currentColor = GOLD;
    }
    else if (cuurentBars < 5){
        currentColor = DEEPRED;
    }
    if (isEnemy == "enemy"){
        if (trueSight == 1){
            printf("[ %d / %d ] ", currentHP, maxHP);
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
