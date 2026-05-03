#include "battle.h"
#include "main.h"
#include "ui.h"
#include "events.h"
#include "defs.h"

string areaColor();
string changeColor(StatusType status);
StatusType enemyStatus = NONE;
StatusType playerStatus = NONE;

string Item::getName(){ return name; }
string Item::getDesc(){ return description; }
string Item::getCategory(){ return itemCategory; }
string Item::getType(){ return itemType; }
string Item::getColor(){ return color; }
int Item::getValue(){ return value; }
StatusType Item::getStatus(){ return status; }


string currentLoc = "Forest";
/* ================= ITEM DEFINITIONS ================= */

// currently not implimented into the game at all
// probably gonna have to be a future addition lol

// Swords ------------------------------------------------------------------
Item ironSword = Item("Iron Sword", "A basic, chipped sword found in a cave.", "Weapon", "Sword", WHITE, 5, NONE);
Item steelSword = Item("Steel Sword", "A sturdy bow forged with iron and carbon.", "Weapon", "Sword", WHITE, 6, NONE);

Item goldSword = Item("Gold Sword", "A heavy, oriental sword unfit for battle, though it still works.", "Weapon", "Sword", GREEN, 8, NONE);
Item knightSword = Item("Knight Sword", "The offical sword used by Knights of Aureveil.", "Weapon", "Sword", GREEN, 8, BLEED);
Item grassBlade = Item("Grass Blade", "A strange sword enchanted with natural magic.", "Weapon", "Sword", GREEN, 10, POISON);
Item flameBlade = Item("Grass Blade", "An enchanted sword, lit ablaze.", "Weapon", "Sword", GREEN, 11, BURN);
Item staffOfRemnant = Item("Staff of Kyra's Remnant", "A cursed spear created by Racher the Lich", "Weapon", "Sword", CYAN, 12, FEAR);
Item iceSword = Item("Ice Sword", "A blade enchanted with the cold of the north", "Weapon", "Sword", CYAN, 12, FROZEN);
Item tideSword = Item("Tide Sword", "A sword forged from the depths of the Blue Lake, blessed by Lirien.", "Weapon", "Sword", CYAN, 20);
Item emeraldSword = Item("Emerald Sword", "A dazzling sword that blinds opponents with is sparkle.", "Weapon", "Sword", CYAN, 21, BLIND);
Item ancientSword = Item("Ancient Sword", "A strange sword enchanted with ancient magic.", "Weapon", "Sword", PURPLE, 19, FEAR);
Item crystalSpear = Item("Crystal Spear", "A spear carved from the Crystal Heart by the Dwarf Clan. Cold to the touch.", "Weapon", "Sword", CYAN, 25, FROZEN);

Item ultraSword = {"Ultra Sword", "An OP sword used for testing and nothing else", "Weapon", "Sword", RED, 999, FEAR};

// Bows ------------------------------------------------------------------
Item woodenBow = Item("Wooden Bow", "A simple, useable bow found in a cave.", "Weapon", "Bow", WHITE, 4, NONE);
Item steelBow = Item("Steel Bow", "A wooden bow reinforced with steel.", "Weapon", "Bow", WHITE, 5, NONE);
Item knightBow = Item("Knight Bow", "The offical bow used by Knights of Aureveil.", "Weapon", "Bow", GREEN, 6, BLEED);
Item rimegrassBow = Item("Rimegrass Bow", "A strange bow enchanted with natural magic.", "Weapon", "Bow", GREEN, 6, POISON);
Item flameBow = Item("Flame Bow", "An enchanted bow that lights arrows on fire", "Weapon", "Bow", GREEN, 7, BURN);
Item frostBow = Item("Frost Bow", "An enchanted bow that freezes the air around an arrow", "Weapon", "Bow", GREEN, 7, FROZEN);
Item emeraldBow = Item("Emerald Bow", "A dazzling bow that blinds opponents with is sparkle", "Weapon", "Bow", GREEN, 8, BLIND);
Item ancientBow = Item("Ancient Bow", "A bow forged in a age long past, by a civilization long forgotten", "Weapon", "Bow", PURPLE, 14, FEAR);

