# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <time.h>
# include <ctype.h>

#include "battle.h"
#include "main.h"
#include "ui.h"


/*
    MAIN script from which the entire game will be run
*/

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
void searchArea();

// the storyProgress variable tracks where the player is in the story
// storyProgress = 0 means you are at the tutorial area, 1 means you are in area 1 etc.
int storyProgress = 1;
int maxStoryProgress = 50;
int location = 1; // 1=forest, 2=plains, 3=lake 

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
char playerName[] = "The Paladin";
int playerMaxHP = 25;
int coins = 0;

Inventory inventory[100];
int inventoryCount = 0;

int worldState = 1;

/* ================= QUEST REWARDS ================= */

// Forest Village quest
char* quest1RewardsGOOD[32] = {"Steel Armor ( 27 HP )", "Health Elixer", "Health Potion", "Health Potion", "Elderite Gemstone"};
char* quest1RewardsEVIL[32] = {"Golden Sword ( 8 ATK )", "Health Elixer", "Health Potion", "Berzerker Potion"};
char* quest2RewardsEVIL[32] = {"Knight Sword ( 9 ATK )", "Knight Armor ( 28 HP )", "Health Potion", "Berzerker Potion"};

// If you guys are feeling creative you can edit the names of the areas here and it wont break anything
//                                 vvvv                       vvvvv                    vvvvv
char availableLocations[16][16] = {"Forest", "Plains", "Lake", "Caves", "Mountains", "Test"};

/* ================= MONSTERS/ENEMIES ================= */

// Name, difficulty, pattern size, HP, ATK, alignment, drop

// Forest enemy groups
Monster forest[] = {
    {"Lumora", 1, 3, 10, 3, GOOD, "Lumora Wing"},       // Easy, 3-letter pattern 
    {"Deer", 1, 5, 20, 4, GOOD, "Leather"},       // Easy, 5-letter pattern
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth"},       // Easy, 5-letter pattern
    {"Flagon", 2, 6, 25, 7, EVIL, "Ember Scale"},       // Medium, 5-letter pattern
    {"Key Golem", 1, 3, 30, 1, GOOD, "Verdent Key"}
};

Monster quest1GOOD[] = {
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth"},
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth"}, 
    {"Groblin Shaman", 3, 5, 40, 7, EVIL, "Groblin Staff"},   
    {"Groblin Chief", 3, 5, 35, 10, EVIL, "Groblin Tusk"}
};

Monster quest1EVIL[] = {
    {"Adventurer 'Kalen'", 1, 5, 15, 5, GOOD, "Tunic"},
    {"Guard 'Eldric'", 3, 5, 20, 4, GOOD, "Scrap Metal"}, 
    {"Guard 'Kaelor'", 3, 5, 20, 4, GOOD, "Scrap Metal"},   
    {"Knight 'Halor'", 4, 6, 35, 10, GOOD, "Scrap Metal"}
};

