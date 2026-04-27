#include "battle.h"
#include "main.h" 
#include "ui.h"
#include "events.h"
#include "defs.h"

/*================= EVENT FUNCTIONS ================= */

// In game events found by searching, quests, etc.
// The ascii art for these were made by chatgpt by the way (except the chest & dice)
void loreTablet(string text) {
    printf("╔══════════════╗\n");
    printf("║              ║\n");
    printf("║   ═ ══ ═══   ║\n");
    printf("║   ═══ ════   ║\n");
    printf("║   ═══  ═══   ║\n");
    printf("║   ════════   ║\n");
    printf("║   ═══  ═══   ║\n");
    printf("║   ══ ═════   ║\n");
    printf("║              ║\n");
    printf("╠══════════════╣\n");
    printf("╚══════════════╝\n\n");
    printf("You found a stone tablet!\n");
    printf("Its words are thus...\n\n");

    cout << "\"" << ITALIC << text << UNTALIC << "\"\n";
}
// Some lore tablets have specific replies, but most don't
// This is to save time so I dont have to type responses ever time theres a new tablet
void genericLoreResponse() {
    string messages[5] = {
    "\nYou ponder it's meaning...\n",
    "\nThe words make you think...\n",
    "\nIt makes you question what came before...\n",
    "\nYou contemplate its significance...\n",
    "\nThe tablet sparks a quiet curiosity...\n"
    };

    int messagesIndex = rand() % 5;
    cout << messages[messagesIndex] << endl;
}

void riddle(string message, string correct, int *search, Item reward) {
    printf("          ╔════════════╗\n");
    printf("         ╔══════════════╗\n");
    printf("       ╔══════════════════╗\n");
    printf("       ║      ╔════╗      ║\n");
    printf("       ║      ║    ║      ║\n");
    printf("       ║    ╔═╩════╩═╗    ║\n");
    printf("       ║    ║        ║    ║\n");
    printf("       ║    ║        ║    ║\n");
    printf("       ╠════╩════════╩════╣\n");
    printf("       ║                  ║\n");
    printf("       ╚══════════════════╝\n\n");
    printf("You come across an Ancient Shrine...\n");
    printf("This is the riddle etched into its surface...\n\n");
    cout << GREEN << message << NORMAL << endl;
    printf("What is your answer?\n> ");
    string answer;
    getline(cin, answer);

    // doing fgets twice made it work for some reason????
    // it had some strange behavior where it sent previous inputs as the answer automatically
    // no idea why this works

    for (int i = 0; answer[i] != '\0'; i++) {
        answer[i] = tolower(answer[i]);
    }
    
    cout << "Your answer: " << answer;

    system("cls");

    for (int i = 0; i < 2; i++)
    {
        printf("          ╔════════════╗\n");
        printf("         ╔══════════════╗\n");
        printf("       ╔══════════════════╗\n");
        printf("   o   ║      ╔════╗      ║   o\n");
        printf("  o  · ║      ║    ║      ║ ·  ·\n");
        printf("    ·  ║    ╔═╩════╩═╗    ║  o\n");
        printf("     o ║    ║        ║    ║   ·\n");
        printf("  ·  . ║    ║        ║    ║    o\n");
        printf("       ╠════╩════════╩════╣\n");
        printf("       ║                  ║\n");
        printf("       ╚══════════════════╝\n\n");

        printf("The stone tablet atop the Shrine pedestal vibrates...\n");

        Sleep(350);

        printf(RESETCURSOR);

        printf("          ╔════════════╗\n");
        printf("         ╔══════════════╗\n");
        printf("    o  ╔══════════════════╗\n");
        printf(" ·     ║      ╔════╗      ║ .    ·\n");
        printf("    ·  ║      ║    ║      ║  o\n");
        printf(" o     ║    ╔═╩════╩═╗    ║    ·\n");
        printf("   ·   ║    ║        ║    ║ o\n");
        printf("  .  o ║    ║        ║    ║   ·\n");
        printf("       ╠════╩════════╩════╣\n");
        printf("       ║                  ║\n");
        printf("       ╚══════════════════╝\n\n");

        printf("The stone tablet atop the Shrine pedestal vibrates...\n");

        Sleep(350);

        printf(RESETCURSOR);
    }

    system("cls");

    printf("          ╔════════════╗\n");
    printf("         ╔══════════════╗\n");
    printf("       ╔══════════════════╗\n");
    printf("       ║      ╔════╗      ║\n");
    printf("       ║      ║····║      ║\n");
    printf("       ║    ╔═╩════╩═╗    ║\n");
    printf("       ║    ║  ·  ·  ║    ║\n");
    printf("       ║    ║  ·  ·  ║    ║\n");
    printf("       ╠════╩════════╩════╣\n");
    printf("       ║                  ║\n");
    printf("       ╚══════════════════╝\n\n");
    // https://www.youtube.com/watch?v=V32VSYVd1Ro
    // https://stackoverflow.com/questions/26821468/strstr-null-doesnt-work < THANK YOU to these dudes i was stuck here for 2 hours

    if (answer.find(correct) != string::npos)
    {
        printf("The Shrine accepts your answer...\n");
        cout << "It rewarded you with a " << YELLOW << reward.name << NORMAL << "\n\n";
        (*search)++;
        if (reward.itemType == "Bow") {
            upgradeBow(reward);
        }
        if (reward.itemType == "Sword") {
            upgradeSword(reward);
        }
        else {
            addItem(reward.name, 1);
        }
    }
    else {
        printf("The Shrine doesn't accept your answer...\n\n");
    }
}

