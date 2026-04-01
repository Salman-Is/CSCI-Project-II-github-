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

void randomEffect();

/* ================= ITEM FUNCTIONS ================= */
// addItem("ITEM NAME", X); X = 1 if its starting inventory, X = 0 if not
void newAddItem(Item item, int startingItems) {

}

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
            printf("You obtained another %s%s%s!\n", GOLD, itemName, NORMAL);
        }
    }
    // if not found, create new stack
    else if(inventoryCount < 100){
        strcpy(inventory[inventoryCount].name, itemName);
        inventory[inventoryCount].quantity = 1;
        inventoryCount++;
        if (startingItems == 0)
        {
            printf("You obtained: %s%s%s!\n", GOLD, itemName, NORMAL);
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
void encounter(Monster area[], int count, int tutorial, int special){
    // tutorial = 1, make special neg if rand, index of area if specific
    Monster enemy;
    if (special < 0)
    {
        enemy = area[rand() % count];
    }
    else {
        enemy = area[special];
    }
    
    currentEnemyATK = enemy.atk;
    currentEnemyHP = enemy.hp;
    strcpy(currentEnemyDrop, enemy.drop);

    // Determine alignment text (so sparing makes sense to the player)
    char* alignmentText = (enemy.alignment == GOOD) ? "GOOD" : "EVIL";
    char* alignmentColor = (enemy.alignment == GOOD) ? CYAN : RED;

    currentEnemyALIGNMENT = enemy.alignment;
    
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
        printf("You defeated the %s%s%s!\n\n", alignmentColor, enemy.name, NORMAL);
        addItem(enemy.drop, 0);
        addCoins(5, "battle");
        printf("\n");
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
    pressEnter();
}

int questGauntlet(Monster area[], int count, char groupName[], char locationName[]) {
    printf("A quest gauntlet begins!\n");
    printf("You must defeat every %s in %s...\n\n", groupName, locationName);

    Sleep(3000);

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
        pressEnter();
        system("cls");
    }

    printf("You cleared the entire gauntlet!\n\n");
    return 1;
}
/* ================= MAIN BATTLE FUNCTION ================= */
/**
 * Function for battle logic
 * 
 * 
 */
int runBattle(char* enemyName, int difficultyLevel, int patternLength, int alignment, double sec_to_wait, int tutorial){
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
    int spare = 0;

    system("cls");

    /* =================== BATTLE LOOP =================== */
    while (playerHP > 0 && enemyHP > 0 && spare == 0){
        if (spare == 0){
            if (enemyHP <= 0 || playerHP <= 0){
                break;
            }
            if (playerTurn == 0){
                processStatus(enemyStatus, &enemyHP, enemyMaxHP);
                if (canAct(enemyStatus) == 0) {
                    printf("Enemy couldn't move!\n");
                    playerTurn = 1;
                    continue;
                }
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
                    pressEnter();
                    system("cls");
                }

                // ----------------- UI -----------------
                printUI("enemy", enemyName, enemyHP, enemyMaxHP, alignment, playerHP, playerMaxHP);

                Sleep(3000);
    
                printf("\nEnemy prepares an attack!\n");
                printf("Counter this pattern:\n%s[ ", BOLD);
                printf(RED);
                for(int i=0;i<patternLength;i++) {
                    printf("%c ", pattern[i]);
                }
                printf(NORMAL);
                printf("]\n");
                printf(UNBOLD);

                // ----------------- Timer -----------------
                Sleep(3000);

                system("cls");
                printUI("enemy", enemyName, enemyHP, enemyMaxHP, alignment, playerHP, playerMaxHP);

                // ----------------- Player Counter Input -----------------
                printf("\nEnter counter sequence: %s%s", BOLD, BLUE);
                scanf(" %s", user_pat);
                printf("%s%s", UNBOLD, NORMAL);

                int correct = 0;

                for(int i=0;i<patternLength;i++) {
                    user_pat[i]=toupper(user_pat[i]);
                }
                for(int i=0;i<patternLength;i++) {
                    if(user_pat[i]==pattern[i]) correct++;
                }
                // ----------------- Damage Calculation -----------------
                // changed because of int division rounding bug
                int damageToEnemy = (correct * currentSword.value * attackBuff) / patternLength;                 
                int damageToPlayer = ((patternLength - correct) * currentEnemyATK) / patternLength; 

                // juuust in case (idk if this would matter but it might still round to 0)
                if (correct > 0 && damageToEnemy == 0) {
                    damageToEnemy = 1;
                }

                // again, this might never happen but I don't want to math it out lol
                if (correct == patternLength && damageToEnemy == 0) {
                    damageToEnemy = 2;
                }

                damageToPlayer = modifyDamage(damageToPlayer, enemyStatus);
                damageToEnemy = modifyDamage(damageToEnemy, playerStatus);
                    
                enemyHP -= damageToEnemy;
                playerHP -= damageToPlayer;

                // perfect counter system
                if (correct == patternLength){
                    printf("You counter the attack with your %s%s%s%s%s...\n", BLUE, BOLD, currentSword, UNBOLD, NORMAL);
                    Sleep(1500);
                    printf("%sPERFECT COUNTER!%s\n", BLUE, NORMAL);
                    damageToEnemy = (damageToEnemy*critDamage) + 2;
                }

                printf("\nYou dealt %d damage to the %s!\n", damageToEnemy, enemyName);
                printf("You took %d damage!\n", damageToPlayer);

                // ----------------- Pause -----------------
                Sleep(3000);  
                system("cls");

                playerTurn = 1;
            }
            else if (playerTurn == 1){
                processStatus(playerStatus, &playerHP, playerMaxHP);
                if (canAct(playerStatus) == 0) {
                    printf("You couldn't move!\n");
                    playerTurn = 0;
                    continue;
                }
                if (tutorial == 1){
                    printf("════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
                    printf("                                 On your turn, you are given 3 choices.                                       \n\n");
                    printf("                      1. Firing an arrow deals damage based on your current Bow ATK                             \n");
                    printf("             2. Using an item will give you acess to healing, potions, and artifacts to help you                \n");
                    printf("      3. Sparing a creature will let the creature go, but it doesn't always work on an %sEVIL%s creature...   \n\n", RED, NORMAL);
                    printf("           Killing or Sparing monsters will have consequences depending on the creatures ALIGNMENT...           \n");
                    printf("                            Be sure your KARMA stays where you want it to be...                                 \n");
                    printf("════════════════════════════════════════════════════════════════════════════════════════════════════════════════\n\n");
                    pressEnter();
                    system("cls");
                }
                printUI("player", enemyName, enemyHP, enemyMaxHP, alignment, playerHP, playerMaxHP);
                printf("Fire an arrow to deal damage, use an item, or spare the creature?\n");
                printf("> ");
                // 1 = fire arrow, 2 = use item 3 = spare
                char turnChoice = '0';

                scanf(" %c", &turnChoice);
                if (turnChoice == '1') {
                    fireArrow(&enemyHP);
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
                    system("cls");
                    printf("You made an invalid move.\nYou gave your enemy a chance to strike...\n\n");
                    playerHP -= currentEnemyATK;
                    printf("You took %d damage!\n", currentEnemyATK);
                    playerTurn = 0;
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

void grantKarma(int addOrSubtract, int amount, char message[]) { //subtract = 0, add = anything else
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
        printf("You obtained %d %sCoins%s!\n", amount, YELLOW, NORMAL);
        coins += amount;
    }
    else {
        printf("You found %d %sCoins%s!\n", amount, YELLOW, NORMAL);
        coins += amount;
    }
}
/*
void fireArrow(int* enemyHP, int enemyMaxHP)
{
    system("cls");

    printf("You draw your %s...\n", currentBow.name);

    // deal damage
    int damage = currentBow.value;
    *enemyHP -= damage;

    printf("You dealt %d damage!\n", damage);

    // apply status effect if it exists
    if (currentBow.effect != NONE)
    {
        printf("The arrow inflicts %s!\n", statusText(currentBow.effect));
        applyStatus(&enemyStatus, currentBow.effect);
    }

    // clamp HP
    if (*enemyHP < 0) *enemyHP = 0;
}
*/

void fireArrow(int* enemyHP)
{
    // deal damage
    int damage = currentBow.value;
    damage = modifyDamage(damage, playerStatus);
    if (damage < 1) damage = 1;  // just in case
    *enemyHP -= damage;

    printf("\nYou draw your %s%s%s%s%s...\n", BOLD, currentBow.color, currentBow.name, UNBOLD, NORMAL);
    printf("Your arrow dealt %d damage!\n", damage);
    Sleep(3000);
    system("cls");

    // apply status effect if it exists
    if (currentBow.status != NONE){
        printf("The arrow inflicts %s%S%s!\n", changeColor(currentBow.status), statusText(currentBow.status), NORMAL);
        applyStatus(&enemyStatus, currentBow.status);
    }

    if (*enemyHP < 0) *enemyHP = 0;
}

/* ================= STATUS EFFECT FUNCTIONS ================= */

// I made them work for the player as well, but let's keep it exclusive to enemies for now
// It's a bit complex, but I made all the different kinds of effects int different functions
// Just in case we want to add more later on

// changes chosen status to new status
void applyStatus(StatusType* status, StatusType newStatus){
    *status = newStatus;
}

/*
After checking that the enemy is still alive AND has a status, this function handles the effects
for poison, and half the effect of bleed and burn. Basically, this handles the "damaging" effects
*/
void processStatus(StatusType status, int* hp, int maxHP){
    if (*hp <= 0){
        return;
    }
    if (status == NONE) {
        return;
    }
    int damage = 0;

    switch (status){
        case POISON:
            damage = (int)(maxHP * 0.15);
            if (damage < 1) damage = 1;
            printf("%s%s", BOLD, PURPLE);
            printf("The Poison eats away at %s... (-%d HP)\n\n",(status == playerStatus ? "you" : "the enemy") ,damage);
            *hp -= damage;
            printf("%s%s", UNBOLD, NORMAL);
            break;

        case BURN:
            damage = (int)(maxHP * 0.10);
            printf("%s%s", BOLD, ORANGE);
            printf("The Flames scorch %s... (-%d HP)\n\n",(status == playerStatus ? "you" : "the enemy") ,damage);
            printf("%s%s", UNBOLD, NORMAL);
            *hp -= damage;
            break;

        case BLEED:
            damage = (int)(maxHP * 0.25);
            if (damage < 1) damage = 1;
            printf("%s%s", BOLD, RED);
            printf("%s to Bleed out... (-%d HP)\n\n", (status == playerStatus ? "You continue" : "The enemy continues") ,damage);
            printf("%s%s", UNBOLD, NORMAL);
            *hp -= damage;
            break;

        case FEAR:
            printf("%s%s", BOLD, DARKBLUE);
            printf("Fear lingers in %s...\n\n", (status == playerStatus ? "your heart" : "the heart of your enemy"));
            printf("%s%s", UNBOLD, NORMAL);
            break;

        case FROZEN:
            break;

        case NONE:
        default:
            break;
    }

    // clamp HP
    if (*hp < 0) *hp = 0;
}

// Right now, this is just for freeze, but maybe later we can add paralyze like Pokemon
// 50/50 chance the enemy can't move 
int canAct(StatusType status){
    if (status == FROZEN){
        int chance = rand() % 100;
        if (chance < 33){
            printf("%s%s", BOLD, CYAN);
            printf("%s Frozen...\n\n", (status == playerStatus ? "You are" : "The enemy is"));
            printf("%s%s", UNBOLD, NORMAL);
            return 0;
        }
    }
    return 1;
}

// This handles the effect of fear, and the other half of bleed and burn.
// This modifys the damage of the recipient
int modifyDamage(int baseDamage, StatusType status){
    switch (status)
    {
        case BURN:
            baseDamage = (int)(baseDamage * 0.75);
            break;

        case FEAR:
            baseDamage = (int)baseDamage / 2;
            break;

        case BLEED:
            baseDamage = (int)(baseDamage * 1.5);
            break;

        default:
            break;
    }

    return baseDamage;
}

char* statusText(StatusType status){
    switch(status){
        case POISON: 
            return "POIS";
        case BURN: 
            return "BURN";
        case FEAR: 
            return "FEAR";
        case FROZEN:
            return "FRZN";
        case BLEED: 
            return "BLED";
        default:
            return "NONE";
    }
}
