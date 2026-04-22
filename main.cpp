# include <cstdio>
# include <cstring>
# include <stdlib.h>
# include <time.h>
# include <ctype.h>
# include <unistd.h>
# include <windows.h>
#include <string>

using namespace std;

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
void Travel();

/* ================= TRACK PLAYER ================= */ // STORY PROGRESS -1
// the storyProgress variable tracks where the player is in the story
// storyProgress = 0 means you are at the tutorial area, 1 means you are in area 1 etc.
int storyProgress = -1;
int maxStoryProgress = 25;
int location = 1; // 1=forest, 2=plains, 3=lake 
int saveLocation = 1;
int isTravelling = 0; // 1 when travelling, 0 when not

int unlockedAreas = 0; // tracks how many places the player unlocked

int battleStart = 0;

// ik it's lazy to do this but ill do it anyway lol
int currentEnemyHP = 25;
int currentEnemyATK = 5;
int currentEnemyALIGNMENT = 0;
char currentEnemyDrop[32] = "";

/* ================= PLAYER STATS ================= */
int karma = 50;
int coins = 0;
int karmaAtkBoost = 1;
int karmaHpBoost = 1;

int dialougeSpeed = 0;
int trueSight = 0;

string playerAlignment = "NEUTRAL";

Item currentSword;
Item currentBow;
Item currentArmor;

Inventory inventory[100];
int inventoryCount = 0;

int worldState = 1;
#define COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

/* ================= QUEST REWARDS ================= */

// Forest Village quest
Item* quest1RewardsGOOD[] = { &steelArmor, &healthElixer, &healthPotion, &healthPotion, &elderiteGemstone };
Item* quest1RewardsEVIL[]  = { &goldSword, &healthElixer, &healthPotion, &berzerkerPotion };

// Knight Outpost quest
Item* quest2RewardsGOOD[] = { &knightArmor, &knightBow, &healthPotion, &focusCharm, &kingdomCrest };
Item* quest2RewardsEVIL[]  = { &knightSword, &knightArmor, &berzerkerPotion, &berzerkerPotion };

// Plains Map quest
Item* quest3RewardsGOOD[] = { &ancientMap };
Item* quest3RewardsEVIL[]  = { &ancientMap, &swordmasterArmor, &flameBlade, &frostBow };

// Plains Temple quest
Item* quest4Rewards[] = { &lichPhial, &staffOfRemnant };

/* ================= AREA/LOCATION NAMES ================= */
string availableLocations[6] = {"The Forest of Echoes", "The Verdent Plains", "The Blue Lake", 
"The Crystal Caves", "The Celestial Mountains", "Acention"};
string progressKey[32] = {"Forest Village", "Knight's Outpost", "Plains Map", "Skysealed Temple (Group)", "Skysealed Temple (Alone)"};

/* ================= MONSTERS/ENEMIES ================= */

// Name, difficulty, pattern size, HP, ATK, alignment, drop
Monster tutorial[] = {{"Ooz", 1, 5, 20, 2, EVIL, "Gel", NONE}};
// Forest enemy groups
Monster forest[] = {
    {"Lumora", 1, 3, 10, 3, GOOD, "Lumora Wing", NONE},       // Easy, 3-letter pattern 
    {"Deer", 1, 5, 20, 4, GOOD, "Leather", NONE},       // Easy, 5-letter pattern
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth", NONE},       // Easy, 5-letter pattern
    {"Flagon", 2, 6, 25, 7, EVIL, "Ember Scale", BURN}};
Monster helpVillage[] = {
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth", NONE},
    {"Groblin", 1, 5, 25, 5, EVIL, "Groblin Tooth", NONE}, 
    {"Groblin Shaman", 3, 5, 40, 7, EVIL, "Groblin Staff", POISON},   
    {"Groblin Chief", 3, 5, 35, 10, EVIL, "Groblin Tusk", NONE}};
Monster pillageVillage[] = {
    {"Adventurer 'Kalen'", 1, 5, 15, 5, GOOD, "Leather", NONE},
    {"Guard 'Eldric'", 3, 5, 20, 4, GOOD, "Scrap Metal", NONE}, 
    {"Guard 'Kaelor'", 3, 5, 20, 4, GOOD, "Scrap Metal", NONE},   
    {"Knight 'Halor'", 4, 6, 35, 10, GOOD, "Scrap Metal", NONE}};
Monster assistKnights[] = { 
    {"Juvinile Flagon", 2, 5, 20, 6, EVIL, "Ember Scale", NONE},
    {"Flagon", 2, 6, 25, 7, EVIL, "Ember Scale", BURN}, 
    {"Flagon", 2, 6, 25, 7, EVIL, "Ember Scale", BURN},   
    {"Elder Flagon", 4, 6, 40, 10, EVIL, "Inferno Scale", BURN}};