Item ultraBow = Item("Ultra Bow", "An OP bow used for testing and nothing else", "Weapon", "Bow", RED, 999, FEAR);

// Armor ------------------------------------------------------------------
// hp starts at 25, the number next to the name is how much it adds to your health, and the int at the end is your health after addition
Item chainArmor = Item("Chain Armor", "A flimsy set of armor found in a cave.", "Armor", "Armor", WHITE, 25);
Item steelArmor = Item("Steel Armor", "A set of armor forged with iron and carbon.", "Armor", "Armor", WHITE, 27);
Item knightArmor = Item("Knight Armor", "The offical set of armor used by Knights of Aureveil.", "Armor", "Armor", GREEN, 29);
Item swordmasterArmor = Item("Swordmaster Armor", "A set of armor forged by the famous Swordmaster Lorel.", "Armor", "Armor", GREEN, 32);
Item tidebreakerArmor = Item("Tidebreaker Armor", "The personal set of armor used by Lirien of the Blue Lake.", "Armor", "Armor", CYAN, 37);
Item crystalArmor = Item("Crystal Armor", "Armor gifted by the Crystal Heart and the Dwarf Clan. Impossibly light.", "Armor", "Armor", CYAN, 42);

Item ultraArmor = Item("Ultra Armor", "An OP set of armor used for testing and nothing else", "Armor", "Armor", RED, 999);

// Drops ------------------------------------------------------------------

// Forest ------------------------------------------------------------------
Item gel = Item("Gel", "An oozing mass with little use", "Drop", "NULL", WHITE, 1);
Item lumoraWing = Item("Lumora Wing", "The fragile wing of a Lumora", "Drop", "NULL", WHITE, 3);
Item leather = Item("Leather", "A supple material useful for crafting", "Drop", "NULL", WHITE, 2);
Item groblinTooth = Item("Groblin Tooth", "A blunt tooth used for crushing prey", "Drop", "NULL", WHITE, 4);
Item emberScale = Item("Ember Scale", "A firey scale with magical properties", "Drop", "NULL", GREEN, 5);

// Forest Quests ------------------------------------------------------------------
Item groblinStaff = Item("Groblin Staff", "A crude staff used by Groblin Shamans.", "Drop", "NULL", PURPLE, 10, POISON);
Item groblinTusk = Item("Groblin Tusk", "A large tusk from a Groblin Chief.", "Drop", "NULL", WHITE, 10);
Item scrapMetal = Item("Scrap Metal", "Bent and battered metal, barely useful.", "Drop", "NULL", GRAY, 5);
Item refinedMetal = Item("Refined Metal", "Well-crafted metal stripped from a Knight.", "Drop", "NULL", WHITE, 10);
Item infernoScale = Item("Inferno Scale", "A blazing scale from an Elder Flagon.", "Drop", "NULL", ORANGE, 15, BURN);

// Plains ------------------------------------------------------------------
Item beastlyTooth = Item("Beastly Tooth", "A sharp, serated tooth that causes infection", "Drop", "NULL", WHITE, 15);
Item nimoraWing = Item("Nimora Wing", "The fragile wing of a Lumora", "Drop", "NULL", WHITE, 4);
Item trollLeather = Item("Troll Leather", "Leather that has been hardened by Troll engineering", "Drop", "NULL", WHITE, 6);
Item fossilizedMoss = Item("Fossilized Moss", "Hardened moss from the back of an ancient creature", "Drop", "NULL", GREEN, 12);
Item greatAntlers = Item("Great Antlers", "Proof you took down a Great Stag", "Drop", "NULL", WHITE, 13);
Item lichPhial = Item("Lich's Phial", "All that remains of The Lich", "Drop", "NULL", PURPLE, 57);

// Plains Quests ------------------------------------------------------------------
Item brokenDagger = Item("Broken Dagger", "A cracked dagger that has seen better days.", "Drop", "NULL", GRAY, 10);
Item fairyDust = Item("Fairy Dust", "A glittering powder with magical properties.", "Drop", "NULL", CYAN, 20);
Item vileRemnant = Item("Vile Remnant", "A dark residue left by an undead creature.", "Drop", "NULL", DARKBLUE, 10, FEAR);;

