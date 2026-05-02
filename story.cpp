#include "battle.h"
#include "main.h"
#include "ui.h"
#include "events.h"

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

// Emerald City quest
Item* quest5Rewards[] =  { &emeraldBow, &focusCharm, &elderiteGemstone, &berzerkerPotion };

// Infected Lake quest
Item* quest6RewardsGOOD[] = { &tidebreakerArmor, &shellShard, &healthElixer, &mysticSalve };

// --- Quest Variables ---
int startQuest1 = 0;
int startQuest2 = 0;
int startQuest3 = 0;
int startQuest4 = 0;
int startQuest5 = 0;
int startQuest6 = 0;
int startQuest7 = 0;
int startQuest8 = 0;
// --- Quest Flags ---
int quest1Action = 0;
int quest5Action = 0;
int quest6Action = 0;

void forestQuest() {
    while (storyProgress == 1){
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
}

void outpostQuest() {
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
}

void plainsMapQuest() {
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
}

void plainsTempleGOOD() {
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
                string correct[] = {"moon", "sun", "star"};
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
}

void plainsTempleEVIL() {
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
                string correct[] = {"moon", "sun", "star"};
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
}

void emeraldCityQuest() {
    while (storyProgress == 6) {
        int navigationChoice = options();
        if (navigationChoice == 1) {
            specialPrintf("After days of travel, the trees begin to thin and the road widens.\n");
            specialPrintf("You crest a hill and see it for the first time...\n");
            specialPrintf("Emerald City. The city of commerce.\n");
            specialPrintf("Its walls are tall and green-tinted, catching the light like polished stone.\n");
            specialPrintf("You make your way to the gate. A guard stops you. Would you like to speak to them?\n\n");

            int speak = dialouge();
            if (speak == 1) {
                system("cls");
                dialougeBox("City Guard", WHITE, "GUARD_1");
                startQuest5++;
            }
            else if (speak == 2) {
                printf("The guard watches you carefully as you pass through the gate...\n");
            }

            if (startQuest5 != 0) {
                specialPrintf("The city is in turmoil. Word has spread of the Lich's resurrection.\n");
                specialPrintf("The Kingdom's council is divided on how to respond.\n");
                specialPrintf("One faction wants to mobilize the Royal Knights to declare war on the remaining monsters in the Plains.\n");
                specialPrintf("Another believes the threat has passed with the Lich's defeat.\n");
                specialPrintf("A royal advisor approaches you. She has heard of what you did in the temple.\n\n");
                pressEnter();

                dialougeBox("Royal Advisor Sera", GOLD, "SERA_1");

                string questChoice = questAlignment(
                    "Convince the council the threat is over",
                    "Declare war on the remaining monsters"
                );

                if (questChoice == "GOOD") {
                    specialPrintf("You tell the council the threat is gone. You fought the Lich, after all.\n");
                    specialPrintf("The journey here was calm, much more than the journey to the temple had been.\n");
                    specialPrintf("The council believes you. They agree to stand down.\n");
                    specialPrintf("You collect your reward and leave.\n\n");
                    pressEnter();
                    questRewards(quest5Rewards, 2, 50);
                    pressEnter();
                    specialPrintf("As you were walking down the front steps of the council building, you are stopped by a Royal Knight.\n");
                    pressEnter();
                    
                    dialougeBox("Corrupt Captain", DARKBLUE, "CORRUPT_1");

                    if (bossFight(corruptCaptain) == 1) {
                        dialougeBox("Corrupt Captain", DARKBLUE, "CORRUPT_2");
                        dialougeBox("Royal Advisor Sera", GOLD, "SERA_2");
                        dialougeBox("Corrupt Captain", DARKBLUE, "CORRUPT_3");
                        dialougeBox("Royal Advisor Sera", GOLD, "SERA_3");
                        specialPrintf("You defeated The Captain!\n\n");
                        specialPrintf("You watch as he is taken into custody by his own knights...\n\n");
                        pressEnter();
                        }
                    else {
                        printf("You were killed by The Captain!\n");
                        pressEnter();
                    }
                }
                else if (questChoice == "EVIL") {
                    specialPrintf("You tell the council the threat is worse than ever. You fought the Lich, after all.\n");
                    specialPrintf("Who would dare question you?\n");
                    specialPrintf("The council believes you. The city continues to rally the Knights.\n");
                    specialPrintf("You collect your reward and leave before anyone discovers the truth.\n\n");
                    pressEnter();
                    questRewards(quest5Rewards, 2, 50);
                    pressEnter();
                    specialPrintf("As you were walking down the front steps of the council building, you are stopped by Sera.\n");
                    dialougeBox("Royal Advisor Sera", GOLD, "SERA_4");
                    quest5Action = 1;
                }
                else {
                    continue;
                }
                pressEnter();
            }
        }
    }
}

void storyLake() {
    while (storyProgress == 7) {
        int navigationChoice = options();
        if (navigationChoice == 1) {
            specialPrintf("You leave Emerald City behind and follow the road west.\n");
            specialPrintf("The air grows humid as the trees give way to a vast shoreline.\n");
            specialPrintf("The Blue Lake stretches out before you, dark and churning.\n");
            specialPrintf("It shouldn't be. The sky is clear. There is no wind.\n");
            specialPrintf("A small fishing village sits at the water's edge, abandoned.\n");
            specialPrintf("Every door is shut. Every window is dark.\n\n");
            
            specialPrintf("The exception is the door closest to you. Do you want to knock?\n\n");
            
            int speak = dialouge();
            specialPrintf("You knock on the nearest door. A long silence. Then it opens a crack.\n\n");
            pressEnter();
            if (speak == 1) {
                system("cls");
                dialougeBox("Fisherman Edric", CYAN, "EDRIC_1");
                startQuest6++;
            }
            else if (speak == 2) {
                printf("You turn away for now.\n");
            }

            if (startQuest6 != 0) {
                dialougeBox("Fisherman Edric", CYAN, "EDRIC_2");
                dialougeBox("Fisherman Edric", CYAN, "EDRIC_3");

                specialPrintf("You take the old diving bell from the village storehouse.\n");
                specialPrintf("The water is black around you as you descend.\n");
                specialPrintf("You can feel something wrong in it. Something that shouldn't be here.\n");
                specialPrintf("The deeper you go, the louder the silence gets.\n\n");
                pressEnter();

                specialPrintf("At the bottom, you find him.\n");
                specialPrintf("The spirit is bound to the lakebed by chains of dark energy.\n");
                specialPrintf("He is barely conscious. The infection is spreading from him outward.\n");
                specialPrintf("And standing over him is the thing responsible.\n\n");
                pressEnter();

                dialougeBox("Vael, the Drowned", DEEPRED, "VAEL_1");

                specialPrintf("The demon turns to face you fully.\n");
                specialPrintf("The water around it warps and darkens.\n\n");
                pressEnter();

                string questChoice = questAlignment("Fight Vael and free the spirit", "Strike a deal with Vael and leave the spirit to his fate");

                if (questChoice == "GOOD") {
                    specialPrintf("You draw your weapon. The water resistance is immense.\n");
                    specialPrintf("Vael doesn't move. It watches you approach.\n");
                    specialPrintf("Then it smiles, and the lake erupts around you.\n\n");
                    pressEnter();

                    if (bossFight(vael) == 1) {
                        system("cls");
                        dialougeBox("Vael, the Drowned", DEEPRED, "VAEL_2");
                        specialPrintf("Vael fractures. The dark energy holding it together scatters into the water.\n");
                        specialPrintf("The chains binding the spirit dissolve.\n");
                        specialPrintf("The lake shudders. Then goes still.\n\n");
                        pressEnter();

                        dialougeBox("Lirien, Spirit of the Lake", CYAN, "LIRIEN_1");
                        dialougeBox("Lirien, Spirit of the Lake", CYAN, "LIRIEN_2");

                        specialPrintf("The water begins to clear around you as you ascend.\n");
                        specialPrintf("By the time you surface, the lake is blue again.\n");
                        specialPrintf("The whole village is standing at the dock.\n\n");
                        pressEnter();

                        dialougeBox("Fisherman Edric", CYAN, "EDRIC_4");

                        questRewards(quest6RewardsGOOD, 4, 60);
                        quest6Action++;
                        storyProgress++;
                    }
                    else {
                        printf("Vael dragged you into the dark...\n");
                        pressEnter();
                    }
                }
                else if (questChoice == "EVIL") {
                    specialPrintf("You stop. You look at the demon. Then at the spirit.\n");
                    specialPrintf("This deal draws your intrest...\n");
                    specialPrintf("You signal to Vael that you want to talk.\n\n");
                    pressEnter();

                    dialougeBox("Vael, the Drowned", DEEPRED, "VAEL_3");
                    dialougeBox("Lirien, Spirit of the Lake", CYAN, "LIRIEN_3");
                    dialougeBox("Vael, the Drowned", DEEPRED, "VAEL_4");
                    dialougeBox("Lirien, Spirit of the Lake", CYAN, "LIRIEN_4");

                    specialPrintf("Lirien breaks free from Vael's chains, and grabs his trident.\n");
                    specialPrintf("You take a breath and exit the diving bell. Vael assumes a battle stance.\n");
                    specialPrintf("Though weakened, Lirien easily overpowers Vael, and turns to you.\n");
                    pressEnter();

                    if (bossFight(lirien) == 1) {               

                        dialougeBox("Lirien, Spirit of the Lake", CYAN, "LIRIEN_5");
                        dialougeBox("Vael, the Drowned", DEEPRED, "VAEL_5");

                        specialPrintf("You surface to find Edric waiting at the dock, hopeful.\n");
                        specialPrintf("You tell him there was nothing you could do.\n");
                        specialPrintf("The look on his face will stay with you longer than you'd like.\n\n");
                        pressEnter();

                        quest6Action--;
                        storyProgress++;
                    }
                    else {
                        printf("You were sealed by Lirien under the Lake...\n");
                        pressEnter();
                    }
                }
                else {
                    continue;
                }
                pressEnter();
            }
        }
    }
}