Monster attackKnights[] = {
    {"Knight 'Marlo'", 4, 6, 35, 10, GOOD, "Scrap Metal", NONE},
    {"Knight 'Lysa'", 4, 6, 35, 10, GOOD, "Scrap Metal", NONE}, 
    {"Knight Captain 'Therin'", 4, 7, 35, 10, GOOD, "Refined Metal", NONE},   
    {"Royal Knight 'Fenric'", 4, 8, 35, 10, GOOD, "Scrap Metal", BLEED}};

// Plains enemy groups
Monster plains[] = {
    {"Snarlbeast", 2, 6, 30, 7, EVIL, "Beastly Tooth", POISON},
    {"Nimora", 1, 5, 10, 3, EVIL, "Nimora Wing", NONE},
    {"Grass Troll", 2, 5, 30, 5, EVIL, "Troll Leather", NONE},
    {"Mossback", 1, 8, 50, 3, GOOD, "Fossilized Moss", NONE},
    {"Great Stag", 2, 8, 35, 6, GOOD, "Antlers", FEAR}};
Monster attackAdventurers[] = { // attack adventurers
    {"Swordmaster 'Lorel'", 4, 6, 35, 10, GOOD, "Berzerker Potion", BLEED},
    {"Rouge 'Reric'", 4, 5, 20, 35, GOOD, "Broken Dagger", BURN}, 
    {"Mage 'Sypha'", 5, 5, 25, 20, GOOD, "Fairy Dust", FROZEN}};
Monster lichBoss = {"Racher the Lich", 3, 9, 65, 50, GOOD, "True Sight Sigil", FEAR};
Monster templeHoard[] = {
    {"Nightmare Wraith", 3, 5, 25, 35, EVIL, "Vile Remnant", FEAR},
    {"Hollowed Witch", 3, 5, 25, 35, EVIL, "Vile Remnant", FEAR},
    {"Dark Revenant", 3, 5, 25, 35, EVIL, "Vile Remnant", FEAR},
    {"Blighted Stalker", 3, 5, 25, 35, EVIL, "Vile Remnant", FEAR},
    {"Cursed Sentinel", 3, 7, 25, 35, EVIL, "Vile Remnant", FEAR}};

// Lake enemy groups
Monster lake[] = {
    {"Mega Turtle", 1, 5, 40, 5, GOOD, "Shell Shard", NONE},
    {"Lake Serpent", 2, 5, 25, 10, EVIL, "Venom Vial", POISON},
    {"Kraken", 3, 5, 35, 10, EVIL, "Kraken Tentacle", FEAR},
    {"Ripplet", 2, 5, 15, 5, GOOD, "Shiny Scale", FROZEN},
    {"Glowfin", 3, 5, 15, 7, EVIL, "Luminous Scale", NONE},
    {"Oozard", 4, 5, 15, 8, EVIL, "Gelatinous Mass", FROZEN}};

// Cave enemy groups
Monster caves[] = {
    {"Cursed Bat", 2, 5, 15, 5, EVIL, "Echo Fang", BLEED},
    {"Crystal Snake", 2, 4, 25, 6, EVIL, "Crystal Venom", POISON},
    {"Shardling", 2, 4, 25, 6, GOOD, "Quartz Shard", FROZEN},
    {"Shifter Fox", 3, 5, 15, 5, EVIL, "Mirror Cloak", NONE},
    {"Stone Spider", 3, 5, 35, 6, EVIL, "Mineral Silk", NONE},
    {"Ancient Automaton", 4, 4, 45, 9, GOOD, "Gear Charge", FEAR}};

// Mountain enemy groups
Monster mountains[] = {
    {"Peak Eagle", 2, 5, 25, 7, GOOD, "Soaring Feather", NONE},
    {"Ice Giant", 3, 4, 55, 10, EVIL, "Ionic Ice", FROZEN},
    {"Mountain Goat", 2, 6, 30, 16, GOOD, "Durable Horn", NONE},
    {"Snow Leopard", 3, 4, 30, 20, GOOD, "Gorgeous Leather", BLEED},
    {"Dark Dragon", 4, 6, 65, 25, EVIL, "Demonic Scale", FEAR},
    {"High Dragon", 4, 6, 65, 25, GOOD, "Golden Scale", FEAR}};