int puzzleDoor(string correct[]) {
    printf("       ╔═════════════╗\n");
    printf("       ║             ║\n");
    printf("       ║ ╔═╗ ╔═╗ ╔═╗ ║\n");
    printf("       ║ ╚═╝ ╚═╝ ╚═╝ ║\n");
    printf("       ║             ║\n");
    printf("       ║             ║\n");
    printf("       ║             ║\n");
    printf("       ║             ║\n");
    printf("       ║             ║\n");
    printf("       ╠═════════════╣\n");
    printf("       ║  ═══   ══   ║\n");
    printf("       ╚═════════════╝\n\n");
    string answer[4];
    cout << GREEN << "What are the answers? (3 symbols) (NO SPACES)" << NORMAL << "\n\n";

    string answers[4];
    for (int i = 0; i < 3; i++) {
        printf("Symbol [ %d ]: ", i + 1);
        scanf(" %98s", answers[i]);
        // convert to lowercase
        for (int j = 0; answers[i][j] != '\0'; j++) {
            answers[i][j] = tolower(answers[i][j]);
        }
    }

    system("cls");

    for (int i = 0; i < 2; i++){
        printf("       ╔═════════════╗\n");
        printf("       ║             ║\n");
        printf("       ║ ╔═╗ ╔═╗ ╔═╗ ║\n");
        printf("       ║ ╚═╝ ╚═╝ ╚═╝ ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ╠═════════════╣\n");
        printf("       ║  ═══   ══   ║\n");
        printf("       ╚═════════════╝\n\n");

        printf("The door ponders your answer...\n");

        Sleep(350);

        printf(RESETCURSOR);

        printf("       ╔═════════════╗\n");
        printf("       ║ ╔═╗     ╔═╗ ║\n");
        printf("       ║ ╚═╝ ╔═╗ ╚═╝ ║\n");
        printf("       ║     ╚═╝     ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ╠═════════════╣\n");
        printf("       ║  ═══   ══   ║\n");
        printf("       ╚═════════════╝\n\n");

        printf("The door ponders your answer...\n");

        Sleep(350);

        printf(RESETCURSOR);

        printf("       ╔═════════════╗\n");
        printf("       ║             ║\n");
        printf("       ║ ╔═╗     ╔═╗ ║\n");
        printf("       ║ ╚═╝ ╔═╗ ╚═╝ ║\n");
        printf("       ║     ╚═╝     ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ╠═════════════╣\n");
        printf("       ║  ═══   ══   ║\n");
        printf("       ╚═════════════╝\n\n");

        printf("The door ponders your answer...\n");

        Sleep(350);

        printf(RESETCURSOR);

        printf("       ╔═════════════╗\n");
        printf("       ║             ║\n");
        printf("       ║ ╔═╗ ╔═╗ ╔═╗ ║\n");
        printf("       ║ ╚═╝ ╚═╝ ╚═╝ ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ║             ║\n");
        printf("       ╠═════════════╣\n");
        printf("       ║  ═══   ══   ║\n");
        printf("       ╚═════════════╝\n\n");

        printf("The door ponders your answer...\n");

        Sleep(350);

        printf(RESETCURSOR);
    }

    system("cls");

    printf("       ╔═════════════╗\n");
    printf("       ║             ║\n");
    printf("       ║ ╔═════════╗ ║\n");
    printf("       ║ ╚═════════╝ ║\n");
    printf("       ║             ║\n");
    printf("       ║             ║\n");
    printf("       ║             ║\n");
    printf("       ║             ║\n");
    printf("       ║             ║\n");
    printf("       ╠═════════════╣\n");
    printf("       ║  ═══   ══   ║\n");
    printf("       ╚═════════════╝\n\n");

    for (int i = 0; i < 3; i++) {
        printf("Answer %d was... ", i + 1);
        Sleep(1000);

        if (answers[i].find(correct[i]) != string::npos) {
            printf(GREEN "correct!\n" NORMAL);
            Sleep(1000);
        } else {
            printf(RED "incorrect.\n" NORMAL);
            Sleep(1000);
            return 0;
        }
    }
    return 1;
}

