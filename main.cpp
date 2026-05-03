#include "battle.h"
#include "main.h"
#include "ui.h"
#include "events.h"
#include "defs.h"
#include "story.h"

//MAIN script from which the entire game will be run

void codeLookup();
void fastForward();
void giveItem();
void Travel();

/* ================= TRACK PLAYER ================= */ // STORY PROGRESS -1
// the storyProgress variable tracks where the player is in the story
// storyProgress = 0 means you are at the tutorial area, 1 means you are in area 1 etc.
int storyProgress = -1;
int maxStoryProgress = 16;
int location = 1; // 1=forest, 2=plains, 3=lake 
int saveLocation = 1;
int isTravelling = 0; // 1 when travelling, 0 when not

int unlockedAreas = 0; // tracks how many places the player unlocked

int battleStart = 0;

// ik it's lazy to do this but ill do it anyway lol
int currentEnemyHP = 25;
int currentEnemyATK = 5;
int currentEnemyALIGNMENT = 0;
Item currentEnemyDrop = Item("", "", "", "", WHITE, 0);

/* ================= PLAYER STATS ================= */
int karma = 50;
int coins = 0;
int karmaAtkBoost = 1;
int karmaHpBoost = 1;

int dialougeSpeed = 0;
int trueSight = 0;

string playerAlignment = "NEUTRAL";

Item currentSword = Item("Temp Sword", "Text needed for this to stop yelling at me", "Weapon", "Sword", WHITE, 5, NONE);;
Item currentBow = Item("Temp Bow", "Im just testing to see if this works", "Weapon", "Bow", WHITE, 5, NONE);;
Item currentArmor = Item("Temp Armor", "La la la ignore this la", "Armor", "Armor", WHITE, 25);

vector<Item> inventory;
vector<Rune> unlockedRunes;
Rune* activeRune = nullptr;

int worldState = 1;
#define COUNT(arr) (sizeof(arr) / sizeof(arr[0]))

/* ================= AREA/LOCATION NAMES ================= */
string availableLocations[6] = {"The Forest of Echoes", "The Verdent Plains", "The Blue Lake", 
"The Crystal Caves", "The Celestial Mountains", "Acention"};
string progressKey[15] = {"Forest Village", "Knight's Outpost", "Plains Map", "Skysealed Temple (Group)", "Skysealed Temple (Alone)", "Emerald City",
     "Infected Lake", "Lake Interlude", "Crystal Heart", "Crystal Dragon"};

/* ================= MONSTERS/ENEMIES ================= */

// Name, difficulty, pattern size, HP, ATK, alignment, drop
Monster tutorial[] = {{"Ooz", 1, 5, 20, 2, EVIL, gel, NONE}};
// Forest enemy groups
Monster forest[] = {
    {"Lumora", 1, 3, 10, 3, GOOD, lumoraWing, NONE},       // Easy, 3-letter pattern 
    {"Deer", 1, 5, 20, 4, GOOD, leather, NONE},       // Easy, 5-letter pattern
    {"Groblin", 1, 5, 25, 5, EVIL, groblinTooth, NONE},       // Easy, 5-letter pattern
    {"Flagon", 2, 6, 25, 7, EVIL, emberScale, BURN}};
Monster helpVillage[] = {
    {"Groblin", 1, 5, 25, 5, EVIL, groblinTooth, NONE},
    {"Groblin", 1, 5, 25, 5, EVIL, groblinTooth, NONE}, 
    {"Groblin Shaman", 3, 5, 40, 7, EVIL, groblinStaff, POISON},   
    {"Groblin Chief", 3, 5, 35, 10, EVIL, groblinTusk, NONE}};
Monster pillageVillage[] = {
    {"Adventurer 'Kalen'", 1, 5, 15, 5, GOOD, leather, NONE},
    {"Guard 'Eldric'", 3, 5, 20, 4, GOOD, scrapMetal, NONE}, 
    {"Guard 'Kaelor'", 3, 5, 20, 4, GOOD, scrapMetal, NONE},   
    {"Knight 'Halor'", 4, 6, 35, 10, GOOD, scrapMetal, NONE}};
