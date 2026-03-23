#include "battle.h"
#include "main.h" 
#include "ui.h"
#include "events.h"

/*================= EVENT FUNCTIONS ================= */

// In game events found by searching, quests, etc.
// The ascii art for these were made by chatgpt by the way (except the chest & dice)
void loreTablet(char text[]) {
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

    printf("\"%s%s%s\"\n", ITALIC, text, UNTALIC);
}
// Some lore tablets have specific replies, but most don't
// This is to save time so I dont have to type responses ever time theres a new tablet
void genericLoreResponse() {
    char messages[5][64] = {
    "\nYou ponder it's meaning...\n",
    "\nThe words make you think...\n",
    "\nIt makes you question what came before...\n",
    "\nYou contemplate its significance...\n",
    "\nThe tablet sparks a quiet curiosity...\n"
    };

    int messagesIndex = rand() % 5;
    printf("%s\n", messages[messagesIndex]);
}

void riddle(char message[], char correct[], int *search, char reward[]) {
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
    printf("%s%s%s\n", GREEN, message, NORMAL);
    printf("What is your answer?\n> ");
    char answer[99];
    fgets(answer, 99, stdin);
    fgets(answer, 99, stdin);
    // doing fgets twice made it work for some reason????
    // it had some strange behavior where it sent previous inputs as the answer automatically
    // no idea why this works

    for (int i = 0; answer[i] != '\0'; i++) {
        answer[i] = tolower(answer[i]);
    }
    time_t start_time, current_time;
    printf("Your answer: %s", answer);

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

        time(&start_time);
        do {
            time(&current_time);
        } while (difftime(current_time, start_time) < .5);

        system("cls");

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

        time(&start_time);
        do {
            time(&current_time);
        } while (difftime(current_time, start_time) < .5);

        system("cls");
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

    if (strstr(answer, correct) != NULL)
    {
        printf("The Shrine accepts your answer...\n");
        printf("It rewarded you with a %s%s%s\n\n", YELLOW, reward, NORMAL);
        (*search)++;
        addItem(reward, 1);
    }
    else {
        printf("The Shrine doesn't accept your answer...\n\n");
    }
}

void chest(char key[], char item[], char chestType[], char chestColor[], int *search) {
    printf("╔══════════════════╗\n");
    printf("╠═════╠══════║═════╣\n");
    printf("║                  ║\n");
    printf("║    [        ]    ║\n");
    printf("╚══════════════════╝\n\n");
    int hasFoundKey = -1;
    printf("You find a %s%s%s chest.\n", chestColor, chestType, NORMAL);
    
    for(int i = 0; i < inventoryCount; i++) {
        if(strcmp(inventory[i].name, key) == 0) {
            hasFoundKey = 1;
            break;
        }
        else {
            hasFoundKey = 0;
        }
    }

    if (hasFoundKey == 1)
    {
        printf("You use your %s%s%s to open the chest...\n\n", chestColor, key, NORMAL);
        (*search)++;
        addItem(item, 0);
    }
    else {
        printf("Find a %s%s%s to open the chest.\n\n", chestColor, key, NORMAL);
    }
}

int dice()
{
    int roll = rand() % 6 + 1;
    
    // I know it looks strange, but this is an array of dice
    char faces[6][200] = {
        "           ╔═════════╗\n           ║         ║\n           ║    ○    ║\n           ║         ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║       ● ║\n           ║         ║\n           ║ ●       ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║       ● ║\n           ║    ○    ║\n           ║ ●       ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║ ●     ● ║\n           ║         ║\n           ║ ●     ● ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║ ●     ● ║\n           ║    ○    ║\n           ║ ●     ● ║\n           ╚═════════╝",
        "           ╔═════════╗\n           ║ ●     ● ║\n           ║ ●     ● ║\n           ║ ●     ● ║\n           ╚═════════╝"
    };

    system("cls");
    printf("%s\n", faces[roll-1]);

    return roll;
}

