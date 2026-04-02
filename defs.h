#ifndef DEFS_H
#define DEFS_H

#define RED "\x1b[31m"
#define BLACK "\x1b[30m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define NORMAL "\x1b[m"

// extended pallete
// https://jakob-bagterp.github.io/colorist-for-python/ansi-escape-codes/extended-256-colors/#examples_2
#define LIME "\x1b[38;5;40m"
#define ORANGE "\x1b[38;5;202m"
#define DARKBLUE "\x1b[38;5;17m"
#define LILAC "\x1b[38;5;225m"
#define GRAY "\x1b[38;5;235m"
#define GOLD "\x1b[38;5;220m"
#define PURPLE "\x1b[38;5;141m"
#define DEEPRED "\x1b[38;5;9m"
#define PALEYELL "\x1b[38;5;181m"

// formatting
#define BOLD "\x1b[1m"
#define UNBOLD "\x1b[22m"
#define ITALIC "\033[3m"
#define UNTALIC "\033[23m"
#define RESETCURSOR "\033[H"


typedef struct {
    char name[30];
    int quantity;
} Inventory;

typedef enum {
    NONE, // does nothing
    POISON, // damaged every turn (-15% hp)
    BURN, // damaged every turn (-5 hp), attack reduced (-25%)
    FEAR, // attack is halved
    FROZEN, // 50% chance to not attack
    BLEED, // damaged every turn (-25% hp), attack doubled
} StatusType;

typedef struct {
    char name[30];
    int difficultyLevel;
    int patternLength;
    int hp;
    int atk;
    int alignment;
    char drop[30];
    StatusType status;
} Monster;

typedef struct Item_struct {
    char name[64];
    char description[500];
    char itemCategory[16]; // weapon, armor, potion etc
    char itemType[16];     // sword, bow, healing etc
    char color[16];        // WHITE, GREEN, CYAN, PURPLE, GOLD, YELLOW
    int value;             // defense, healing, damage etc.
    StatusType status;     // additional effects
} Item;



extern Item currentSword;
extern Item currentBow;
extern Item currentArmor;

#endif