Monster assistKnights[] = { 
    {"Juvinile Flagon", 2, 5, 20, 6, EVIL, emberScale, NONE},
    {"Flagon", 2, 6, 25, 7, EVIL, emberScale, BURN}, 
    {"Flagon", 2, 6, 25, 7, EVIL, emberScale, BURN},   
    {"Elder Flagon", 4, 6, 40, 10, EVIL, infernoScale, BURN}};
Monster attackKnights[] = {
    {"Knight 'Marlo'", 4, 6, 35, 10, GOOD, scrapMetal, NONE},
    {"Knight 'Lysa'", 4, 6, 35, 10, GOOD, scrapMetal, NONE}, 
    {"Knight Captain 'Therin'", 4, 7, 35, 10, GOOD, refinedMetal, NONE},   
    {"Royal Knight 'Fenric'", 4, 8, 35, 10, GOOD, scrapMetal, BLEED}};

// Plains enemy groups
Monster plains[] = {
    {"Snarlbeast", 2, 6, 30, 7, EVIL, beastlyTooth, POISON},
    {"Nimora", 1, 5, 10, 3, EVIL, nimoraWing, NONE},
    {"Grass Troll", 2, 5, 30, 5, EVIL, trollLeather, NONE},
    {"Mossback", 1, 8, 50, 3, GOOD, fossilizedMoss, NONE},
    {"Great Stag", 2, 8, 35, 6, GOOD, greatAntlers, FEAR}};
Monster attackAdventurers[] = { // attack adventurers
    {"Swordmaster 'Lorel'", 4, 6, 35, 10, GOOD, berzerkerPotion, BLEED},
    {"Rouge 'Reric'", 4, 5, 20, 35, GOOD, brokenDagger, BURN}, 
    {"Mage 'Sypha'", 5, 5, 25, 20, GOOD, fairyDust, FROZEN}};
Monster lichBoss = {"Racher the Lich", 3, 9, 65, 50, EVIL, trueSightSigil, FEAR};
Monster templeHoard[] = {
    {"Nightmare Wraith", 3, 5, 25, 35, EVIL, vileRemnant, FEAR},
    {"Hollowed Witch", 3, 5, 25, 35, EVIL, vileRemnant, FEAR},
    {"Dark Revenant", 3, 5, 25, 35, EVIL, vileRemnant, FEAR},
    {"Blighted Stalker", 3, 5, 25, 35, EVIL, vileRemnant, FEAR},
    {"Cursed Sentinel", 3, 7, 25, 35, EVIL, vileRemnant, FEAR}};
Monster corruptCaptain = {"Corrupt Captain", 3, 7, 45, 50, EVIL, refinedMetal, BLIND};

// Lake enemy groups
Monster lake[] = {
    {"Mega Turtle", 1, 5, 40, 5, GOOD, shellShard, NONE},
    {"Lake Serpent", 2, 5, 25, 10, EVIL, venomVial, POISON},
    {"Kraken", 3, 5, 35, 10, EVIL, krakenTentacle, FEAR},
    {"Ripplet", 2, 5, 15, 5, GOOD, shinyScale, FROZEN},
    {"Glowfin", 3, 5, 15, 7, EVIL, luminousScale, BLIND},
    {"Oozard", 4, 5, 15, 8, EVIL, gelatinousMass, FROZEN}};
Monster vael = {"Vael, the Drowned", 5, 7, 75, 50, EVIL, demonicScale, BLEED};
Monster lirien = {"Lirien, Spirit of the Lake", 5, 7, 75, 50, GOOD, regenerationCrystal, FROZEN};
Monster escapedMonsters[] = {
    {"Snarlbeast", 2, 6, 30, 7, EVIL, beastlyTooth, POISON},
    {"Snarlbeast", 2, 6, 30, 7, EVIL, beastlyTooth, POISON},
    {"Nimora", 1, 5, 10, 3, EVIL, nimoraWing, NONE},
    {"Grass Troll", 2, 5, 30, 5, EVIL, trollLeather, NONE},
    {"Grass Troll", 2, 5, 30, 5, EVIL, trollLeather, NONE}};
