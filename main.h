#ifndef MAIN_H
#define MAIN_H

#define RED "\x1b[31m"
#define BLACK "\x1b[30m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define NORMAL "\x1b[m"

#define BOLD "\x1b[1m"
#define UNBOLD "\x1b[22m"

#define GOOD  1
#define EVIL  -1


typedef struct {
    char name[30];
    int quantity;
} Inventory;

// the storyProgress variable tracks where the player is in the story
// storyProgress = 0 means you are at the tutorial area, 1 means you are in area 1 etc.
extern int storyProgress;
extern int location; // 1=forest, 2=plains, 3=lake 

extern int battleStart;

// ik it's lazy to do this but ill do it anyway lol
extern int currentEnemyHP;
extern int currentEnemyATK;
extern int currentEnemyALIGNMENT;
extern char currentEnemyDrop[32];

/* ================= PLAYER STATS ================= */
extern int karma;
extern int maxTurnDamage;  // Sword Damage (counterattacks) <-- from correct pattern
extern int maxPlayerTurnDamage; // Arrow Damage (direct attacks) <-- from choosing to fire arrow
extern char playerName[];
extern int playerMaxHP;
extern int coins;

extern Inventory inventory[100];
extern int inventoryCount;

extern int worldState;

#endif 