// Lake ------------------------------------------------------------------
Item shellShard = Item("Shell Shard", "A sturdy shard of a Mega Turtle that is the base of Tide Armor", "Drop", "NULL", WHITE, 7);
Item venomVial = Item("Venom Vial", "A drop of venom collected from a Lake Serpent", "Drop", "NULL", WHITE, 9);
Item krakenTentacle = Item("Kraken Tentacle", "The arm of a Kraken useful for potions", "Drop", "NULL", WHITE, 20);
Item shinyScale = Item("Shiny Scale", "A gleaming scale that can be used to craft charms", "Drop", "NULL", WHITE, 10);
Item gelatinousMass = Item("Gelatinous Mass", "A large, oozing mass with little use", "Drop", "NULL", WHITE, 10);
Item luminousScale = Item("Luminous Scale", "A glowing scale from a Glowfin.", "Drop", "NULL", CYAN, 10);

// Caves ------------------------------------------------------------------
Item echoFang = Item("Echo Fang", "A hollow fang that hums faintly.", "Drop", "NULL", PURPLE, 11, BLEED);
Item crystalVenom = Item("Crystal Venom", "A crystallized venom with toxic properties.", "Drop", "NULL", PURPLE, 11, POISON);
Item quartzShard = Item("Quartz Shard", "A jagged shard of quartz crystal.", "Drop", "NULL", CYAN, 15);
Item mirrorCloak = Item("Mirror Cloak", "A shimmering cloak that bends light.", "Drop", "NULL", WHITE, 12);
Item mineralSilk = Item("Mineral Silk", "Incredibly tough silk spun by a Stone Spider.", "Drop", "NULL", GRAY, 10);
Item gearCharge = Item("Gear Charge", "A charged gear harvested from an Automaton.", "Drop", "NULL", GOLD, 30);

Item diamond = Item("Diamond", "A flawless diamond from the Crystal Dragon's cavern.", "Drop", "NULL", PURPLE, 60);
Item sapphire = Item("Sapphire", "A deep blue sapphire found in the depths of the caves.", "Drop", "NULL", PURPLE, 70);
Item topaz = Item("Topaz", "A warm golden topaz plucked from the cave wall.", "Drop", "NULL", PURPLE, 50);
Item ruby = Item("Ruby", "A brilliant red ruby radiating with heat.", "Drop", "NULL", PURPLE, 65);
Item emerald = Item("Emerald", "A vivid green emerald with an unusual shimmer.", "Drop", "NULL", PURPLE, 75);

// Mountains ------------------------------------------------------------------
Item soaringFeather = Item("Soaring Feather", "A large feather from a Peak Eagle.", "Drop", "NULL", WHITE, 10);
Item ionicIce = Item("Ionic Ice", "A shard of magical ice that never melts.", "Drop", "NULL", CYAN, 14, FROZEN);
Item durableHorn = Item("Durable Horn", "An incredibly tough horn from a Mountain Goat.", "Drop", "NULL", WHITE, 13);
Item gorgeousLeather = Item("Gorgeous Leather", "Soft, luxurious leather from a Snow Leopard.", "Drop", "NULL", WHITE, 23);
Item demonicScale = Item("Demonic Scale", "A dark scale radiating with evil energy.", "Drop", "NULL", RED, 55, FEAR);
Item goldenScale = Item("Golden Scale", "A brilliant scale radiating with divine energy.", "Drop", "NULL", GOLD, 60);


// Special ------------------------------------------------------------------
Item verdentKey = Item("Verdent Key", "An emerald key found in the Plains. It has to open something...", "Item", "Key", GREEN, 0);
Item frostKey = Item("Frost Key", "A sapphire key found in the Caves. It has to open something...", "Item", "Key", CYAN, 0);
Item ancientCoin = Item("Ancient Coin", "", "Item", "NULL", PURPLE, 50);
Item elderiteGemstone = Item("Elderite Gemstone", "", "Item", "NULL", CYAN, 50);
Item kingdomCrest = Item("Kingdom Crest", "", "Item", "NULL", CYAN, 50);
Item ancientMap = Item("Ancient Map", "", "Item", "NULL", CYAN, 0);