void chest(string key, Item item, string chestType, string chestColor, int *search) {
    printf("╔══════════════════╗\n");
    printf("╠═════╠══════║═════╣\n");
    printf("║                  ║\n");
    printf("║    [        ]    ║\n");
    printf("╚══════════════════╝\n\n");
    int hasFoundKey = -1;
    cout << "You find a " << chestColor << chestType << NORMAL << "chest.\n";
    
    for(int i = 0; i < (int)inventory.size(); i++) {
        if(inventory.at(i).name == key) {
            hasFoundKey = 1;
            break;
        }
        else {
            hasFoundKey = 0;
        }
    }

    if (hasFoundKey == 1)
    {
        cout << "You use your " << chestColor << key << NORMAL << "to open the chest...\n\n";
        (*search)++;
        if (item.itemType == "Bow") {
            upgradeBow(item);
        }
        if (item.itemType == "Sword") {
            upgradeSword(item);
        }
        if (item.itemType == "Armor") {
            upgradeArmor(item);
        }
        else {
            addItem(item.name, 1);
        }
    }
    else {
        cout << "Find a " << chestColor<< key << NORMAL << "to open the chest.\n\n";
    }
}

int dice()
{
    int roll = rand() % 6 + 1;
    
    // I know it looks strange, but this is an array of dice
    string faces[6] = {
        "           ╔═════════╗\n           ║         ║\n           ║    ○    ║\n           ║         ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║       ● ║\n           ║         ║\n           ║ ●       ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║       ● ║\n           ║    ○    ║\n           ║ ●       ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║ ●     ● ║\n           ║         ║\n           ║ ●     ● ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║ ●     ● ║\n           ║    ○    ║\n           ║ ●     ● ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║ ●     ● ║\n           ║ ●     ● ║\n           ║ ●     ● ║\n           ╚═════════╝"
    };

    system("cls");
    cout << faces[roll-1] << endl;

    return roll;
}

string questAlignment(string goodOption, string evilOption) {
    printf("\nAs The Paladin, it is your job to decide the fate of this world.\n");
    cout << "In this moment, do you choose to be " << CYAN << "GOOD" << NORMAL << ", or" << RED << "EVIL" << NORMAL << "?\n\n";
    cout << "[ GOOD | " << goodOption << " ]     [ EVIL | " << evilOption << " ]\n";
    printf("( Type GOOD or EVIL )\n");
    printf("> ");

    int chosen = 0;
    string choice = "";
    scanf(" %s", choice);

    for(int i=0;i<(sizeof(choice)/sizeof(choice[0]));i++) {
        choice[i]=toupper(choice[i]);
    }

    if (choice == "GOOD")
    {
        system("cls");
        cout << "You have chosen the path of " << CYAN << "GOOD" << NORMAL << endl;
        int chosen = 1;
        return "GOOD";
    }
    else if (choice == "EVIL")
    {
        system("cls");
        cout << "You have chosen the path of " << RED << "EVIL" << NORMAL << endl;
        int chosen = 1;
        return "EVIL";
    }
    else {
        system("cls");
        printf("The path you seek does not exist...\n");
        return "bruh";
    }

}

