# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <time.h>
# include <ctype.h>
# include <unistd.h>
# include <windows.h>

#include "battle.h"
#include "main.h"
#include "ui.h"
#include "events.h"

//MAIN script from which the entire game will be run

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
    - Story
    - More regular monsters
    - Boss monsters
    - Tutorial (and a way for the player to choose a name)
    - Better travelling system (only to area you alread went to)
    - (You guys can create new features if you're up for it)
*/
void codeLookup();
void fastForward();
void giveItem();

/* ================= TRACK PLAYER ================= */
// the storyProgress variable tracks where the player is in the story
// storyProgress = 0 means you are at the tutorial area, 1 means you are in area 1 etc.
int storyProgress = -1;
int maxStoryProgress = 32;
int location = 1; // 1=forest, 2=plains, 3=lake 
int saveLocation = 1;
int isTravelling = 0; // 1 when travelling, 0 when not

int battleStart = 0;

// ik it's lazy to do this but ill do it anyway lol
int currentEnemyHP = 25;
int currentEnemyATK = 5;
int currentEnemyALIGNMENT = 0;
char currentEnemyDrop[32] = "";

/* ================= PLAYER STATS ================= */
int karma = 50;
int maxTurnDamage = 5;  // Sword Damage (counterattacks) <-- from correct pattern
int maxPlayerTurnDamage = 4; // Arrow Damage (direct attacks) <-- from choosing to fire arrow
int playerMaxHP = 25;
int coins = 0;

char currentSword[] = "Iron Sword";
char currentBow[] = "Wooden Bow";
char currentArmor[] = "Chain Armor";

Inventory inventory[100];
int inventoryCount = 0;

int worldState = 1;

/* ================= QUEST REWARDS ================= */

// Forest Village quest
char* quest1RewardsGOOD[32] = {"Steel Armor ( 27 HP )", "Health Elixer", "Health Potion", "Health Potion", "Elderite Gemstone"};
char* quest1RewardsEVIL[32] = {"Golden Sword ( 8 ATK )", "Health Elixer", "Health Potion", "Berzerker Potion"};
char* quest2RewardsGOOD[32] = {"Knight Armor ( 28 HP )", "Knight Bow ( 6 ATK )", "Health Potion", "Focus Charm", "Verdent Plains Key"};
char* quest2RewardsEVIL[32] = {"Knight Sword ( 9 ATK )", "Knight Armor ( 28 HP )", "Berzerker Potion", "Verdent Plains Key"};

/* ================= AREA/LOCATION NAMES ================= */
char availableLocations[6][32] = {"The Forest of Echoes", "The Verdent Plains", "The Blue Lake", "The Crystal Caves", "The Celestial Mountains", "Acention"};
char progressKey[32][32] = {"Forest Village", "Outpost", "Plains"};

/* ================= MONSTERS/ENEMIES ================= */

// Name, difficulty, pattern size, HP, ATK, alignment, drop
Monster tutorial[] = {{"Ooz", 1, 5, 20, 2, EVIL, "Gel"}};
// Forest enemy groups
Monster forest[] = {
    {"Lumora", 1, 3, 10, 3, GOOD, "Lumora Wing"},       // Easy, 3-letter pattern 
    {"Ooz", 1, 5, 20, 2, EVIL, "Gel"},
    {"Deer", 1, 5, 20, 4, GOOD, "Leather"},       // Easy, 5-letter pattern
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth"},       // Easy, 5-letter pattern
    {"Flagon", 2, 6, 25, 7, EVIL, "Ember Scale"},       // Medium, 5-letter pattern
};
Monster quest1GOOD[] = { // help village
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth"},
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth"}, 
    {"Groblin Shaman", 3, 5, 40, 7, EVIL, "Groblin Staff"},   
    {"Groblin Chief", 3, 5, 35, 10, EVIL, "Groblin Tusk"}
};
Monster quest1EVIL[] = { // pillage village
    {"Adventurer 'Kalen'", 1, 5, 15, 5, GOOD, "Leather"},
    {"Guard 'Eldric'", 3, 5, 20, 4, GOOD, "Scrap Metal"}, 
    {"Guard 'Kaelor'", 3, 5, 20, 4, GOOD, "Scrap Metal"},   
    {"Knight 'Halor'", 4, 6, 35, 10, GOOD, "Scrap Metal"}
};
Monster quest2GOOD[] = { // assist knights
    {"Juvinile Flagon", 2, 5, 20, 6, EVIL, "Ember Scale"},
    {"Flagon", 2, 6, 25, 7, EVIL, "Ember Scale"}, 
    {"Flagon", 2, 6, 25, 7, EVIL, "Ember Scale"},   
    {"Elder Flagon", 4, 6, 40, 10, EVIL, "Inferno Scale"}
};
Monster quest2EVIL[] = { // attak knights
    {"Knight 'Marlo'", 4, 6, 35, 10, GOOD, "Scrap Metal"},
    {"Knight 'Lysa'", 4, 6, 35, 10, GOOD, "Scrap Metal"}, 
    {"Knight Captain 'Therin'", 4, 7, 35, 10, GOOD, "Refined Metal"},   
    {"Royal Knight 'Fenric'", 4, 8, 35, 10, GOOD, "Scrap Metal"}
};

// Plains enemy groups
Monster plains[] = {
    {"Snarlbeast", 2, 6, 30, 7, EVIL, "Beastly Tooth"},
    {"Nimora", 1, 5, 10, 3, EVIL, "Nimora Wing"},
    {"Grass Troll", 2, 5, 30, 5, EVIL, "Scrap Metal"},
    {"Mossback", 1, 8, 50, 3, GOOD, "Fossilized Moss"},
    {"Great Stag", 3, 8, 35, 6, GOOD, "Antlers"}
};

// Lake enemy groups
Monster lake[] = {
    {"Turtle", 1, 5, 40, 5, GOOD, "Shell Shard"},
    {"Lake Serpent", 2, 5, 25, 10, EVIL, "Venom Vial"},
    {"Kraken", 3, 5, 35, 10, EVIL, "Calamari"},
    {"Ripplet", 2, 5, 15, 5, GOOD, "Shiny Scale"},
    {"Glowfin", 3, 5, 15, 7, EVIL, "Luminous Scale"},
    {"Oozard", 4, 5, 15, 8, EVIL, "Gelatinous Mass"}
};

// Cave enemy groups
Monster caves[] = {
    {"Monster1", 1, 1, 5, 5, GOOD, "Something"},
};

// Mountain enemy groups
Monster mountains[] = {
    {"Monster1", 1, 1, 5, 5, EVIL, "Something"},
};

// Final area bosses
Monster final[] = { // You will recive a really strong weapon before this guys dw
    {"Astra, Deity of JUSTICE", 5, 8, 500, 30, GOOD, "A World of Evil"},
    {"Krya, Deity of MALICE", 5, 8, 500, 30, EVIL, "A World of Good"},
    {"???, Deity of CHAOS", 7, 9, 750, 40, EVIL, "???"}
};

/* ================= PLAYER OPTIONS ================= */
/**
 * User is prompted with what they want to do 
 * There are 6 options right now, and the player can chose from those 6
 * The function checks for your input and returns a value based on that
 * 
 * I sort of changed it around a bit, so now the onyl return statement that matters
 * is return 1, but I left the others there just in case we need them again
 */
int options() {
    char* locColor = areaColor();
    printf("\n%sWhat would you like to do?%s\n", BOLD, UNBOLD);
    printf("\n[ Location: %s%s%s ] [ Progress: %d / %d ] [ Karma: %d / 100 ]\n", locColor, currentLoc, NORMAL, storyProgress, maxStoryProgress, karma);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                            EXPLORE                           ║\n");
    printf("║                                                              ║\n");
    printf("║     [1]    [2]        [3]         [4]       [5]      [6]     ║\n"); // STATS will be changed to "shop" in project 3
    printf("║   [WALK] [SEARCH] [ENCOUNTER] [INVENTORY] [TRAVEL] [STATS]   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    // printf("╔══════════════════════════════╗╔══════════════════════════════╗\n");
    // printf("║            INSPECT           ║║             BUILD            ║\n");
    // printf("║                              ║║                              ║\n");
    // printf("║     [A]      [B]    [C]      ║║       [D]     [E]   [F]      ║\n");
    // printf("║   [STATS]  [CHAT]  [LOG]     ║║     [CRAFT] [BREW] [RUNE]    ║\n");
    // printf("╚══════════════════════════════╝╚══════════════════════════════╝\n");

    char choice;
    printf("> ");
    scanf(" %c", &choice);
    
    if (choice == '1') { // WALK
        if (isTravelling != 0) {
            system("cls");
            printf("You walk around, but dont find much...\n");
            printf("( Return to the story to progress )\n");
        }
        else {
            system("cls");
            return 1;
        }
        
    }
    else if (choice == '2') { // SEARCH
        system("cls");
        printf("You search the area...\n");
        searchArea();
        return 2;
    }
    else if (choice == '3') { // ENCOUNTER
        switch(location){
            case 1: encounter(forest, FOREST_COUNT, 0); break;
            case 2: encounter(plains, PLAINS_COUNT, 0); break;
            case 3: encounter(lake, LAKE_COUNT, 0); break;
            case 4: encounter(caves, CAVES_COUNT, 0); break;
            case 5: encounter(mountains, MOUNTAINS_COUNT, 0); break;
            case 6: encounter(final, FINAL_COUNT, 0); break;
            default: printf("No monsters here.\n");
        }
        system("cls");
        return 3;
    }
    else if (choice == '4') { // INVENTORY
        system("cls");
        printf("You open your inventory.\n");
        openInventory(0, &playerMaxHP, playerMaxHP);
        return 4;
    }
    else if (choice == '5') { // TRAVEL
        system("cls");
        if (isTravelling == 0){
            saveLocation = location;
        }
        int newLocation = 0;
        printf("LOCATIONS:\n\n");
        for(int i = 0; i < sizeof(availableLocations)/sizeof(availableLocations[0]); i++)
        {
            printf("[%d] %s\n",(i+1), availableLocations[i]);
        }
        printf("\n[0] < Return to story\n\n");
        printf("> ");
        scanf(" %d", &newLocation);
        if (newLocation != 0 && newLocation <= sizeof(availableLocations)/sizeof(availableLocations[0])){
            system("cls");
            location = newLocation;
            isTravelling = 1;
            printf("You travel to the %s.\n", availableLocations[newLocation-1]);
        }
        else if (newLocation == 0){
            system("cls");
            location = saveLocation;
            isTravelling = 0;
            printf("You return to the %s.\n", progressKey[storyProgress-1]);
        }
        
        return 5;
    }
    else if (choice == '6') { // STATS
        system("cls");
        statsPage();
        return 6;
    }
    else if (choice == 'a' || choice == 'A') {
        system("cls");
        printf("This feature has not been added yet...\n");
        return 7;
    }
    else if (choice == 'b' || choice == 'B') {
        system("cls");
        printf("This feature has not been added yet...\n");
        return 8;
    }
    else if (choice == 'c' || choice == 'C') {
        system("cls");
        printf("This feature has not been added yet...\n");
        return 9;
    }
    else if (choice == 'd' || choice == 'D') {
        system("cls");
        printf("This feature has not been added yet...\n");
        return 10;
    }
    else if (choice == 'e' || choice == 'E') {
        system("cls");
        printf("This feature has not been added yet...\n");
        return 11;
    }
    else if (choice == 'f' || choice == 'F') {
        system("cls");
        printf("This feature has not been added yet...\n");
        return 12;
    }
    // === SUPER SECRET DEBUGGING OPTIONS (shhhh)===
    else if (choice == 'v') { // HEALTH
        playerMaxHP = 999;
        system("cls");
        printf("Infinite health\n");
    }
    else if (choice == 'w') { // DAMAGE
        maxTurnDamage = 999;
        maxPlayerTurnDamage = 999;
        system("cls");
        printf("Infinte damage\n");
    }
    else if (choice == 'x'){ // FIND COLORS
        system("cls");
        codeLookup();
    }
    else if (choice == 'y'){ // SKIP TO LATER STORY
        fastForward();
    }
    else if (choice == 'z') { // ADD ITEM TO INVENTORY
        system("cls");
        giveItem();
    }
    else {
        system("cls");
        printf("Invalid choice, choose a number.\n");
        return options();
    }
    
}
/* ================= MAIN STORY ================= */
/**
 * This is where the story takes place
 * The game asks the player what they want to do every chance it gets
 * if options() returns 1, it progresses the story 
 * The players current place in the story is tracked by storyProgress
 * storyProgress is incremented whenever the player finishes a quest
 * 
 */
int main(void) {
    system("cls");
    system("chcp 65001 > nul"); // < while getting ASCI art from chatgpt it told me to do this or it wouldn't work, so this doesnt count as A level work
    addItem("Health Potion", 1);
    // --- Quest Variables ---
    int startQuest1 = 0;
    int startQuest2 = 0;
    int startQuest3 = 0;
    int startQuest4 = 0;
    int startQuest5 = 0;
    // --- Quest Flags ---
    int quest1Action = 0;
    int quest2Action = 0;
    int quest3Action = 0;
    int quest4Action = 0;
    int quest5Action = 0;

    srand(time(NULL));
    while (storyProgress == -1) {
        int start = 0;
        printf("      _____ _            ____       _           _ _       \n");
        printf("     |_   _| |__   ___  |  _ \\ __ _| | __ _  __| (_)_ __  \n");
        printf("       | | | '_ \\ / _ \\ | |_) / _` | |/ _` |/ _` | | '_ \\ \n");
        printf("       | | | | | |  __/ |  __/ (_| | | (_| | (_| | | | | |\n");
        printf("       |_| |_| |_|\\___| |_|   \\__,_|_|\\__,_|\\__,_|_|_| |_|\n\n");

        printf("        [ 1 ] START GAME                      [ 2 ] EXIT        \n");
        printf("╔══════════════════════════════════════════════════════════════╗\n");
        printf("║                                                              ║\n");
        printf("║                  Press a number to begin...                  ║\n");
        printf("║                                                              ║\n");
        printf("║                     Version 1.0  |  2026                     ║\n");
        printf("║                                                              ║\n");
        printf("╚══════════════════════════════════════════════════════════════╝\n\n");
        printf("> ");
        scanf(" %d", &start);
        if (start == 1){
            printf("Your journey begins...\n\n");
            storyProgress = 0; 
        }
        else if (start == 2) {
            printf("Abandoning your journey before it begins... interesting...");
            exit(0);
        }
        else if (start == 3){
            fastForward();
        }
    
    }
    while (storyProgress == 0) {
        system("cls");
        printf("══════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
        printf("                     You wake up in the middle of a cave. It's dark, and you're alone.                        \n");
        printf("          You didn't exist until now, and yet you are filled with purpose. You know what you must do.         \n");
        printf("     You were created for a single purpose. You must make it to the Celestial Mountains to reach Acention...\n\n");
        printf("                           There, YOU, will decide the fate of this world.                                    \n");
        printf("══════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
        printf("\nPress ENTER to continue...");
        getchar();
        getchar();
        system("cls");
        printf("Your solitude is interupted by a passing monster. Prepare youself...\n");
        time_t start_time, current_time;
        time(&start_time);
        do {
            time(&current_time);
        } while (difftime(current_time, start_time) < 3);
        encounter(tutorial, 1, 1);
        system("cls");
        printf("You look to your right and find a record of some kind...\n");
        loreTablet("Your creators welcome you to the Kingdom of Celestia, Paladin.\n\n - Astra.            - Kyra.");
        printf("Those names resonate with something within you...\n");
        printf("\nYou leave the cave you woke up in and start heading towards the smoke in the distance...\n");
        printf("\nPress ENTER to continue...");
        getchar();
        getchar();
        system("cls");
        storyProgress = 1;
    }
    while (storyProgress == 1) {
        int navigataionChoice = options();
        if (navigataionChoice == 1) // WALK
        {
            printf("Deep within the forest, you find a small village.\n");
            printf("Though it was quite humble, it looks as if its been damaged.\n");
            printf("You see a resident nearby, would you like to speak to them?\n\n");
            int speak = dialouge();
            if (speak == 1){
                system("cls"); // this character comes back stronger later if you choose evil
                printf("[ %sAdventurer Kalen%s ]\n", GREEN, NORMAL);
                printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
                printf("║                                                                                                        ║\n");
                printf("║  \"Ah, I can see you noticed that our village isn't in the best shape\"                                  ║\n");
                printf("║  \"We've had a bit of a Groblin problem, they've been attacking us relentlessly.\"                       ║\n");
                printf("║  \"Our resident Knight, Halor, sent for backup from the nearby Outpost, but they have yet to respond.\"  ║\n");
                printf("║  \"It really would be nice to have some backup, our resources are dwindling.\"                           ║\n");
                printf("║  \"We kept them at bay until now, but I'm not sure if we can hold out for much longer...\"               ║\n");
                printf("║                                                                                                        ║\n");
                printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
                startQuest1++;
            }
            else if (speak == 2){
                printf("The nearby resident see's you staring. She looks away uncomfortably...\n");
            }
            if (startQuest1 != 0)
            {
                char* quest1choice = questAlignment("Help the village", "Pillage them while they're weak");
                if (strcmp(quest1choice, "GOOD") == 0){
                    if (questGauntlet(quest1GOOD, 4, "Groblin", "the Forest Village") == 1) {
                        printf("You defeated every remaining Groblin in the village...\n");
                        printf("The villagers erupt in cheers for your victory!\n");
                        printf("'May the light of Astra guide your travels!'\n");
                        printf("They shower you with their most valuable treasures...\n\n");
                        questRewards(quest1RewardsGOOD, 5, 30);
                        quest1Action++;
                        storyProgress++;
                    }
                }
                else if (strcmp(quest1choice, "EVIL") == 0){
                    if (questGauntlet(quest1EVIL, 4, "Warrior", "the Forest Village") == 1) {
                        printf("You defeated every remaining Warrior in the village...\n");
                        printf("They curse you before they lose consiousness.\n");
                        printf("You raid their treasure room and take their loot...\n\n");
                        questRewards(quest1RewardsEVIL, 4, 20);
                        quest1Action--;
                        storyProgress++;
                    }
                }
                else {
                    continue;
                }
                printf("\nPress ENTER to exit the village...");
                getchar();
                getchar();
            }
        }
    }
    while (storyProgress == 2) {
        int navigataionChoice = options();
        if (navigataionChoice == 1) // WALK
        {
            printf("After walking through the forest, you stumble upon a Knight's Outpost...\n");
            printf("Behind them is the gate to the %sVerdent Plains%s, a vibrant pasture filled with rare monsters...\n", LIME, NORMAL);
            printf("The Knights look at you as you walk up to one of them.\n");
            printf("Only a Knight can give someone access to the Verdent Plains. Would you like to speak to them?\n\n");
            int speak = dialouge();
            if (speak == 1){
                system("cls");
                printf("[ %sKnight Marlo%s ]\n\n", GREEN, NORMAL);
                printf("╔═════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
                printf("║                                                                                                        ║\n");
                printf("║  \"Passage into the Plains has been closed by the Kingdom.\"                                              ║\n");
                printf("║  \"A number of monster attacks have been reported in the reigon, and we must ensure the Forest is safe\"  ║\n");
                printf("║  \"Before escorting civilians. We havent even been able to mobilize to many of the nearby vill-\"         ║\n");
                printf("║                                                                                                         ║\n");
                printf("║  \"Ah, I see you have a sword. If you don't mind, we would appreciate it if you helped us clear out      ║\n");
                printf("║  \"Some of these monsters. We'll pay you handsomely for you service...\"                                  ║\n");
                printf("║                                                                                                         ║\n");
                printf("╚═════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
                startQuest2++;
            }
            else if (speak == 2){
                printf("The Knight's stare at you with suspicion...\n");
            }
            if (startQuest2 != 0)
            {
                char* quest2choice = questAlignment("Assist the Knights", "Turn your sword on them for the Key");
                if (strcmp(quest2choice, "GOOD") == 0){
                    if (questGauntlet(quest2GOOD, 4, "Flagon", "the Outpost") == 1) {
                        printf("You defeated every Flagon invading the Outpost...\n");
                        printf("They were no match for you...\n");
                        questRewards(quest2RewardsGOOD, 5, 40);
                        quest2Action++;
                        storyProgress++;
                    }
                }
                else if (strcmp(quest2choice, "EVIL") == 0){
                    if (questGauntlet(quest2EVIL, 4, "Knight", "the Outpost") == 1) {
                        printf("You defeated every Knight in the Outpost...\n");
                        printf("They were no match for you...\n");
                        questRewards(quest2RewardsEVIL, 4, 50);
                        quest2Action--;
                        storyProgress++;
                    }
                }
                else {
                    continue;
                }
                printf("\nPress ENTER to exit the village...");
                getchar();
                getchar();
            }
             
        }
    }
    return 0;
}
/**
 * This function holds all the search results for every area
 * It's not pretty, and we will think of a better solution for project 3
 * Basically, an if statement checks what area you are in
 * Then, a switch case determines which item you are "on" using the static int searchPoints
 * Its a static int so it won't get reassigned to 1 when the function is called again
 * When you find/complete the search event, it increments searchPoints
 * This way, you can only find something once after you complete whatever it is
 * Except for lore tablets, which are the default in the switch case
 * 
 * Later on, we should make a system where theres a "pool" of possible search events
 * and theres a random chance to get each one, so we don't need this huge switch case function
 * Probably using structs
 */

/* ================= DEBUGGING ================= */
/**
 * function to find ANSI codes 
 * (so i dont have to look it up every time i want a new color)
 */
void codeLookup() {
    for (int i = 0; i < 250; i++)
    {
        
        printf("\x1b[38;5;%dm", i);
        printf("[ %d ] Hello World\n", i);
        printf(NORMAL);
    }
}
/**
 * Functio for skipping ahead in the story so we can test it
 * Saves time debugging if theres a new feature later in the story
 */
void fastForward() {
    int newProgress = 0;
    system("cls");
    printf("PROGRESS:\n\n");
    for(int i = 0; i < (sizeof(progressKey)/sizeof(progressKey[0])); i++)
    {
        printf("[%d] %s\n",(i+1), progressKey[i]);
    }
    printf("> ");
    scanf(" %d", &newProgress);
    storyProgress = newProgress;
    system("cls");
    printf("Your story progress is [ %s ].\n", progressKey[newProgress-1]);
}
/**
 * Used to hack in an item into your inventory wherever you are
 * Speeds up development
 */
void giveItem() {
    printf("What item do you want to add to your inventory?\n");
    printf("> ");
    char item[64];
    fgets(item, 64, stdin);
    fgets(item, 64, stdin);
    item[strcspn(item, "\n")] = '\0';
    addItem(item, 1);
    printf("You have added a [ %s%s%s ] to your inventory\n", CYAN, item, NORMAL);
}