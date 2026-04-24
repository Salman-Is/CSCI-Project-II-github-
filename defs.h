#ifndef DEFS_H
#define DEFS_H
# include <cstdio>
# include <cstring>
# include <stdlib.h>
# include <time.h>
# include <ctype.h>
# include <unistd.h>
# include <windows.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

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
#define PINK "\x1b[38;5;211m"

// formatting
#define BOLD "\x1b[1m"
#define UNBOLD "\x1b[22m"
#define ITALIC "\033[3m"
#define UNTALIC "\033[23m"
#define RESETCURSOR "\033[H"


typedef struct {
    string name;
    int quantity;
} Inventory;

typedef enum {
    NONE, // does nothing (white)
    POISON, // damaged every turn (-15% hp) (purple)
    BURN, // damaged every turn (-5 hp), attack reduced (-25%) (orange)
    FEAR, // attack is halved, will always run when spared (dark blue)
    FROZEN, // 50% chance to not attack (cyan)
    BLEED, // damaged every turn (-25% hp), attack doubled (red)
    REGENERATION, // heals every turn (10% hp) (hot pink)
    DIVINE, // this status cannot be overwitten (immune to all status effects) (gold)
    BLIND, // enemies have a chance to miss attacks, player can't see some of enemies pattern (black)
    // UNIMPLEMENTED
    CURSED, // chance to reflect damage when the victim attacks (gray)
} StatusType;

typedef struct {
    string name;
    int difficultyLevel;
    int patternLength;
    int hp;
    int atk;
    int alignment;
    string drop;
    StatusType status;
} Monster;

typedef struct Item_struct {
    string name;
    string description;
    string itemCategory; // weapon, armor, potion etc
    string itemType;     // sword, bow, healing etc
    string color;        // WHITE, GREEN, CYAN, PURPLE, GOLD, YELLOW
    int value;             // defense, healing, damage etc.
    StatusType status;     // additional effects
} Item;



extern Item currentSword;
extern Item currentBow;
extern Item currentArmor;

#endif