Item essence = Item("Essence", "A gift from the Deity of Evil.", "Drop", "NULL", RED, 0);
Item ichor = Item("Ichor", "A gift from the Deity of Good.", "Drop", "NULL", CYAN, 0);

Item septre = Item("Septre", "An artifact that grants the holder Divinity.", "Charm", "Status", GOLD, 0);

Item trueSightSigil = Item("True Sight Sigil", "A sigil that allows you to see enemy HP.", "Item", "NULL", GOLD, 0);
Item regenerationCrystal = Item("Regeneration Crystal", "An artifact that grants the holder Regeneration.", "Charm", "Status", GOLD, 0);
Item alexandriteScale = Item("Alexandrite Scale", "An artifact that shields you from the first strike in battle.", "Charm", "Status", GOLD, 0);

// Potions/Charms --------------------------------------------------------------
Item healthPotion = Item("Health Potion", "A potion that will heal most injuries.", "Potion", "Healing", WHITE, 5);
Item healthElixer = Item("Health Elixer", "A magical potion made with the blessing of Astra.", "Potion", "Healing", GREEN, 10);
Item mysticSalve = Item("Mystic Salve", "An oinment imbued with the power of Regeneration.", "Potion", "Status", CYAN, 0);
Item berzerkerPotion = Item("Berzerker Potion", "An unstable potion that draws out your deep power.", "Potion", "Attack+", GREEN, 2);
Item focusCharm = Item("Focus Charm", "An artifact that brings with it a calming force.", "Charm", "Crit+", CYAN, 2);

Item burnOintment = Item("Burn Ointment", "An oinment that can cure BURN.", "Potion", "Status", WHITE, 0);
Item bezoar = Item("Bezoar", "A stone that cures POISON. Don't ask where it comes from.", "Potion", "Status", WHITE, 0);
Item warmthCrystal = Item("Warmth Crystal", "A gemstone that produces heat. It cures FROZEN.", "Potion", "Status", WHITE, 0);