void questRewards(Item* rewards[], int count, int money) {
    for(int i = 0; i < count; i++) {
        Item item = *rewards[i];
        if (item.itemType == "Bow") {
            upgradeBow(item);
        } else if (item.itemType == "Sword") {
            upgradeSword(item);
        } else if (item.itemType == "Armor") {
            upgradeArmor(item);
        } else {
            addItem(item.name, 0);
        }
    }
    addCoins(money, "battle");
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

void searchArea() {
    int chanceofMonster = rand() % 100;
    if (chanceofMonster < 20){
        printf("You found a monster!\n");
        printf("You can't avoid it, prepare for battle...\n");
        Sleep(2500);
        randomEncounter();
        return;
    }
    else {
        if (chanceofMonster > 85) {        
            string shopItems[] = {"Health Potion", "Health Elixer", "Berzerker Potion", "Focus Charm"};
            int shopPrices[] = {5, 10, 15, 20};
            shop(shopItems, shopPrices, 4); 
            return;
        }
    }
    
    switch (location){ // Apparently the scope of a variable created in 1 case is the entire switch case
    case 1:
        (void)0;    
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
            upgradeSword(steelSword);
            searchPoints1++;
            break;
        case 4: 
            riddle("What has a head, a tail, and no body?", "coin", &searchPoints1, ancientCoin);
            break;
        case 5:
            chest("Verdent Key", grassBlade, "Verdent", LIME, &searchPoints1);
            break;
        case 6:
            upgradeSword(steelSword);
            break;
        default:
            loreTablet("Spare no thought for those above.\n Through their grace we are evolved.\n Through their plights we are destroyed.");
            genericLoreResponse();
            break;
        }
        break;
    case 2:
        (void)0;    
        static int searchPoints2 = 1; 
        switch (searchPoints2){
        case 1:
            addItem("Verdent Key", 0);
            searchPoints2++;
            break;
        case 2:
            riddle("What grows without life, and consumes air with no breath?", "fire", &searchPoints2, flameBow);
            break;
        case 3:
            upgradeBow(rimegrassBow);
            searchPoints2++;
            break;
        case 4: 
            addCoins(25,"no");
            searchPoints2++;
            break;
        case 5:
            chest("Frost Key", ancientSword, "Frost", CYAN, &searchPoints2);
            break;
        case 6:
            upgradeBow(ancientBow);
            searchPoints2++;
            break;
        default:
            loreTablet("Creation, Paladin.\n You will know the state of the world.\n Our planet will be one.");
            printf("This ancient message is addressed to you...");
            break;
        }
        break;
    case 3:
        (void)0;    
        static int searchPoints3 = 1; 
        switch (searchPoints3){
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
            loreTablet("Creation, Paladin.\n You will relay the state of the world.\n Our planet will be one.");
            printf("This ancient message is addressed to you...");
            break;
        }
        break;
    case 4:
        (void)0;    
        static int searchPoints4 = 1; 
        switch (searchPoints4){
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
            loreTablet("Creation, Paladin.\n You will relay the state of the world.\n Our planet will be one.");
            printf("This ancient message is addressed to you...");
            break;
        }
        break;
    case 5:
        break;
    case 6:
        break;
    default:
        break;
    }

}
/**
 * Very simple function for dialouge, just checks what the player chooses
 * and returns 1 or 0, just used to save lines of code
 */
int dialouge() {
    // return 1 if yes, return 2 if no
    string speakChoice;
    while(1) {
        printf("[ 1 | Yes ]     [ 2 | No ]\n");
        printf("> ");
        scanf(" %s", speakChoice);
        if (speakChoice == "1" || speakChoice == "yes"){
            system("cls");
            return 1;
        }
        else if (speakChoice == "2" || speakChoice == "no") {
            system("cls");
            return 2;
        }
        printf("Invalid Choice. Please type 1/yes, or 2/no\n\n");
    }
}

