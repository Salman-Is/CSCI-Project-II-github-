# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <time.h>
# include <ctype.h>

# include "battle.h"
# include "main.h"
# include "ui.h"

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
void searchArea();
int dialouge();
/* ================= TRACK PLAYER ================= */
// the storyProgress variable tracks where the player is in the story
// storyProgress = 0 means you are at the tutorial area, 1 means you are in area 1 etc.
int storyProgress = 1;
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
char* quest2RewardsGOOD[32] = {"Knight Armor ( 28 HP )", "Knight Bow ( 6 ATK )", "Health Potion", "Focus Charm", "Verdent Plains Key"};
char* quest2RewardsEVIL[32] = {"Knight Sword ( 9 ATK )", "Knight Armor ( 28 HP )", "Berzerker Potion", "Verdent Plains Key"};

// If you guys are feeling creative you can edit the names of the areas here and it wont break anything
// but adding areas WILL require you to change the number of locations in the array
char availableLocations[6][32] = {"The Forest of Echoes", "The Verdent Plains", "The Blue Lake", "The Crystal Caves", "The Celestial Mountains", "Acention"};
char progressKey[32][32] = {"Forest Village", "Outpost", "Plains"};

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
Monster quest1GOOD[] = { // help village
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth"},
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth"}, 
    {"Groblin Shaman", 3, 5, 40, 7, EVIL, "Groblin Staff"},   
    {"Groblin Chief", 3, 5, 35, 10, EVIL, "Groblin Tusk"}
};
Monster quest1EVIL[] = { // pillage village
    {"Adventurer 'Kalen'", 1, 5, 15, 5, GOOD, "Tunic"},
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
    {"Astra, Deity of JUSTICE", 6, 10, 500, 50, GOOD, "A World of Evil"},
    {"Krya, Deity of MALICE", 6, 10, 500, 50, EVIL, "A World of Good"},
    {"???, Deity of CHAOS", 6, 11, 750, 50, EVIL, "???"}
};

/* ================= PLAYER OPTIONS ================= */
/*
    - User is prompted with what they want to do
    - Switch case returns the choice
*/
int options() {
    char* locColor = areaColor();
    printf("                   What would you like to do?\n\n");
    printf("\n[ Location: %s%s%s ] [ Progress: %d / %d ] [ Karma: %d / 100 ]\n", locColor, currentLoc, NORMAL, storyProgress, maxStoryProgress, karma);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                            EXPLORE                           ║\n");
    printf("║                                                              ║\n");
    printf("║     [1]    [2]        [3]         [4]       [5]      [6]     ║\n");
    printf("║   [WALK] [SEARCH] [ENCOUNTER] [INVENTORY] [TRAVEL] [STATS]   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    // printf("╔══════════════════════════════╗╔══════════════════════════════╗\n");
    // printf("║            INSPECT           ║║             BUILD            ║\n");
    // printf("║                              ║║                              ║\n");
    // printf("║     [A]      [B]    [C]      ║║       [D]     [E]   [F]      ║\n");
    // printf("║   [STATS] [QUESTS] [LOG]     ║║     [CRAFT] [BREW] [RUNE]    ║\n");
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
            case 1: encounter(forest, FOREST_COUNT); break;
            case 2: encounter(plains, PLAINS_COUNT); break;
            case 3: encounter(lake, LAKE_COUNT); break;
            case 4: encounter(caves, CAVES_COUNT); break;
            case 5: encounter(mountains, MOUNTAINS_COUNT); break;
            case 6: encounter(final, FINAL_COUNT); break;
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
            location = newLocation;
            isTravelling = 1;
            printf("You travel to the %s.\n", availableLocations[newLocation-1]);
        }
        else if (newLocation == 0){
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
    else if (choice == 'w') { 
        maxTurnDamage = 10000;
        maxPlayerTurnDamage = 10000;
        system("cls");
        printf("ONE SHOT mode\n");
    }
    else if (choice == 'x'){
        system("cls");
        codeLookup();
    }
    else if (choice == 'y'){
        system("cls");
        int newProgress = 0;
        printf("PROGRESS:\n\n");
        for(int i = 0; i < (sizeof(progressKey)/sizeof(progressKey[0])); i++)
        {
            printf("[%d] %s\n",(i+1), progressKey[i]);
        }
        printf("> ");
        scanf(" %d", &newProgress);
        storyProgress = newProgress;
        printf("Your story progress is [ %s ].\n", progressKey[newProgress-1]);
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
            printf("You see a resident nearby, would you like to speak to them?\n\n");
            int speak = dialouge();
            if (speak == 1){
                system("cls"); // this character comes back stronger later if you choose evil
                printf("[ %sAdventurer Kalen%s ]\n", GREEN, NORMAL);
                printf("╔═══════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
                printf("║ \"Ah, I can see you noticed that our village isn't in the best shape\"                                  ║\n");
                printf("║ \"We've had a bit of a Groblin problem, they've been attacking us relentlessly.\"                       ║\n");
                printf("║ \"Our resident Knight, Halor, sent for backup from the nearby Outpost, but they have yet to respond.\"  ║\n");
                printf("║ \"It really would be nice to have some backup, our resources are dwindling.\"                           ║\n");
                printf("║ \"We kept them at bay until now, but I'm not sure if we can hold out for much longer...\"               ║\n");
                printf("╚═══════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
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
                printf("╔════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
                printf("║ \"Passage into the Plains has been closed by the Kingdom.\"                                              ║\n");
                printf("║ \"A number of monster attacks have been reported in the reigon, and we must ensure the Forest is safe\"  ║\n");
                printf("║ \"Before escorting civilians. We havent even been able to mobilize to many of the nearby vill-\"         ║\n");
                printf("║                                                                                                        ║\n");
                printf("║ \"Ah, I see you have a sword. If you don't mind, we would appreciate it if you helped us clear out      ║\n");
                printf("║ \"Some of these monsters. We'll pay you handsomely for you service...\"                                  ║\n");
                printf("╚════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
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

void searchArea() {
    if (location == 1) // Area One
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
            case 6:
                addItem("Steel Bow ( 5 ATK )", 0);
                maxPlayerTurnDamage = 5; // <- Need a better system than this (cuz then if you come back here and grab this your damage might get lower)
                searchPoints1++;
                break;
            default:
                loreTablet("Spare no thought for those above.\n Through their grace we are evolved.\n Through their plights we are destroyed.");
                genericLoreResponse();
                break;
            }
        }
        else if (location == 2) // Area Two
        {
            static int searchPoints2 = 1; // wont get reset every time the function is called
            switch (searchPoints2){
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4: 
                break;
            case 5:
                break;
            case 6:
                break;
            default:
                break;
            }
        }
}

int dialouge() {
    // return 1 if yes, return 2 if no
    char speakChoice[32];
    while(1) {
        printf("[ 1 | Yes ]     [ 2 | No ]\n");
        printf("> ");
        scanf(" %s", speakChoice);
        if (strcmp(speakChoice, "1") == 0 || strcmp(speakChoice, "yes") == 0){
            return 1;
        }
        else if (strcmp(speakChoice, "2") == 0 || strcmp(speakChoice, "no") == 0) {
            return 2;
        }
        printf("Invalid Choice. Please type 1/yes, or 2/no\n\n");
    }
    
}
/* ================= DEBUGGING ================= */
void codeLookup() {
    //find ANSI codes (so i dont have to look it up every time i want a new color)

    for (int i = 0; i < 250; i++)
    {
        
        printf("\x1b[38;5;%dm", i);
        printf("[ %d ] Hello World\n", i);
        printf(NORMAL);
    }
}