char* questAlignment(char goodOption[], char evilOption[]) {
    printf("\nAs The Paladin, it is your job to decide the fate of this world.\n");
    printf("In this moment, do you choose to be %sGOOD%s, or %sEVIL%s?\n\n", CYAN, NORMAL, RED, NORMAL);
    printf("[ GOOD | %s ]     [ EVIL | %s ]\n", goodOption, evilOption);
    printf("( Type GOOD or EVIL )\n");
    printf("> ");

    int chosen = 0;
    char choice[32] = "";
    scanf(" %s", choice);

    for(int i=0;i<(sizeof(choice)/sizeof(choice[0]));i++) {
        choice[i]=toupper(choice[i]);
    }

    if (strcmp(choice, "GOOD") == 0)
    {
        system("cls");
        printf("You have chosen the path of %sGOOD%s\n", CYAN, NORMAL);
        int chosen = 1;
        return "GOOD";
    }
    else if (strcmp(choice, "EVIL") == 0)
    {
        system("cls");
        printf("You have chosen the path of %sEVIL%s\n", RED, NORMAL);
        int chosen = 1;
        return "EVIL";
    }
    else {
        system("cls");
        printf("The path you seek does not exist...\n");
        return "bruh";
    }

}

void questRewards(char* rewards[], int count, int money) {
        for(int i = 0; i < count; i++)
    {
        addItem(rewards[i], 0);
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
            addItem("Steel Sword ( 6 ATK )", 0);
            maxTurnDamage = 6; // <- Need a better system than this (cuz then if you come back here and grab this your damage might get lower)
            searchPoints1++;
            break;
        case 4: 
            riddle("What has a head, a tail, and no body?", "coin", &searchPoints1, "Ancient Coin");
            break;
        case 5:
            chest("Verdent Key", "Grass Blade ( 10 ATK )", "Verdent", LIME, &searchPoints1);
            maxTurnDamage = 10; // <- Need a better system than this (cuz then if you come back here and grab this your damage might get lower)
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
            riddle("What grows without life, and consumes air with no breath?", "fire", &searchPoints2, "Flame Bow ( 7 ATK )");
            break;
        case 3:
            addItem("Rimegrass Bow ( 6 ATK )", 0);
            maxTurnDamage = 7; 
            searchPoints2++;
            break;
        case 4: 
            addCoins(25,"no");
            searchPoints2++;
            break;
        case 5:
            chest("Frost Key", "Ancient Sword ( 19 ATK )", "Frost", CYAN, &searchPoints2);
            maxTurnDamage = 9;
            break;
        case 6:
            addItem("Ancient Bow ( 14 ATK )", 0);
            maxPlayerTurnDamage = 5;
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
    char speakChoice[32];
    while(1) {
        printf("[ 1 | Yes ]     [ 2 | No ]\n");
        printf("> ");
        scanf(" %s", speakChoice);
        if (strcmp(speakChoice, "1") == 0 || strcmp(speakChoice, "yes") == 0){
            system("cls");
            return 1;
        }
        else if (strcmp(speakChoice, "2") == 0 || strcmp(speakChoice, "no") == 0) {
            system("cls");
            return 2;
        }
        printf("Invalid Choice. Please type 1/yes, or 2/no\n\n");
    }
}

// https://www.reddit.com/r/C_Programming/comments/7p0deg/aligning_outputs_from_printf_make_the_output_look/
// hours spent looking for a solution and some guy on reddit had the same problem 8 years ago
void dialougeBox(char* name, char* color, char* dialougeText[]) {
    int count = 0;
    while (dialougeText[count] != NULL) {
        count++;
    }
    time_t start_time, current_time;
    int printed = 0;
    for (int i = 0; i < count; i++)
    {
        system("cls");
        printf("[ %s%s%s ]\n", color, name, NORMAL);
        printf("╔══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗\n");
        for (int j = 0; j <= i; j++) {
            printf("║ %-112s ║\n", dialougeText[j]);
        }
        printf("╚══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n");
        time(&start_time);
        do {
            time(&current_time);
        } while (difftime(current_time, start_time) < 2);
    }
    pressEnter();
}
