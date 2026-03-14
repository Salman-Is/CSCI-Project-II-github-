#include "battle.h"
#include "main.h"
#include "ui.h"
#include "events.h"
/*
    BATTLE script 

/*
    Refactored battle system from memory game

    - Its based on counterattacks
    - The enemy gives you a pattern
    - Waits a few seconds then clears the terminal
    - And asks you to recreate it
    - You take damage for every wrong letter, and deal damage for every right letter
    - Details on the text page
    - Lowkey I was way too confused on how to use multiple scripts soooo it's all on one for now
*/
/*
    >>>>>>>>>>>>>>>> TODO LIST <<<<<<<<<<<<<<<<<

    - Compress other switch cases (Prof Dan's feedback from submission 1)
    - Take damage if you don't input pattern in time
    - Item remove function
    - Real weapon system that upgrades when you find better weapons
    - Finish STAT screen
    - Story
    - SEPARATE SCRIPTS (800 lines already ToT)
        - battle.c --> runBattle(), encounter(), printUI(), printPlayerUI(), addItem(), removeItem()
        - enemies.c --> Monster structs
        - main.c --> openInventory(), loreTablet(), statsPage(), genericLoreResponse()
    - More regular monsters
    - Boss monsters
    - Tutorial (and a way for the player to choose a name)
    - Consequences of Karma (details on text page)
    - Better travelling system (only to area you alread went to)
    - (You guys can create new features if you're up for it)
*/

/*
    >>>>>>>>>>>>>>>> TODO LIST <<<<<<<<<<<<<<<<<

    - Take damage if you don't input pattern in time
    - Weapon Upgrade function (checks if your damage would benifit from this new weapon before changing damage)
    - (You guys can create new features if you're up for it)
*/

int critDamage = 1.5;
int attackBuff = 1;

/* ================= ITEM FUNCTIONS ================= */
// addItem("ITEM NAME", X); X = 1 if its starting inventory, X = 0 if not
void addItem(char* itemName, int startingItems) {
    // loop cycles through inventory 
    // if the new item has the same name as an item thats already in the
    // inventory, it adds it to the stack of items already there
    // if the new item was never obtained before it creates a new stack

    int found = -1;

    // check if item already exists
    for(int i = 0; i < inventoryCount; i++)
    {
        if(strcmp(inventory[i].name, itemName) == 0)
        {
            found = i;
            break;
        }
    }
    // if found, increase quantity
    if(found != -1)
    {
        inventory[found].quantity++;
        if (startingItems == 0) {
            printf("You obtained another %s%s%s!\n", YELLOW, itemName, NORMAL);
        }
    }
    // if not found, create new stack
    else if(inventoryCount < 100){
        strcpy(inventory[inventoryCount].name, itemName);
        inventory[inventoryCount].quantity = 1;
        inventoryCount++;
        if (startingItems == 0)
        {
            printf("You obtained: %s%s%s!\n", YELLOW, itemName, NORMAL);
        }
    }
    else
    {
        printf("Inventory full!\n");
    }
}

void removeItem(char* itemName) {
    // loop cycles through inventory 
    // if the chosen item has the same name as an item thats already in the
    // inventory, it removes it from the stack of items already there
    // if there is onyl one of the chosen item it deletes the stack

    int found = -1;

    // find the item
    for(int i = 0; i < inventoryCount; i++)
    {
        if(strcmp(inventory[i].name, itemName) == 0)
        {
            found = i;
            break;
        }
    }

    if(found != -1)
    {
        inventory[found].quantity--;

        // if stack becomes empty, remove it
        if(inventory[found].quantity <= 0)
        {
            for(int i = found; i < inventoryCount - 1; i++)
            {
                inventory[i] = inventory[i + 1];
            }
            inventoryCount--;
        }

        printf("You used a %s%s%s.\n", YELLOW, itemName, NORMAL);
    }
    else
    {
        printf("Item not found in inventory.\n");
    }
}