// Cave enemy groups
Monster caves[] = {
    {"Cursed Bat", 2, 5, 15, 5, EVIL, echoFang, BLIND},
    {"Crystal Snake", 2, 4, 25, 6, EVIL, crystalVenom, POISON},
    {"Shardling", 2, 4, 25, 6, GOOD, quartzShard, BLIND},
    {"Shifter Fox", 3, 5, 15, 5, EVIL, mirrorCloak, NONE},
    {"Stone Spider", 3, 5, 35, 6, EVIL, mineralSilk, BLEED},
    {"Ancient Automaton", 4, 4, 45, 9, GOOD, gearCharge, FEAR}};
Monster crystalGuardians[] = {
    {"Crystal Snake", 4, 6, 30, 10, GOOD, quartzShard, BLIND},
    {"Crystal Sentry", 4, 6, 30, 10, GOOD, quartzShard, BLIND},
    {"Crystal Golem", 5, 7, 35, 14, GOOD, quartzShard, BLIND},
    {"Crystal Sentry", 4, 6, 30, 10, GOOD, quartzShard, BLIND},
    {"Crystal Colossus", 5, 8, 50, 18, GOOD, quartzShard, BLIND},
};
Monster alexandrite = {"Alexandrite, the Crystal Dragon", 5, 7, 120, 60, GOOD, alexandriteScale, BLIND};

// Mountain enemy groups
Monster mountains[] = {
    {"Peak Eagle", 2, 5, 25, 7, GOOD, soaringFeather, NONE},
    {"Ice Giant", 3, 4, 55, 10, EVIL, ionicIce, FROZEN},
    {"Mountain Goat", 2, 6, 30, 16, GOOD, durableHorn, NONE},
    {"Snow Leopard", 3, 4, 30, 20, GOOD, gorgeousLeather, BLEED},
    {"Dark Dragon", 4, 6, 65, 25, EVIL, demonicScale, FEAR},
    {"High Dragon", 4, 6, 65, 25, GOOD, goldenScale, FEAR}};