// https://www.reddit.com/r/C_Programming/comments/7p0deg/aligning_outputs_from_printf_make_the_output_look/
// hours spent looking for a solution and some guy on reddit had the same problem 8 years ago
void dialougeBox(string name, string color, string tag){
    char fileName[] = "dialouge.txt";
    FILE *file = fopen(fileName, "r");

    if (!file) {
        cout << "Could not open " << fileName << endl;
        return;
    }
    char line[256];
    char lines[32][512];
    int count = 0;
    int found = 0;

    // Store a tag string like "[KALEN_1]"
    // Function looks for this tag to identify which lines t use
    char target[64];
    sprintf(target, "[%s]", tag);

    /*
        This basically looks through the entire file and adds every line 
        under the tag into the string array "lines"
    */
    while (fgets(line, sizeof(line), file))
    {
        // this removes the new line at the end of fgets. 
        // This newline character is the bane of my existence
        line[strcspn(line, "\n")] = '\0';

        // skip empty lines
        if (strlen(line) == 0) {
            continue;
        }

        /*
        check if the line is the correct tag
        tags always start with a '[' btw
        
        if the tag is wrong it stops.
        */
        if (line[0] == '[')
        {
            if (strcmp(line, target) == 0) {
                found = 1;
                continue;
            }
            else if (found) {
                break; 
            }
        }
        if (found) {
            strcpy(lines[count], line);
            count++;
        }
    }

    fclose(file);

    if (!found) {
        cout << "Dialogue tag [" << tag << "] not found.\n";
        return;
    }
    system("cls");
    for (int i = 0; i < count; i++){
        printf(RESETCURSOR);

        cout << "[ " << color << name << NORMAL << " ]\n";
        printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");

        for (int j = 0; j <= i; j++) {
            cout << "║ " << left <<setw(112) << lines[j] << " ║\n";
        }

        printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");

        if (dialougeSpeed == 0) {
            Sleep(1200);
        }
        else {
            Sleep(0);
        }
    }

    pressEnter();
}

void shop(string items[], int prices[], int shop_count){
    int choice;
    while (1){ // temp solution, shops will be more robust later
        system("cls");
        printf("You found a Traveling Merchant!\n");
        printf("╔══════════════════════════════════╗\n");
        printf("║ [#] | TRAVELING MERCHANT         ║\n");
        printf("╚══════════════════════════════════╝\n\n");
        cout << YELLOW << "COINS" << NORMAL << ": " << coins << "\n\n";
        printf("╔══════════════════════════════════╗\n");
        // display items
        for (int i = 0; i < shop_count; i++){
            cout << "║ [" << (i + 1) << "] " << left << setw(19) << items[i] << right << setw(2) << prices[i] << " Coins ║\n";
        }
        printf("╚══════════════════════════════════╝\n\n");
        printf("╔══════════════════════════════════╗\n");
        printf("║ [0] | EXIT                       ║\n");
        printf("╚══════════════════════════════════╝\n\n");

        printf("> ");
        scanf("%d", &choice);

        if (choice == 0){
            system("cls");
            return;
        }

        choice--; // adjust index

        if (choice < 0 || choice >= shop_count){
            printf("Invalid choice. Try again.\n");
            pressEnter();
            continue;
        }

        if (coins >= prices[choice]){
            coins -= prices[choice];
            cout << "You bought " << items[choice] << "!\n";
            addItem(items[choice], 1);
        }
        else{
            printf("Not enough coins!\n");
        }
        pressEnter();
    }
}

void upgradeSword(Item newWeapon) {
    if (newWeapon.value > currentSword.value) {
        printf("You found a better sword!\n");
        cout << currentSword.name << " -> " << newWeapon.name << endl;

        addItem(currentSword.name, 1); //stores in inventory

        currentSword = newWeapon;
    }
    else {
        cout << "The " << newWeapon.name << " is weaker than your current sword.\n";

        addItem(newWeapon.name, 1);
    }
}

void upgradeBow(Item newWeapon) {
    if (newWeapon.value > currentBow.value) {
        printf("You found a better bow!\n");
        cout << currentBow.name << " -> " << newWeapon.name << endl;

        addItem(currentBow.name, 1); // store old one

        currentBow = newWeapon;
    }
    else {
        cout << "The " << newWeapon.name << " is weaker than your current bow.\n";
        addItem(newWeapon.name, 1);
    }
}

void upgradeArmor(Item newArmor) {
    if (newArmor.value > currentArmor.value) {
        printf("You found a better set of armor!\n");
        cout << currentArmor.name << " -> " << newArmor.name << endl;

        addItem(currentArmor.name, 1); // store old one

        currentArmor = newArmor;
    }
    else {
        cout << "The " << newArmor.name << " is weaker than your current set of armor.\n";
        addItem(newArmor.name, 1);
    }
}