void newWeapon(char weaponName[], int weaponDamage) {
    // check if new weapon damage is more than maxTurnDamage/maxPlayerTurnDamage
    // Only change maxTurnDamage/maxPlayerTurnDamage to weaponDamage IF its more than maxTurnDamage/maxPlayerTurnDamage
}
/* ================= ENCOUNTER FUNCTION ================= */
void encounter(Monster area[], int count, int tutorial)
{
    // tutorial = 1
    Monster enemy = area[rand() % count];
    
    currentEnemyATK = enemy.atk;
    currentEnemyHP = enemy.hp;
    strcpy(currentEnemyDrop, enemy.drop);

    // Determine alignment text (so sparing makes sense to the player)
    char* alignmentText = (enemy.alignment == GOOD) ? "GOOD" : "EVIL";
    char* alignmentColor = (enemy.alignment == GOOD) ? CYAN : RED;

    currentEnemyALIGNMENT = enemy.alignment;
    
    printf("A %s [%s%s%s] appears before you!\n", enemy.name, alignmentColor, alignmentText, NORMAL);
    int result;
    if (tutorial != 1)
    {
        result = runBattle(enemy.name, enemy.difficultyLevel, enemy.patternLength, enemy.alignment, 3.0, 0);
    }
    else if (tutorial == 1) {
        result = runBattle(enemy.name, enemy.difficultyLevel, enemy.patternLength, enemy.alignment, 3.0, 1);
    }
    
    
    // check if battle won --> check alignment --> check karma -->
    // max karma is 100, min karma is 0
    if (result == 1)
    {
        addItem(enemy.drop, 0);
        addCoins(5, "battle");
        printf("You defeated the %s%s%s!\n\n", alignmentColor, enemy.name, NORMAL);
        if (enemy.alignment == GOOD) {
            printf("You killed a %sGOOD%s creature!\n", CYAN, NORMAL);
            grantKarma(0, 5, "An immense guilt weighs upon your soul...");
        }
        else {
            printf("You killed an %sEVIL%s creature!\n", RED, NORMAL);
            grantKarma(1, 5, "Justice lifted a weight from your soul...");
        }
    } 
    else if (result == 2) {
        printf("You spared the %s%s%s!\n", alignmentColor, enemy.name, NORMAL);

        if (enemy.alignment == GOOD)
        {
            printf("A %sGOOD%s creature was allowed to live!\n", CYAN, NORMAL);
            grantKarma(1, 1, "Justice lifted a weight from your soul...");
        }
        else {
            printf("You let something %sEVIL%s return to the world...\n", RED, NORMAL);
            grantKarma(0, 1, "An immense guilt weighs upon your soul...");
        } 
    }
    else 
    {
        printf("You were defeated by the %s...\n\n", enemy.name);
        if (karma > 60)
        {
            grantKarma(0, 10, "This death angers your soul...");
        }
        else if (karma < 40)
        {
            grantKarma(1, 10, "This death makes you empathize with those who can't rise again...");
        }
    }

    printf("Current Karma: %d\n", karma);
    printf("\nPress ENTER to continue...");
    // https://stackoverflow.com/questions/18801483/press-any-key-to-continue-function-in-c
    getchar(); 
    getchar();
}

int questGauntlet(Monster area[], int count, char groupName[], char locationName[]) {
    time_t start_time, current_time;
    printf("A quest gauntlet begins!\n");
    printf("You must defeat every %s in %s...\n\n", groupName, locationName);

    time(&start_time);
    do {
        time(&current_time);
    } while (difftime(current_time, start_time) < 3);

    for(int i = 0; i < count; i++)
    {
        Monster enemy = area[i];
        
        currentEnemyATK = enemy.atk;
        currentEnemyHP = enemy.hp;
        strcpy(currentEnemyDrop, enemy.drop);

        // Determine alignment text (so sparing makes sense to the player)
        char* alignmentText = (enemy.alignment == GOOD) ? "GOOD" : "EVIL";
        char* alignmentColor = (enemy.alignment == GOOD) ? CYAN : RED;

        currentEnemyALIGNMENT = enemy.alignment;
        
        printf("A %s [%s%s%s] appears before you!\n", enemy.name, alignmentColor, alignmentText, NORMAL);

        int result = runBattle(enemy.name, enemy.difficultyLevel, enemy.patternLength, enemy.alignment, 3.0, 0);

        if (result == 1)
        {
            addItem(enemy.drop, 0);
            addCoins(5, "battle");
            printf("You defeated the %s%s%s!\n\n", alignmentColor, enemy.name, NORMAL);
            if (enemy.alignment == GOOD) {
                printf("You killed a %sGOOD%s creature!\n", CYAN, NORMAL);
                grantKarma(0, 5, "An immense guilt weighs upon your soul...");
            }
            else {
                printf("You killed an %sEVIL%s creature!\n", RED, NORMAL);
                grantKarma(1, 5, "Justice lifted a weight from your soul...");
            }
        } 
        else if (result == 2) {
            printf("You spared the %s%s%s!\n", alignmentColor, enemy.name, NORMAL);

            printf("The enemies overrun you. Quest failed...\n\n");
            return 2;
        }
        else 
        {
            printf("You were defeated by the %s...\n\n", enemy.name);
            if (karma > 60)
            {
                grantKarma(0, 10, "This death angers your soul...");
            }
            else if (karma < 40)
            {
                grantKarma(1, 10, "This death makes you empathize with those who can't rise again...");
            }
            return 0;
        }

        printf("\nPrepare for the next enemy...\n");
        printf("Press ENTER to continue...");
        getchar();
        getchar();
        system("cls");
    }

    printf("You cleared the entire gauntlet!\n\n");
    return 1;
}
/* ================= MAIN BATTLE FUNCTION ================= */
/**
 * Function for battle logic
 * @param enemyName -> tracks the current enemy
 * @param difficultyLevel -> 
 */