/* ================= UI FUNCTIONS ================= */
void printUI(string turn, string enemyName, int enemyHP, int enemyMaxHP, int alignment, int playerHP, int playerMaxHP){
    string enStatus = statusText(enemyStatus);
    string enStatusColor = changeColor(enemyStatus);
    string plStatus = statusText(playerStatus);
    string plStatusColor = changeColor(playerStatus);
    string locColor = areaColor();

    // Intro/ Turn message thing
    if (turn == "enemy") {
        cout << "It's the " << RED << enemyName << NORMAL << "'s turn...\n\n";
    }
    else if (turn == "player") {
        if (battleStart == 0) {
            string alignmentText = (alignment == GOOD) ? "GOOD" : "EVIL";
            string alignmentColor = (alignment == GOOD) ? CYAN : RED;

           cout << "A " << BOLD << enemyName << UNBOLD << " [ " << alignmentColor << alignmentText << NORMAL << " ] stands before you...\n\n";
            battleStart = 1;
        } else {
            printf("Its your turn...\n\n");
        }
    }
    // ENEMY BOX ===================================
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("                                                                      \n");
    cout << "  [ " << RED << enemyName << NORMAL << " ] [ " << (alignment==GOOD?CYAN:RED) << (alignment==GOOD?"GOOD":"EVIL") << NORMAL << " ]\n";
    printf("                                                                      \n");
    cout << "  Status: [ " << enStatusColor << enStatus << NORMAL << " ] Drop: [ " << YELLOW << currentEnemyDrop.getName() << NORMAL << " ]\n";
    printf("                                                                      \n");
    printf("  HP: ");
    healthBar(enemyHP, enemyMaxHP, "enemy");
    printf(" Damage: [ %02d ]                           \n", currentEnemyATK);
    printf("                                                                      \n");
    printf("╚════════════════════════════════════════════════════════════════════╝\n");

    // PLAYER BOX =====================================================
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║                                                                    ║\n");
    cout << "║ [ " << BLUE << "The Paladin" << NORMAL << " ] [ " 
     << ((playerAlignment == "GOOD") ? CYAN : (playerAlignment == "EVIL") ? RED : NORMAL)
     << ((playerAlignment == "GOOD") ? "GOOD" : (playerAlignment == "EVIL") ? "EVIL" : "NEUT") 
     << NORMAL << " ]                                           ║\n";
    printf("║                                                                    ║\n");
    cout << "║ Status: [ " << plStatusColor << plStatus << NORMAL << " ]                                                   ║\n";
    printf("║                                                                    ║\n");
    printf("║ HP: [ %02d / %d ] ", playerHP, playerMaxHP);
    healthBar(playerHP, playerMaxHP, "player");
    printf(" Arrow: [ %02d ] Sword: [ %02d ]    ║\n",currentBow.getValue(), currentSword.getValue());
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
void openInventory(int *playerHP){
    system("cls");
    if (battleStart == 1) {
        printf("╔═════════════════════════════════════╗\n");
        printf("[#] | BATTLE ITEMS                    ║\n");
        printf("╚═════════════════════════════════════╝\n");
        string battleItems[] = {"Health Potion", "Health Elixer", "Focus Charm", "Berzerker Potion", "Mystic Salve"};
        int battleItemCount = sizeof(battleItems) / sizeof(battleItems[0]);
        for (int i = 0; i < (int)inventory.size(); i++) {
            for (int j = 0; j < battleItemCount; j++) {
                if (inventory.at(i).getName() == battleItems[j]) {
                    cout << "[" << (i+1) << "] "
                         << inventory.at(i).getName() << " x" << inventory.at(i).quantity << "\n";
                    break;
                }
            }
        }
        printf("╔═════════════════════════════════════╗\n");
        printf("╚═════════════════════════════════════╝\n");
        printf("Select item number to use (0 to exit): ");

        int choice;
        cin >> choice;
        system("cls");
        if (choice == 0) return;
        choice--;
        if (choice < 0 || choice >= (int)inventory.size()) {
            printf("Invalid choice.\n");
            return;
        }
        if (inventory[choice].getName() == "Health Potion") {
            removeItem(healthPotion);
            *playerHP += 5;
            if (*playerHP > currentArmor.getValue()) *playerHP = currentArmor.getValue();
            return;
        }
        if (inventory[choice].getName() == "Health Elixer") {
            removeItem(healthElixer);
            *playerHP += 10;
            if (*playerHP > currentArmor.getValue()) *playerHP = currentArmor.getValue();
            return;
        }
        if (inventory[choice].getName() == "Mystic Salve") {
            removeItem(mysticSalve);
            applyStatus(&playerStatus, REGENERATION);
            return;
        }
        if (inventory[choice].getName() == "Focus Charm") {
            removeItem(focusCharm);
            critDamage = 2;
            return;
        }
        if (inventory[choice].getName() == "Berzerker Potion") {
            removeItem(berzerkerPotion);
            attackBuff = 2;
            return;
        }
    }
    else {
        int choice;
        while (1) {
            system("cls");
            printf("╔══════════════════════════════════════════════════════════════════════════════════╗\n");
            printf("[#] | INVENTORY                                                                    ║\n");
            printf("╚══════════════════════════════════════════════════════════════════════════════════╝\n");

            for (int i = 0; i < (int)inventory.size(); i++) {
                printf("[%02d] %-16s x%d   ", (i+1), inventory.at(i).getName().c_str(), inventory.at(i).quantity);
                if ((i + 1) % 3 == 0) printf("\n");
            }

            printf("\n╔══════════════════════════════════════════════════════════════════════════════════╗");
            printf("\n╚══════════════════════════════════════════════════════════════════════════════════╝\n");
            printf("Select item to inspect (0 to exit): ");

            cin >> choice;
            system("cls");

            if (choice == 0) return;
            choice--;

            if (choice < 0 || choice >= (int)inventory.size()) {
                printf("Invalid choice.\n");
                pressEnter();
                continue;
            }

            viewItem(inventory[choice]);
            pressEnter();
        }
    }
}

void runeMenu() {
    if (unlockedRunes.empty()) {
        system("cls");
        printf("You have not unlocked any runes yet.\n");
        pressEnter();
        return;
    }
    int choice;
    while (1) {
        system("cls");
        printf("╔══════════════════════════════════════════════════════════╗\n");
        printf("║ RUNES                                                    ║\n");
        printf("╠══════════════════════════════════════════════════════════╣\n");
        if (activeRune != nullptr) {
            cout << "║ Active: " << activeRune->color << left << setfill(' ') << setw(49) << activeRune->name << NORMAL << "║\n";
        } else {
            printf("║ Active: %-49s ║\n", "None");
        }
        printf("╠══════════════════════════════════════════════════════════╣\n");
        for (int i = 0; i < (int)unlockedRunes.size(); i++) {
            cout << "║ [" << (i+1) << "] " << unlockedRunes[i].color << left << setfill(' ') << setw(51) << unlockedRunes[i].name << NORMAL << "║\n";
            cout << "║     " << left << setfill(' ') << setw(53) << unlockedRunes[i].description << "║\n";
        }
        printf("╠══════════════════════════════════════════════════════════╣\n");
        printf("║ [0] Back                                                 ║\n");
        printf("╚══════════════════════════════════════════════════════════╝\n");
        printf("> ");
        cin >> choice;
        system("cls");

        if (choice == 0) return;
        choice--;

        if (choice < 0 || choice >= (int)unlockedRunes.size()) {
            printf("Invalid choice.\n");
            pressEnter();
            continue;
        }

        activeRune = &unlockedRunes[choice];
        cout << "You equipped the " << activeRune->color << activeRune->name << NORMAL << " rune.\n";
        pressEnter();
    }
}

void viewItem(Item item) {
    system("cls");
    cout << "════════════════════════════════════════════════════\n";
    cout << " " << item.getColor() << item.getName() << NORMAL << "\n";
    cout << "════════════════════════════════════════════════════\n";
    cout << " Category : " << item.getCategory() << "\n";
    cout << " Type     : " << item.getType()     << "\n";
    cout << " Value    : " << item.getValue()    << "\n";
    if (item.getStatus() != NONE) {
        cout << " Status   : " << changeColor(item.getStatus())
             << left << setw(27) << statusText(item.getStatus()) << NORMAL << "║\n";
    }
    cout << "════════════════════════════════════════════════════\n";
    cout << item.getDesc() << "\n";
    cout << "════════════════════════════════════════════════════\n\n";
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
    cout << "Name:        " << BLUE << "The Paladin" << NORMAL << "\n";
    cout << karmaColor << "Karma:       " << karma << NORMAL << "\n";
    cout << "Coins:       " << coins << "\n";
    cout << "\n";
    cout << "Armor:       " << currentArmor.getName() << " -> x" << currentArmor.getValue() << " health\n";
    cout << "Sword:       " << currentSword.getName() << " -> x" << currentSword.getValue() << " damage\n";
    cout << "Bow:         " << currentBow.getName() << " -> x" << currentBow.getValue() << " damage\n\n";
    cout << "Alignment:   " << playerAlignment << "\n";
    cout << "\n";
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
            cout << currentColor << "■" << NORMAL;  
        else
            printf("%s■%s", GRAY, NORMAL);
    }
    printf(" ]");
}