// Final area bosses
Monster final[] = { // You will recive a really strong weapon before this guys dw
    {"Astra, Deity of JUSTICE", 5, 8, 500, 30, GOOD, "A World of Evil", NONE},
    {"Krya, Deity of MALICE", 5, 8, 500, 30, EVIL, "A World of Good", NONE},
    {"???, Deity of CHAOS", 7, 9, 750, 40, EVIL, "???", NONE}};

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
    string locColor = areaColor();
    StatusType playerStatus = NONE;
    StatusType enemyStatus = NONE;
    playerAl();
    printf("\n%sWhat would you like to do?%s\n", BOLD, UNBOLD);
    printf("\n[ Area: %s%s%s ] [ Progress: %d / %d ] [ %s ]\n", locColor, currentLoc, NORMAL, storyProgress, maxStoryProgress, playerAlignment);
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                            EXPLORE                           ║\n");
    printf("║                                                              ║\n");
    printf("║     [1]       [2]       [3]         [4]       [5]     [6]    ║\n"); 
    printf("║  [ADVANCE] [SEARCH] [ENCOUNTER] [INVENTORY]  [MAP]  [GEAR]   ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    printf("╔══════════════════════════════╗╔══════════════════════════════╗\n");
    printf("║            INSPECT           ║║             BUILD            ║\n");
    printf("║                              ║║                              ║\n");
    printf("║     [A]      [B]     [C]     ║║       [D]     [E]   [F]      ║\n");
    printf("║   [STATS]  [SHOP]  [CODEX]   ║║     [CRAFT] [BREW] [RUNE]    ║\n");
    printf("╚══════════════════════════════╝╚══════════════════════════════╝\n");

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
        randomEncounter();
        system("cls");
        return 3;
    }
    else if (choice == '4') { // INVENTORY
        system("cls");
        printf("You open your inventory.\n");
        openInventory(0, &currentArmor.value);
        return 4;
    }
    else if (choice == '5') { // MAP (TRAVEL)
        system("cls");
        if (isTravelling == 0){
            saveLocation = location;
        }
        int newLocation = 0;
        Travel();
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
    else if (choice == '6') { // GEAR
        system("cls");
        
        return 6;
    }
    else if (choice == 'a' || choice == 'A') { // STATS
        system("cls");
        statsPage();
        return 7;
    }
    else if (choice == 'b' || choice == 'B') { // SHOP
        system("cls");
        return 8;
    }
    else if (choice == 'c' || choice == 'C') { // LOG (Information Page like a PokeDex)
        system("cls");
        printf("This feature has not been added yet...\n");
        return 9;
    }
    else if (choice == 'd' || choice == 'D') { // CRAFT
        system("cls");
        printf("This feature has not been added yet...\n");
        return 10;
    }
    else if (choice == 'e' || choice == 'E') { // BREW (Potions)
        system("cls");
        printf("This feature has not been added yet...\n");
        return 11;
    }
    else if (choice == 'f' || choice == 'F') { // RUNE (Passive Ability)
        system("cls");
        printf("This feature has not been added yet...\n");
        return 12;
    }
    // === SUPER SECRET DEBUGGING OPTIONS (shhhh)===
    else if (choice == 's') { // speed up dialouge
        system("cls");
        dialougeSpeed = 1;
        printf("Dialouge is instant\n");
    }
    else if (choice == 't') { // SEE ENEMY HEALTH
        system("cls");
        trueSight = 1;
        printf("Enabled True Sight.\n");
    }
    else if (choice == 'u') { // HEALTH
        upgradeArmor(ultraArmor);
        system("cls");
        printf("Infinite health\n");
    }
    else if (choice == 'v') { // DAMAGE
        upgradeBow(ultraBow);
        upgradeSword(ultraSword);
        system("cls");
        printf("Infinte damage\n");
    }
    else if (choice == 'w'){ // FIND COLORS
        system("cls");
        codeLookup();
    }
    else if (choice == 'x') { // ADD ITEM TO INVENTORY
        system("cls");
        giveItem();
    }
    else if (choice == 'y'){ // SKIP TO LATER STORY
        fastForward();
    }
    else if (choice == 'z') { // Debug Help Menu
        system("cls");
        printf("> DEBUG HELP MENU <\n");
        printf("╔══════════════════════════════════╗\n");
        printf("║                                  ║\n");
        printf("║  [ s ] > Speed up all dialouge   ║\n");
        printf("║  [ t ] > Enable seeing enemy HP  ║\n");
        printf("║  [ u ] > Increase health to 999  ║\n");
        printf("║  [ v ] > Increase damage to 999  ║\n");
        printf("║  [ w ] > Find ANSI code colors   ║\n");
        printf("║  [ x ] > Add item to inventory   ║\n");
        printf("║  [ y ] > Skip to later quest     ║\n");
        printf("║  [ z ] > Help Menu               ║\n");
        printf("║                                  ║\n");
        printf("╚══════════════════════════════════╝\n");
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
    // while getting ASCI art from chatgpt it told me to do this or it wouldn't work, so this SHOULD NOT count towards grade
    system("chcp 65001 > nul");
    addItem("Health Potion", 1);
    // --- Quest Variables ---
    int startQuest1 = 0;
    int startQuest2 = 0;
    int startQuest3 = 0;
    int startQuest4 = 0;
    int startQuest5 = 0;
    // --- Quest Flags ---
    int quest1Action = 0;

    currentBow = woodenBow;
    currentSword = ironSword;
    currentArmor = chainArmor;

    srand(time(NULL));
    while (storyProgress == -1){ // Start Menu
        int start = 0;
        printf("%s%s", BOLD, PALEYELL);
        printf("               _____ _            ____       _           _ _       \n");
        printf("            / |_   _| |__   ___  |  _ \\ __ _| | __ _  __| (_)_ __  \n");
        printf("  _________/>___| |_| '_ \\ / _ \\_| |_) / _  | |/ _  |/ _  | |  _ \\ ____________\n");
        printf(" |__________|___| |_| | | |  __/_|  __/ (_| | | (_| | (_| | | | | |____________/\n");
        printf("           \\>   |_| |_| |_|\\___| |_|   \\____|_|\\____|\\____|_|_| |_|                           \n");
        printf("            \\ %s%s\n\n", UNBOLD, NORMAL);

        printf("        [ 1 ] START GAME            [ 2 ] EXIT           [ 3 ] SKIP TUTORIAL     \n");
        printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║                                                                                ║\n");
        printf("║                           Enter a number to begin...                           ║\n");
        printf("║                                                                                ║\n");
        printf("║                              Version 1.0  |  2026                              ║\n");
        printf("║                                                                                ║\n");
        printf("╚════════════════════════════════════════════════════════════════════════════════╝\n\n");
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
            system("cls");
            storyProgress = 1;
        }
        else {
            system("cls");
            start = 0;
        }
    
    }
    while (storyProgress == 0) { // Tutorial
        system("cls");
        printf("══════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
        specialPrintf("                     You wake up in the middle of a cave. It's dark, and you're alone.                        \n");
        specialPrintf("          You didn't exist until now, and yet you are filled with purpose. You know what you must do.         \n");
        specialPrintf("     You were created for a single purpose. You must make it to the Celestial Mountains to reach Acention...\n\n");
        specialPrintf("                           There, YOU, will decide the fate of this world.                                    \n");
        printf("══════════════════════════════════════════════════════════════════════════════════════════════════════════════\n");
        pressEnter();
        system("cls");
        printf("Your solitude is interupted by a passing monster. Prepare youself...\n");
        Sleep(3000);
        encounter(tutorial, 1, 1, -1);
        system("cls");
        printf("You look to your right and find a record of some kind...\n");
        loreTablet("Your creators welcome you to the Kingdom of Aureveil, Paladin.\n\n- Astra.                                                 - Kyra.");
        specialPrintf("\nThose names resonate with something within you...\n");
        specialPrintf("\nYou leave the cave you woke up in and start heading towards the smoke in the distance...\n");
        pressEnter();
        system("cls");
        storyProgress = 1;
    }
    while (storyProgress == 1) { // Forest Village
        int navigataionChoice = options();
        if (navigataionChoice == 1){
            specialPrintf("Deep within the forest, you find a small village.\n");
            specialPrintf("Though it was quite humble, it looks as if its been damaged.\n");
            specialPrintf("You see a resident nearby, would you like to speak to them?\n\n");
            int speak = dialouge(); // < events.c
            if (speak == 1){
                system("cls"); // this character comes back stronger later if you choose evil
                dialougeBox("Adventurer Kalen", GREEN, "KALEN_1");
                startQuest1++;
            }
            else if (speak == 2){
                printf("The nearby resident see's you staring. She looks away uncomfortably...\n");
            }
            if (startQuest1 != 0){
                string questhoice = questAlignment("Help the village", "Pillage them while they're weak");
                if (questhoice == "GOOD"){
                    if (questGauntlet(helpVillage, 4, "Groblin", "the Forest Village") == 1) {
                        printf("You defeated every remaining Groblin in the village...\n");
                        printf("The villagers erupt in cheers for your victory!\n");
                        printf("'May the light of Astra guide your travels!'\n");
                        printf("They shower you with their most valuable treasures...\n\n");
                        questRewards(quest1RewardsGOOD, 5, 30);
                        quest1Action++;
                        storyProgress++;
                    }
                }
                else if (questhoice == "EVIL"){
                    if (questGauntlet(pillageVillage, 4, "Warrior", "the Forest Village") == 1) {
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
                pressEnter();
            }
        }
    }
    while (storyProgress == 2) { // Knight Outpost
        int navigataionChoice = options();
        if (navigataionChoice == 1){
            specialPrintf("After walking through the forest, you stumble upon a Knight's Outpost...\n");
            specialPrintf("Behind them is the gate to the Verdent Plains, a vibrant pasture filled with rare monsters...\n");
            specialPrintf("The Knights look at you as you walk up to one of them.\n");
            specialPrintf("Only a Knight can give someone access to the Verdent Plains. Would you like to speak to them?\n\n");
            int speak = dialouge();
            if (speak == 1){
                system("cls");
                dialougeBox("Knight Marlo", DARKBLUE, "MARLO_1");
                startQuest2++;
            }
            else if (speak == 2){
                printf("The Knight's stare at you with suspicion...\n");
            }
            if (startQuest2 != 0){
                string questhoice = questAlignment("Assist the Knights", "Turn your sword on them for the Key");
                if (questhoice == "GOOD"){
                    if (questGauntlet(assistKnights, 4, "Flagon", "the Outpost") == 1) {
                        printf("You defeated every Flagon invading the Outpost...\n");
                        printf("They were no match for you...\n");
                        questRewards(quest2RewardsGOOD, 5, 40);
                        storyProgress++;
                    }
                }
                else if (questhoice == "EVIL"){
                    if (questGauntlet(attackKnights, 4, "Knight", "the Outpost") == 1) {
                        printf("You defeated every Knight in the Outpost...\n");
                        printf("They were no match for you...\n");
                        questRewards(quest2RewardsEVIL, 4, 50);
                        storyProgress++;
                    }
                }
                else {
                    continue;
                }
                location = 2;
                unlockedAreas++;
                pressEnter();
            }
             
        }
    }
    while (storyProgress == 3) { // Plains Map
        int navigataionChoice = options();
        if (navigataionChoice == 1){
            specialPrintf("Walking through the gates to the Verdent Plains presents you with a glorious sight.\n");
            specialPrintf("Before you is a beautiful landscape, with beasts lumbering around wildy.\n");
            specialPrintf("You begin to walk around, enjoying the sights, when you stumble across a group of adventurers.\n");
            specialPrintf("They look quite powerful, and they are holding an interesting map... Would you like to speak to them?\n\n");
            int speak = dialouge();
            if (speak == 1){
                system("cls");
                dialougeBox("Swordmaster Lorel", GOLD, "LOREL_1");
                dialougeBox("Mage Sypha", LILAC, "SYPHA_1");
                dialougeBox("Swordmaster Lorel", GOLD, "LOREL_2");
                startQuest3++;
            }
            else if (speak == 2){
                printf("The group continues walking...\n");
            }
            if (startQuest3 != 0){
                specialPrintf("You look at the map, and you recognize the language.\n");
                specialPrintf("It's the same script as the record tablet you found in that cave...\n");
                string questhoice = questAlignment("Assist the adventurers", "Strike them down for their loot");
                if (questhoice == "GOOD"){
                    printf("You silently nod.\n");
                    pressEnter();
                    dialougeBox("Swordmaster Lorel", GOLD, "LOREL_3");
                    dialougeBox("Mage Sypha", LILAC, "SYPHA_2");
                    dialougeBox("Rouge Reric", ORANGE, "RERIC_1");
                    dialougeBox("Swordmaster Lorel", GOLD, "LOREL_4");
                    dialougeBox("Mage Sypha", LILAC, "SYPHA_3");
                    dialougeBox("Rouge Reric", ORANGE, "RERIC_2");
                    questRewards(quest3RewardsGOOD, 1, 0);
                    storyProgress++;
                }
                else if (questhoice == "EVIL"){
                    if (questGauntlet(attackAdventurers, 4, "Adventurer", "the group") == 1) {
                        printf("You defeated every adventurer...\n");
                        printf("They were no match for you.\n");
                        printf("You take their valuables and their map, and head off to find it's treasure.\n");
                        questRewards(quest3RewardsEVIL, 4, 50);
                        storyProgress++;
                    }
                }
                else {
                    continue;
                }
                pressEnter();
            }
             
        }
    }
    while (storyProgress == 4) { // Plains Dungeon [good route]
        int navigataionChoice = options();
        static int openedDoor = 0;
        static int defeatedLich = 0;
        if (navigataionChoice == 1){
            if (startQuest4 == 0) {
                specialPrintf("You walk with the group, guiding them silently using the map's guidelines.\n");
                specialPrintf("The adventurers help you fend off monsters along the way.\n");
                specialPrintf("At the end of the journey, you find yourselves outside of an ancient temple...\n");
                specialPrintf("You hear the shrieks of monsters inside...\n");
                specialPrintf("Are you ready to venture inside?\n\n");
                int speak = dialouge();
                if (speak == 1){
                    dialougeBox("Swordmaster Lorel", GOLD, "LOREL_5");
                    dialougeBox("Rouge Reric", ORANGE, "RERIC_3");
                    dialougeBox("Mage Sypha", LILAC, "SYPHA_4");
                    system("cls");
                    startQuest4++;
                }
                else if (speak == 2){
                    printf("The group looks at you expectantly...\n");
                    pressEnter();
                }
            }
            if (startQuest4 != 0){
                if (openedDoor == 0) {
                    specialPrintf("The four of you walk into the temple, carefully watching each other's backs.\n");
                    specialPrintf("You come across a strange door, with 3 seals.\n");
                    specialPrintf("You find 3 cooresponding murals on the wall...\n");
                    pressEnter();
                    dialougeBox("Swordmaster Lorel", GOLD, "LOREL_6");
                    dialougeBox("Rouge Reric", ORANGE, "RERIC_4");
                    dialougeBox("Mage Sypha", LILAC, "SYPHA_5");
                }
                char* correct[] = {"moon", "sun", "star"};
                if (puzzleDoor(correct) == 1) {
                    openedDoor = 1;
                    dialougeBox("Mage Sypha", LILAC, "SYPHA_6");
                    dialougeBox("Swordmaster Lorel", GOLD, "LOREL_7");
                    dialougeBox("Rouge Reric", ORANGE, "RERIC_5");
                }
                else {
                    specialPrintf("The door did not accept your answers.\n");
                    pressEnter();
                }
                if(openedDoor != 0) {
                    if (defeatedLich == 0) {
                        specialPrintf("The four of you continue to walk towards where the screaming came from...\n");
                        specialPrintf("You find a seemingly empty room, and you all walk inside.\n");
                        specialPrintf("Suddenly, the door behind you slams shut, and hordes of dark monsters flood the room.\n");
                        specialPrintf("The four of you prepare for battle, yet the mosnters stay still.\n");
                        specialPrintf("As if they were waiting for a command...\n");
                        specialPrintf("Everything is made clear when a dark pool of liquid solidifies into the shape.\n");
                        specialPrintf("Of a grey skinned demonic mage.\n");
                        pressEnter();
                        dialougeBox("Mage Sypha", LILAC, "SYPHA_7");
                        dialougeBox("The Lich", GREEN, "LICH_1");
                        dialougeBox("Swordmaster Lorel", GOLD, "LOREL_8");
                        dialougeBox("Racher the Lich", GREEN, "LICH_2");
                        dialougeBox("Racher the Lich", GREEN, "LICH_3");
                        dialougeBox("Racher the Lich", GREEN, "LICH_4");
                        dialougeBox("Swordmaster Lorel", GOLD, "LOREL_9");
                        dialougeBox("Rouge Reric", ORANGE, "RERIC_6");
                        dialougeBox("Mage Sypha", LILAC, "SYPHA_8");
                        dialougeBox("Racher the Lich", GREEN, "LICH_5");
                        specialPrintf("The group stares at you with astonishment, but their shock is interrupted.\n");
                        specialPrintf("The hordes of monsters that filled the room suddenly snap awake.\n");
                        specialPrintf("The group starts attacking them, but they can't let up to attack Racher.\n");
                        specialPrintf("It is up to you to kill the Lich before he escapes. Prepare yourself.\n");
                        pressEnter();
                        if (bossFight(lichBoss) == 1) {
                            trueSight = 1;
                            dialougeBox("Racher the Lich", GREEN, "LICH_6");
                            specialPrintf("You defeated The Lich!\n\n");
                            specialPrintf("You absorb the power of True Sight...\n");
                            specialPrintf("You shall know the true health of all monsters you encounter...\n");
                            questRewards(quest4Rewards, 2, 50);
                            defeatedLich++;
                            pressEnter();
                        }
                        else {
                            printf("You were killed by The Lich!\n");
                            pressEnter();
                        }
                    }
                    if (defeatedLich == 1) {
                        specialPrintf("With the death of the Lich, the temple began to crumble.\n");
                        specialPrintf("You the group run to the exit, making it just in time to watch the monsters.\n");
                        specialPrintf("from the temple burn in the sunlight.\n");
                        specialPrintf("The adventurers turn to look at you, stunned.\n");
                        pressEnter();
                        dialougeBox("Swordmaster Lorel", GOLD, "LOREL_10");
                        dialougeBox("Mage Sypha", LILAC, "SYPHA_9");
                        dialougeBox("Rouge Reric", ORANGE, "RERIC_7");
                        dialougeBox("Swordmaster Lorel", GOLD, "LOREL_11");
                        specialPrintf("As you watch your friends walk away from the temple, you look to the sky.\n");
                        specialPrintf("It's the next morning. You've been fighting all night. You need rest.\n");
                        specialPrintf("You return to the main road, and head towards Emerald City...\n");
                        pressEnter();
                        storyProgress = 6;
                    }
                }
                pressEnter();
            }
             
        }
    }
    while (storyProgress == 5) { // Plains Dungeon Alone [evil route]
        int navigataionChoice = options();
        static int openedDoor = 0;
        static int defeatedLich = 0;
        if (navigataionChoice == 1){
            if (startQuest4 == 0) {
                specialPrintf("You follow the map's directions across the Verdent Plains.\n");
                specialPrintf("You fend off monsters along the way.\n");
                specialPrintf("At the end of the journey, you find yourself outside of an ancient temple...\n");
                specialPrintf("You hear the shrieks of monsters inside...\n");
                specialPrintf("Are you ready to venture inside?\n\n");
                int speak = dialouge();
                if (speak == 1){
                    system("cls");
                    startQuest4++;
                }
                else if (speak == 2){
                    printf("You hesitate to enter...\n");
                }
            }
            if (startQuest4 != 0){
                if (openedDoor == 0) {
                    specialPrintf("You walk into the dark, grimy temple, carefully watching each step.\n");
                    specialPrintf("It's an ornate building, decorated with strange runes.\n");
                    specialPrintf("You come across a strange door, with 3 seals.\n");
                    specialPrintf("You find 3 cooresponding murals on the wall...\n");
                    pressEnter();
                    specialPrintf("The first mural depicts a dark tower before the moon.\n");
                    specialPrintf("The second mural depicts monsters burning in the sun.\n");
                    specialPrintf("The third mural depicts a lone traveller staring at the stars\n");
                    specialPrintf("\nYou must use these clues to open the door.\n");
                    pressEnter();
                }
                char* correct[] = {"moon", "sun", "star"};
                if (puzzleDoor(correct) == 1) {
                    openedDoor = 1;
                    specialPrintf("With the opening of the door, the screams cease.\n");
                    specialPrintf("Whatever lied within has been alerted to your presence.\n");
                    specialPrintf("\nYou must venture deeper into the temple to discover it's secrets...\n");
                    pressEnter();
                }
                else {
                    specialPrintf("The door did not accept your answers.\n");
                    pressEnter();
                }
                if(openedDoor != 0) {
                    if (defeatedLich == 0) {
                        specialPrintf("You continue to walk towards where the screaming came from...\n");
                        specialPrintf("You find a seemingly empty room, and you walk inside.\n");
                        specialPrintf("Suddenly, the door behind you slams shut, and hordes of dark monsters flood the room.\n");
                        specialPrintf("You assume the position for battle, yet the mosnters stay still.\n");
                        specialPrintf("As if they were waiting for a command...\n");
                        specialPrintf("Everything is made clear when a dark pool of liquid solidifies into the shape.\n");
                        specialPrintf("Of a grey skinned demonic mage.\n");
                        pressEnter();
                        dialougeBox("The Lich", GREEN, "LICH_1");
                        dialougeBox("Racher the Lich", GREEN, "LICH_2");
                        dialougeBox("Racher the Lich", GREEN, "LICH_3");
                        dialougeBox("Racher the Lich", GREEN, "LICH_7");
                        specialPrintf("The hordes of monsters that filled the room suddenly snap awake.\n");
                        specialPrintf("Defeat the hordes of monsters and kill the Lich.\n");
                        pressEnter();
                        if (questGauntlet(templeHoard, 5, "Vile Creature", "the temple") == 1) {
                            system("cls");
                            specialPrintf("You cleared the room of the Vile Creatures under the Lich's control.\n");
                            specialPrintf("You turn to face Racher. A twinge of fear spreads across his face.\n");
                            pressEnter();
                            dialougeBox("Racher the Lich", GREEN, "LICH_8");
                            if (bossFight(lichBoss) == 1) {
                                trueSight = 1;
                                dialougeBox("Racher the Lich", GREEN, "LICH_6");
                                specialPrintf("You defeated The Lich!\n\n");
                                specialPrintf("You absorb the power of the True Sight...\n\n");
                                specialPrintf("You shall know the true health of all monsters you encounter...\n");
                                questRewards(quest4Rewards, 2, 50);
                                defeatedLich++;
                                pressEnter();
                            }
                            else {
                                printf("You were killed by The Lich!\n");
                                pressEnter();
                            }
                        }
                        else {
                            printf("You were killed by the hoard!\n");
                            pressEnter();
                        }
                    }
                    if (defeatedLich == 1) {
                        specialPrintf("With the death of the Lich, the temple began to crumble.\n");
                        specialPrintf("You run to the exit, making it just in time to watch the monsters\n");
                        specialPrintf("from the temple burn in the sunlight.\n");
                        specialPrintf("It's the next morning. You've been fighting all night. You need rest.\n");
                        specialPrintf("You return to the main road, and head towards Emerald City...\n");
                        pressEnter();
                        storyProgress = 6;
                    }
                }
                pressEnter();
            }
             
        }
    }
    while (storyProgress == 6) { // End for now
        printf("               ╔══════════════════════════════════════════════╗\n");
        printf("               ║                                              ║\n");
        printf("               ║              END OF CHAPTER ONE              ║\n");
        printf("               ║                                              ║\n");
        printf("               ║             ||||||||||||||||||||             ║\n");
        printf("               ║                                              ║\n");
        printf("               ╚══════════════════════════════════════════════╝\n\n");
        Sleep(2000);
        specialPrintf("                         to be continued...\n");
        Sleep(5000);
    }
    return 0;
}

// Source - https://stackoverflow.com/a/1406437
// Posted by Eric Petroelje, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-22, License - CC BY-SA 2.5

// Technically I did change it up a bit but it's basically the same
void pressEnter() {
    int enter;
    fflush(stdout);
    printf("\n▷ Press Enter to Continue");
    while ((enter = getchar()) != '\n' && enter != EOF);
    getchar();
    system("cls");
}

void playerAl() {
    // Karma between 0-33 -> EVIL, 34-66 -> NEUTRAL, 67(lol)-100 -> GOOD;
    if (karma < 33 && playerAlignment == "EVIL"){
        playerAlignment == "EVIL";
        printf("%sYou stray further from the natural order...\n%s", RED, NORMAL);
        printf("Even the shadows whisper your name in fear.\n\n");
        karmaAtkBoost = 1.5;
        karmaHpBoost = 0.75;
    }
    else if (karma <= 66 && karma > 32 && playerAlignment == "NEUTRAL"){
        playerAlignment == "NEUTRAL";
        printf("%sYou walk the line of chaos and order.%s\n", WHITE, NORMAL);
        printf("The world treats you with cautious respect.\n\n");
        karmaAtkBoost = 1;
        karmaHpBoost = 1;
    }
    else if (karma > 66 && playerAlignment == "GOOD"){
        playerAlignment == "GOOD";
        printf("%sYou feel a surge of righteousness fill your heart.%s\n", CYAN, NORMAL);
        printf("The spirits sing your name with reversence.\n\n");
        karmaAtkBoost = 0.75;
        karmaHpBoost = 1.5;
    }
}

void randomEncounter() {
    switch(location){
        case 1: encounter(forest, FOREST_COUNT, 0, -1); break;
        case 2: encounter(plains, PLAINS_COUNT, 0, -1); break;
        case 3: encounter(lake, LAKE_COUNT, 0, -1); break;
        case 4: encounter(caves, CAVES_COUNT, 0, -1); break;
        case 5: encounter(mountains, MOUNTAINS_COUNT, 0, -1); break;
        case 6: encounter(final, FINAL_COUNT, 0, -1); break;
        default: printf("No monsters here.\n");
    }
}

/* ================= DEBUGGING ================= */
/**
 * function to find ANSI codes 
 * (so i dont have to look it up every time i want a new color)
 */
void codeLookup() {
    for (int i = 0; i < 250; i++)
    {
        
        printf("\x1b[38;5;%dm", i);
        printf("[ %d ] Hello World [■■■■■]\n", i);
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
        printf("[%02d] %s\n",(i+1), progressKey[i]);
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
    int amount;
    printf("How many?\n");
    printf("> ");
    scanf(" %d", &amount);
    for (int i = 0; i < amount; i++)
    {
        addItem(item, 1);
    }
    printf("You have added %d [ %s%s%s ] to your inventory.\n", amount, CYAN, item, NORMAL);
}

void Travel() {
    printf("              [ MAP OF Aureveil ]\n");
    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║                    __.--._        .---,          ║\n");
    printf("║                 _.'       \\:.,   ;     ;         ║\n");
    printf("║               ,-'             \\_/       \\        ║\n");
    printf("║      _.''' `--'        %s▲▲%s     %s▲%s       %s▲▲%s  ;.     ║\n", GRAY, NORMAL, GRAY, NORMAL, GRAY, NORMAL);
    printf("║     :                     %s▲▲%s    %s5%s           ;    ║\n", GRAY, NORMAL, BOLD, UNBOLD);
    printf("║     ,'    %s4%s            %s▲%s   %sMountains%s %s▲▲▲%s    ;    ║\n", BOLD, UNBOLD, GRAY, NORMAL, GRAY, NORMAL, GRAY, NORMAL);
    printf("║    :    %sCaves%s         %s▲▲%s      %s▲▲%s           ,     ║\n", DARKBLUE, NORMAL, GRAY, NORMAL, GRAY, NORMAL);
    printf("║    :                                      ;      ║\n");
    printf("║    :       ....                           :      ║\n");
    printf("║    ;      /  %s3%s  \\                          :     ║\n", BOLD, UNBOLD);
    printf("║    (     : %sLake%s :        %s2%s                  ;    ║\n", BLUE, NORMAL, BOLD, UNBOLD);
    printf("║    `-.    \\..../      %sPlains%s      %s1%s       ,'     ║\n", LIME, NORMAL, BOLD, UNBOLD);
    printf("║      ;                         %sForest%s   :        ║\n", GREEN, NORMAL);
    printf("║    .'                             .-._,'         ║\n");
    printf("║    `.                       ---._;               ║\n");
    printf("║      `-.__          __.--._)                     ║\n");
    printf("║           `.--.____;                             ║\n");
    printf("║                                                  ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    printf("╔═════════════════════════════════════╗\n");
    printf("[#] | [ MAP KEY ]                     ║\n");             	  		 
    printf("╚═════════════════════════════════════╝\n");
    
    printf("═══════════════════════════════════════\n");
    for(int i = 0; i < sizeof(availableLocations)/sizeof(availableLocations[0]); i++)
        {
            if (i <= unlockedAreas) {
                printf("[%d] %s\n",(i+1), availableLocations[i]);
            }     
        }
    printf("═══════════════════════════════════════\n");
    printf("╔═════════════════════════════════════╗\n");
    printf("[0] | < Return to story               ║\n");           				 
    printf("╚═════════════════════════════════════╝\n\n");
}