int runBattle(char* enemyName, int difficultyLevel, int patternLength, int alignment, double sec_to_wait, int tutorial)
{
    char pattern[10];       // Enemy's attack pattern
    char user_pat[20];     // Player input
    char letters[14] = {'X','O','A','B','C','D','E','F','G','H', 'I', 'W', 'Y', 'Z'}; // Possible letters for patterns

    int enemyMaxHP = currentEnemyHP;

    // reset buffs
    critDamage = 1.5;
    attackBuff = 1;

    // 0 if not players turn
    int playerTurn = 1;

    int playerHP = playerMaxHP;
    int enemyHP  = enemyMaxHP;
    int enemyDamagePerLetter = currentEnemyATK / patternLength;
    time_t start_time, current_time;
    int spare = 0;
    system("cls");

    /* =================== BATTLE LOOP =================== */
    while (playerHP > 0 && enemyHP > 0 && spare == 0){
        if (spare == 0){
            if (playerTurn == 0){
                // RAGE mechanic (EVIL exclusive ability)
                if((enemyHP < enemyMaxHP / 2) && currentEnemyALIGNMENT == EVIL){
                    printf("\nThe enemy is filled with %sRage%s...\n", RED, NORMAL);
                    enemyDamagePerLetter += 1;
                    printf("Enemy damage + 1\n\n");
                }
                // BLESSING mechanic (GOOD exclusive ability)
                if((enemyHP < enemyMaxHP / 2) && currentEnemyALIGNMENT == GOOD){
                    int blessingChance = (rand() % 100 + 1);
                    // 50% for the it to work, 50% for it to fail
                    if (blessingChance >= 50) {
                        printf("\nThe enemy recives a %sBlessing%s!\n", CYAN, NORMAL);
                        enemyHP += (enemyHP/2);
                        printf("Enemy health +25%%\n\n");
                    }
                    else {
                        continue;
                    }
                }
                // ----------------- Generate Enemy Pattern -----------------
                // (thanks Dan)
                for (int i = 0; i < patternLength; i++) {
                    pattern[i] = letters[(rand() % (difficultyLevel * 2))];
                }
                pattern[patternLength]='\0';

                if (tutorial == 1){
                    printf("═════════════════════════════════════════════════════════════════════════════════════\n");
                    printf("         Enemies will attack you in specific patterns, represented by letters.       \n");
                    printf("                 In order to use your Sword to counter the attack,                   \n");
                    printf("                you must replicate the pattern after it disappears                   \n");
                    printf("═════════════════════════════════════════════════════════════════════════════════════\n");
                    tutorial = 0;
                    printf("Press ENTER to continue...");
                    getchar();
                    getchar();
                    system("cls");
                }

                // ----------------- UI -----------------
                printUI(enemyName, enemyHP, enemyMaxHP, alignment, playerHP, playerMaxHP);

                time(&start_time);
                do {
                    time(&current_time);
                } while (difftime(current_time, start_time) < 3);
                

                printf("\nEnemy prepares an attack!\n");
                printf("Counter this pattern:\n[ ");
                printf(RED);
                printf(BOLD);
                for(int i=0;i<patternLength;i++) {
                    printf("%c ", pattern[i]);
                }
                printf(NORMAL);
                printf(UNBOLD);
                printf("]\n");

                // ----------------- Timer -----------------
                time(&start_time);
                do {
                    time(&current_time);
                } while (difftime(current_time, start_time) < 3);

                system("cls");
                printUI(enemyName, enemyHP, enemyMaxHP, alignment, playerHP, playerMaxHP);

                // ----------------- Player Counter Input -----------------
                printf("\nEnter counter sequence: ");
                scanf(" %s", user_pat);

                int correct = 0;

                for(int i=0;i<patternLength;i++) {
                    user_pat[i]=toupper(user_pat[i]);
                }
                for(int i=0;i<patternLength;i++) {
                    if(user_pat[i]==pattern[i]) correct++;
                }
                // ----------------- Damage Calculation -----------------
                // changed because of int division rounding bug
                int damageToEnemy = (correct * maxTurnDamage * attackBuff) / patternLength;                 
                int damageToPlayer = ((patternLength - correct) * currentEnemyATK) / patternLength; 

                // juuust in case (idk if this would matter but it might still round to 0)
                if (correct > 0 && damageToEnemy == 0) {
                    damageToEnemy = 1;
                }

                // perfect counter system
                if (correct == patternLength){
                    printf("%sPERFECT COUNTER!%s\n", BLUE, NORMAL);
                    damageToEnemy = (damageToEnemy*critDamage) + 2;
                }

                // again, this might never happen but I don't want to math it out lol
                if (correct == patternLength && damageToEnemy == 0) {
                    damageToEnemy = 2;
                }
                    
                enemyHP -= damageToEnemy;
                playerHP -= damageToPlayer;

                printf("\nYou dealt %d damage to the %s!\n", damageToEnemy, enemyName);
                printf("You took %d damage!\n", damageToPlayer);

                // ----------------- Pause -----------------
                time(&start_time);
                do {
                    time(&current_time);
                } while (difftime(current_time, start_time) < 3);  
                system("cls");
                playerTurn = 1;
            }
            else if (playerTurn == 1){
                if (tutorial == 1){
                    printf("════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
                    printf("                                 On your turn, you are given 3 choices.                                       \n\n");
                    printf("                      1. Firing an arrow deals damage based on your current Bow ATK                             \n");
                    printf("             2. Using an item will give you acess to healing, potions, and artifacts to help you                \n");
                    printf("      3. Sparing a creature will let the creature go, but it doesn't always work on an %sEVIL%s creature...   \n\n", RED, NORMAL);
                    printf("           Killing or Sparing monsters will have consequences depending on the creatures ALIGNMENT...           \n");
                    printf("                            Be sure your KARMA stays where you want it to be...                                 \n");
                    printf("════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n\n");
                    printf("Press ENTER to continue...");
                    getchar();
                    getchar();
                    system("cls");
                }
                printPlayerUI(enemyName, enemyHP, enemyMaxHP, alignment, playerHP, playerMaxHP);
                printf("Fire an arrow to deal damage, use an item, or spare the creature?\n");
                printf("> ");
                // 1 = fire arrow, 2 = use item 3 = spare
                char turnChoice = '0';

                scanf(" %c", &turnChoice);
                if (turnChoice == '1') {
                    system("cls");
                    printf("You shot an arrow!\n");
                    enemyHP -= maxPlayerTurnDamage;
                    playerTurn = 0;
                }
                else if (turnChoice == '2') {
                    system("cls");
                    printf("You chose to use an item!\n");
                    openInventory(0, &playerHP, playerMaxHP);
                    playerTurn = 0;
                }
                else if (turnChoice == '3'){
                    system("cls");
                    if (currentEnemyALIGNMENT == EVIL){
                        int randomValue = (rand() % 100 + 1);
                        // 40% for the run to work, 60% for it to fail
                        if (randomValue >= 60) {
                            spare = 1;
                        }
                        else {
                            printf("The %s refused to be spared!\n", enemyName);
                        }
                    }
                    else {
                        spare = 1;
                    }
                    playerTurn = 0;
                }
                
                else {
                    printf("Add this in later lolll\n");
                }
            }
            int x;  
        }
    }

    // ----------------- Return Battle Result -----------------
    if (spare == 1) {
        battleStart=0;
        return 2;
    }
    if(playerHP > 0){
        battleStart=0; 
        return 1;
    }
    else {
        battleStart=0; 
        return 0;
    }
}

void grantKarma(int addOrSubtract, int amount, char message[]) { //subtract = 0, add = anthing else
    if (addOrSubtract == 0){
        karma -= amount;
        if (karma < 0)
        {
            karma = 0;
            printf("The Deity of %sEVIL%s recognizes your cruelty...\n\n", RED, NORMAL);
            addItem("Essence", 0);
        }
        printf("\n%s\n\n", message);
        printf("-%d Karma\n", amount);
    }
    else {
        karma += amount;
        if (karma > 100)
        {
            karma = 100;
            printf("The Deity of %sGOOD%s presents you with a reward...\n\n", CYAN, NORMAL);
            addItem("Ichor", 0);
        }
        printf("%s\n\n", message);
        printf("+%d Karma\n", amount);
    }
}

void addCoins(int amount, char message[]) {
    if (strcmp(message, "battle") == 0)
    {
        printf("You obtained %d %sCoins%s!\n", amount, NEONYELLOW, NORMAL);
        coins += amount;
    }
    else {
        printf("You found %d %sCoins%s!\n", amount, NEONYELLOW, NORMAL);
        coins += amount;
    }
}