// i found out the sleep() function is better then the time() do while loop for the cpu. and its actually way simpler. 
// also using millesecods can allow us to use partial seconds. ex 500 milleseconds is .5 seconds. 1000 milleseconds is 1 sec
// https://www.geeksforgeeks.org/c/sleep-function-in-c/

void gearMenu() {
    int choice;
    while (1) {
        system("cls");
        printf("╔══════════════════════════════════════════════════════════════════════════════════╗\n");
        printf("║  GEAR                                                                            ║\n");
        printf("╠══════════════════════════════════════════════════════════════════════════════════╣\n");
        printf("║  Select a slot to switch, or press 0 to go back.                                 ║\n");
        printf("╚══════════════════════════════════════════════════════════════════════════════════╝\n");

        printf("\n");
        printf("[1] Sword  : ");
        cout << currentSword.getColor() << BOLD << currentSword.getName() << UNBOLD << NORMAL;
        printf("  ATK: %d  Status: ", currentSword.getValue());
        cout << changeColor(currentSword.getStatus()) << statusText(currentSword.getStatus()) << NORMAL << "\n";

        printf("[2] Bow    : ");
        cout << currentBow.getColor() << BOLD << currentBow.getName() << UNBOLD << NORMAL;
        printf("  ATK: %d  Status: ", currentBow.getValue());
        cout << changeColor(currentBow.getStatus()) << statusText(currentBow.getStatus()) << NORMAL << "\n";

        printf("[3] Armor  : ");
        cout << currentArmor.getColor() << BOLD << currentArmor.getName() << UNBOLD << NORMAL;
        printf("  DEF: %d\n", currentArmor.getValue());

        printf("\n");
        printf("[0] Back\n\n");
        printf("> ");
        
        
        cin >> choice;

        if (choice == 0) return;

        system("cls");

        // figure out what slot they picked
        string targetType = "";
        if (choice == 1) targetType = "Sword";
        else if (choice == 2) targetType = "Bow";
        else if (choice == 3) targetType = "Armor";
        else {
            printf("Invalid choice.\n");
            pressEnter();
            continue;
        }

        // collect matching items from inventory
        vector<int> matches;
        for (int i = 0; i < (int)inventory.size(); i++) {
            if (inventory[i].getType() == targetType) {
                matches.push_back(i);
            }
        }

        if (matches.empty()) {
            printf("You have no other %s in your inventory.\n", targetType.c_str());
            pressEnter();
            continue;
        }
        cout << "Your current " << targetType << " is " << BOLD;
        if (choice == 1) { cout << currentSword.getColor() << currentSword.getName(); 
        } else if (choice == 2)  { cout << currentBow.getColor() << currentBow.getName();
        } else if (choice == 3) { cout << currentArmor.getColor() << currentArmor.getName();
        }
        cout << UNBOLD << NORMAL << ". Would you like to switch?\n\n";
        int confirm = dialouge();
        if (confirm == 2) { continue; }

        // display matching items
        cout << "Select a " << targetType << " to equip:\n\n";
        for (int i = 0; i < (int)matches.size(); i++) {
            Item& item = inventory[matches[i]];
            printf("[%d] ", i+1);
            cout << item.getColor() << BOLD << item.getName() << UNBOLD << NORMAL;
            printf("  Atk/Def: %d  Status: ", item.getValue());
            cout << changeColor(item.getStatus()) << statusText(item.getStatus()) << NORMAL << "\n";
        }
        printf("\n[0] Cancel\n\n");
        
        printf("> ");

        int swapChoice;
        cin >> swapChoice;

        if (swapChoice == 0) continue;
        swapChoice--;

        if (swapChoice < 0 || swapChoice >= (int)matches.size()) {
            printf("Invalid choice.\n");
            pressEnter();
            continue;
        }

        // swap
        Item newItem = inventory[matches[swapChoice]];
        removeItem(newItem);

        if (choice == 1) {
            addItem(currentSword, 1);
            currentSword = newItem;
            cout << "Equipped " << currentSword.getColor() << BOLD << currentSword.getName() << UNBOLD << NORMAL << "!\n";
        } else if (choice == 2) {
            addItem(currentBow, 1);
            currentBow = newItem;
            cout << "Equipped " << currentBow.getColor() << BOLD << currentBow.getName() << UNBOLD << NORMAL << "!\n";
        } else if (choice == 3) {
            addItem(currentArmor, 1);
            currentArmor = newItem;
            cout << "Equipped " << currentArmor.getColor() << BOLD << currentArmor.getName() << UNBOLD << NORMAL << "!\n";
        }
        pressEnter();
    }
}