Monster quest2EVIL[] = {
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

Monster test[] = {
    {"DRAGON", 5, 7, 50, 50, EVIL, "Dragon Scale"},
    {"DRAGON", 5, 7, 50, 50, EVIL, "Dragon Scale"}
};



/* ================= PLAYER OPTIONS ================= */
/*
    - User is prompted with what they want to do
    - Switch case returns the choice
*/
int options() {
    char* locColor = areaColor();
    printf("\n[ Location: %s%s%s ] [ Progress: %d / %d ] [ Karma: %d / 100 ]\n", locColor, currentLoc, NORMAL, storyProgress, maxStoryProgress, karma);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                  What would you like to do?                  ║\n");
    printf("║                                                              ║\n");
    printf("║     [1]    [2]        [3]         [4]       [5]      [6]     ║\n");
    printf("║   [WALK] [SEARCH] [ENCOUNTER] [INVENTORY] [TRAVEL] [STATS]   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");

    char choice;
    printf("> ");
    scanf(" %c", &choice);
    
    if (choice == '1') { // WALK
        system("cls");
        return 1;
    }
    else if (choice == '2') { // SEARCH
        system("cls");
        searchArea();
        return 2;
    }
    else if (choice == '3') { // ENCOUNTER
        switch(location){
            case 1: encounter(forest, FOREST_COUNT); break;
            case 2: encounter(plains, PLAINS_COUNT); break;
            case 3: encounter(lake, LAKE_COUNT); break;
            case 4: encounter(caves, CAVES_COUNT); break;
            case 5: encounter(mountains, MOUNTAINS_COUNT); break;
            case 6: encounter(test, TEST); break;
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
        int newLocation = 0;
        printf("LOCATIONS:\n\n");
        for(int i = 0; i < 6; i++)
        {
            printf("[%d] %s\n",(i+1), availableLocations[i]);
        }
        printf("> ");
        scanf(" %d", &newLocation);
        location = newLocation;
        printf("You travel to the %s.\n", availableLocations[newLocation-1]);
        return 5;
    }
    else if (choice == '6') { // STATS
        system("cls");
        statsPage();
        return 6;
    }
    else if (choice == '7') { // Debugging
        maxTurnDamage = 10000;
        maxPlayerTurnDamage = 10000;
        system("cls");
        printf("ONE SHOT mode\n");
    }
    else if (choice == '8'){
        codeLookup();
    }
    else {
        system("cls");
        printf("Invalid choice, choose a number.\n");
        return options();
    }
}

/* ==================================== MAIN GAME LOOP ==================================== */
/**
 * This is where the story takes place
 * The players current place in the story is tracked by storyProgress
 * storyProgress is incremented whenever the player makes significant progress
 * This includes starting a quest, finishing a quest, or returning to a previous area.
 */
/*
BASIC TEMPLATE (can change depending on area):

while (storyProgress == X) {
    static int searchPoints = 1;                        <- this is how we track how many times the player found an item
    int navigataionChoice = options();                  <- this is how we track the players input
    if (navigataionChoice == 1) // WALK                 <- this is where the content of the story goes (dialouge, starting quests etc)
    {
        printf("story story story blah blah blah.\n");
        printf("wow look this is a cool area fr story story.\n");
        printf("wanna talk to that dude?");
    }
    else if (navigataionChoice == 2) // SEARCH          <- this is the where we handle trasure hunting and stuff
    {
        switch (searchPoints){                          <- this switch case tracks how many times the player searched (as I foreshadowed before with seachpoints)
        case 1: // First thing you can find             <- case 1 means you find an item, then you can NEVER search for that item here again (no infinite item farming)
            addItem("THING");                           <- addItem function (simplifies things a lot)
            searchPoints++;                             <- don't forget to increment!
        case 2: // second thing
            printf("You found %s10%s Coins!", YELLOW, NORMAL);
            coins+=10;
            searchPoints++;
            break;
        case 3: // third etc.
            addItem("THING 2"); 
            searchPoints++;
            break;
        default:
            loreTablet("CRAZY lore stuff here bro");
            genericLoreResponse();
            break;
        }
    }
}
*/
int main(void) {
    system("cls");
    system("chcp 65001 > nul"); // < while getting ASCI art from chatgpt it told me to do this or it wouldn't work, so this doesnt count as A level work
    addItem("Health Potion", 1);

    char quest1Flag[16] = "";
    char quest2Flag[16] = "";
    char quest3Flag[16] = "";
    char quest4Flag[16] = "";
    char quest5Flag[16] = "";

    srand(time(NULL));
    while (storyProgress == 0)
    {
        /* TUTORIAL */
    }
    while (storyProgress == 1) {
        int navigataionChoice = options();
        if (navigataionChoice == 1) // WALK
        {
            printf("Deep within the forest, you find a small village.\n");
            printf("Though it was quite humble, it looks as if its been damaged.\n");
            printf("You see a resident nearby, would you like to speak to them?\n");
            // choice to speak, dialouge/exposition
            char* quest1choice = questAlignment("Help the village", "Pillage them while they're weak");
            if (strcmp(quest1choice, "GOOD") == 0){
                if (questGauntlet(quest1GOOD, 4, "Groblin", "the Forest Village") == 1) {
                    printf("You defeated every remaining Groblin in the village...\n");
                    printf("The villagers erupt in cheers for your victory!\n");
                    printf("They shower you with their most valuable treasures...\n\n");
                    questRewards(quest1RewardsGOOD, sizeof(quest1RewardsGOOD)/sizeof(quest1RewardsGOOD[0]) ,30);
                    storyProgress += 1;
                    strcpy(quest1Flag, "GOOD");
                    printf("\nPress ENTER to exit the village...");
                    getchar();
                    getchar();
                }
            }
            else if (strcmp(quest1choice, "EVIL") == 0){
                if (questGauntlet(quest1EVIL, 4, "Warrior", "the Forest Village") == 1) {
                    printf("You defeated every remaining Warrior in the village...\n");
                    printf("They curse you before they lose consiousness.\n");
                    printf("You raid their treasure room and take their loot...\n\n");
                    questRewards(quest1RewardsEVIL, sizeof(quest1RewardsEVIL)/sizeof(quest1RewardsEVIL[0]) ,20);
                    storyProgress +=1;
                    strcpy(quest1Flag, "EVIL");
                    continue;
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
    while (storyProgress == 2) {
        int navigataionChoice = options();
        if (navigataionChoice == 1) // WALK
        {
            printf("After walking through the forest, you stumble upon a Knight's Outpost...\n");
            printf("Behind them is the gate to the Blue Lake, a stunning pool of water home to hundreds of unknown treasures...\n");
            printf("The Knights look at you as you walk up to one of them.\n");
            printf("Only a Knight can give someone access to the Blue Lake. Would you like to speak to them?\n\n");
            // choice to speak, dialouge/exposition
            char* quest2choice = questAlignment("Speak to the Knights", "Fight them for the Key");
            if (strcmp(quest2choice, "GOOD") == 0){
                
            }
            else if (strcmp(quest2choice, "EVIL") == 0){
                if (questGauntlet(quest2EVIL, 4, "Knight", "the Outpost") == 1) {
                    printf("You defeated every Knight in the Outpost...\n");
                    printf("They were no match for you...\n");
                    questRewards(quest2RewardsEVIL, sizeof(quest2RewardsEVIL)/sizeof(quest2RewardsEVIL[0]) , 30);
                }
            }
            else {
                continue;
            }
        }
    }
    return 0;
}

void searchArea() {
    if (location == 1) // SEARCH
        {
            static int searchPoints1 = 1; // wont get reset every time the function is called
            switch (searchPoints1){
            case 1: // First thing you can find
                addItem("Health Potion", 0);
                searchPoints1++;
                break;
            case 2: // second thing
                addCoins(10, "no");
                searchPoints1++;
                break;
            case 3: // third etc.
                addItem("Steel Sword ( 6 ATK )", 0);
                maxTurnDamage = 6; // <- Need a better system than this (cuz then if you come back here and grab this your damage might get lower)
                searchPoints1++;
                break;
            case 4: 
                riddle("What has a head, a tail, and no body?", "coin", &searchPoints1, "Health Elixer");
                break;
            case 5:
                chest("Verdent Key", "Forest Sword ( 9 ATK )", "Verdent", GREEN, &searchPoints1);
                maxTurnDamage = 9; // <- Need a better system than this (cuz then if you come back here and grab this your damage might get lower)
                break;
            default:
                loreTablet("Spare no thought for those above.\n Through their grace we are evolved.\n Through their plights we are destroyed.");
                genericLoreResponse();
                break;
            }
        }
}

void codeLookup() {
    //find ANSI codes (so i dont have to look it up every time i want a new color)

    for (int i = 0; i < 250; i++)
    {
        
        printf("\x1b[38;5;%dm", i);
        printf("[ %d ] Hello World\n", i);
        printf(NORMAL);
    }
}