// Final area bosses
Monster final[] = { // You will recive a really strong weapon before this guys dw
    {"Astra, Deity of JUSTICE", 5, 8, 500, 30, GOOD, septre, NONE},
    {"Krya, Deity of MALICE", 5, 8, 500, 30, EVIL, septre, NONE},
    {"???, Deity of CHAOS", 7, 9, 750, 40, EVIL, septre, NONE}};

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
    cout << "\n[ Area: " << locColor << currentLoc << NORMAL << " ] [ Progress: " << storyProgress << " / " << maxStoryProgress << " ] [ " << playerAlignment << " ]\n";
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
    printf("║   [STATS]  [LORE]  [RUNES]   ║║     [CRAFT] [BREW] [SHOP]    ║\n");
    printf("╚══════════════════════════════╝╚══════════════════════════════╝\n");

    char choice;
    printf("> ");
    cin >> choice;
    
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
        int armorHold = currentArmor.getValue(); // had to add this since cant get reference from getValue(). 
        openInventory(&armorHold);
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
        cin >> newLocation;
        if (newLocation != 0 && newLocation <= sizeof(availableLocations)/sizeof(availableLocations[0])){
            system("cls");
            location = newLocation;
            isTravelling = 1;
            cout << "You travel to the " << availableLocations[newLocation-1] << ".\n";
        }
        else if (newLocation == 0){
            system("cls");
            location = saveLocation;
            isTravelling = 0;
            cout << "You return to the " << progressKey[storyProgress-1] << ".\n";
        }
        
        return 5;
    }
    else if (choice == '6') { // GEAR
        system("cls");
        gearMenu();
        return 6;
    }
    else if (choice == 'a' || choice == 'A') { // STATS
        system("cls");
        statsPage();
        return 7;
    }
    else if (choice == 'b' || choice == 'B') { // LORE
        system("cls");
        printf("This feature has not been added yet...\n");
        return 8;
    }
    else if (choice == 'c' || choice == 'C') { // RUNES
        system("cls");
        runeMenu();
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
    else if (choice == 'f' || choice == 'F') { // SHOP
        system("cls");
        if(storyProgress != 6 && storyProgress != 1 && storyProgress != 9) {
            printf("You can only shop in a town.");
        }
        else{
            switch (storyProgress) {
                case 1: {
                    Item shopItems[] = {healthPotion, healthElixer, bezoar};
                    int shopPrices[] = {5, 10, 20};
                    shop(shopItems, shopPrices, 3, "Welcome to the Village Bazaar!");
                    break;
                }
                case 6: {
                    Item shopItems[] = {berzerkerPotion, focusCharm, bezoar, burnOintment,};
                    int shopPrices[] = {10, 15, 20, 25};
                    shop(shopItems, shopPrices, 4, "Welcome to the Emerald Emporium!");
                    break;
                }
                case 9: {
                    Item shopItems[] = {berzerkerPotion, focusCharm, bezoar, burnOintment, warmthCrystal};
                    int shopPrices[] = {10, 15, 20, 25, 25};
                    shop(shopItems, shopPrices, 5, "Welcome to the Cavern Market!");
                    break;
                }
            }
        }
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
    return 99;
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

    addItem(gel, 1);
addItem(lumoraWing, 1);
addItem(leather, 1);
addItem(groblinTooth, 1);
addItem(emberScale, 1);
addItem(beastlyTooth, 1);
addItem(nimoraWing, 1);
addItem(trollLeather, 1);
addItem(fossilizedMoss, 1);
addItem(greatAntlers, 1);
addItem(lichPhial, 1);
addItem(shellShard, 1);
addItem(venomVial, 1);
addItem(krakenTentacle, 1);
addItem(shinyScale, 1);
addItem(gelatinousMass, 1);
 
    // while getting ASCI art from chatgpt it told me to do this or it wouldn't work, so this SHOULD NOT count towards grade
    system("chcp 65001 > nul");

    addItem(healthPotion, 1);


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
        cin >> start;
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

    while(1) {
        switch(storyProgress) {
            case 1: forestQuest(); break;
            case 2: outpostQuest(); break;
            case 3: plainsMapQuest(); break;
            case 4: plainsTempleGOOD(); break;
            case 5: plainsTempleEVIL(); break;
            case 6: emeraldCityQuest(); break;
            case 7: lakeQuest(); break;
            case 8: lakeInterlude(); break;
            case 9: crystalHeartQuest(); break;
            case 10: crystalDragonQuest(); break;
        }
    }

    while (storyProgress == 26) { // End for now
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
    if (karma < 33 && playerAlignment != "EVIL"){
        playerAlignment = "EVIL";
        printf("%sYou stray further from the natural order...\n%s", RED, NORMAL);
        printf("Even the shadows whisper your name in fear.\n\n");
        karmaAtkBoost = 1.5;
        karmaHpBoost = 1;
    }
    else if (karma <= 66 && karma > 32 && playerAlignment != "NEUTRAL"){
        playerAlignment = "NEUTRAL";
        printf("%sYou walk the line of chaos and order.%s\n", WHITE, NORMAL);
        printf("The world treats you with cautious respect.\n\n");
        karmaAtkBoost = 1;
        karmaHpBoost = 1;
    }
    else if (karma > 66 && playerAlignment != "GOOD"){
        playerAlignment = "GOOD";
        printf("%sYou feel a surge of righteousness fill your heart.%s\n", CYAN, NORMAL);
        printf("The spirits sing your name with reversence.\n\n");
        karmaAtkBoost = 1;
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
        cout << "[" << (i+1) << "] " << progressKey[i] << "\n";
    }
    printf("> ");
    cin >> newProgress;
    storyProgress = newProgress;
    system("cls");
    cout << "Your story progress is [ " << progressKey[newProgress-1] << " ].\n";
}
/**
 * Used to hack in an item into your inventory wherever you are
 * Speeds up development
 */
void giveItem() {
    printf("What item do you want to add to your inventory?\n");
    printf("> ");
    // char item[64];
    // fgets(item, 64, stdin);
    // fgets(item, 64, stdin);
    // item[strcspn(item, "\n")] = '\0';
    // int amount;
    // printf("How many?\n");
    // printf("> ");
    // cin >> amount;
    // for (int i = 0; i < amount; i++)
    // {
    //     addItem(item, 1);
    // }
    // cout << "You have added " << amount << " [ " << CYAN << item << NORMAL << " ] to your inventory.\n";
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
                cout << "[" << (i+1) << "] " << availableLocations[i] << "\n";
            }     
        }
    printf("═══════════════════════════════════════\n");
    printf("╔═════════════════════════════════════╗\n");
    printf("[0] | < Return to story               ║\n");           				 
    printf("╚═════════════════════════════════════╝\n\n");
}
