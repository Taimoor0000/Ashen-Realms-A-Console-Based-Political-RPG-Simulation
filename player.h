// ============================================================
// player.h  -  Ashen Realms
// MODULE: Player System & Core Statistics Engine
// AUTHOR: Bilal
// DESCRIPTION:
//   This file defines the Player data structure and declares
//   all the functions that manage the player's stats, level,
//   inventory, and game conditions.
//
//   Every other module that needs player data includes this file.
//   The .h (header) file only DECLARES things; actual code is
//   in player.cpp.
// ============================================================

// Include guard: prevents this file from being included twice
// in the same compilation unit (which would cause duplicate errors).
#ifndef PLAYER_H
#define PLAYER_H

#include <string>       // needed for string type in inventory items
using namespace std;

// ============================================================
// INVENTORY STRUCTURE
// Holds everything the player is carrying.
// A nested struct -- it will sit inside the Player struct below.
// ============================================================
struct Inventory
{
    string items[10];   // array of item names (max 10 items)
    int itemCount;      // how many items are currently held
    int potions;        // number of healing potions
    int weapons;        // number of weapons
    int artifacts;      // number of rare artifacts
};

// ============================================================
// PLAYER STRUCTURE
// The master player record used by every module in the game.
// "struct" groups related variables under one name.
// ============================================================
struct Player
{
    // --- Identity ---
    string name;                // player's chosen name

    // --- Core Stats ---
    int health;                 // current health points
    int maxHealth;              // maximum possible health
    int gold;                   // currency for trading
    int reputation;             // how much the world trusts the player
    int suspicion;              // how much the guards suspect the player

    // --- Map Position ---
    int x;                      // row on the 2D grid map
    int y;                      // column on the 2D grid map

    // --- Level & Progression ---
    int level;                  // current player level
    int experience;             // XP earned so far in this level
    int experienceToNextLevel;  // XP needed to reach next level

    // --- Combat Stats ---
    int attack;                 // damage dealt per hit
    int defense;                // damage absorbed per hit

    // --- Game State Tracking ---
    bool isAlive;               // true if player is still alive
    int turnsPlayed;            // total turns taken this session
    int enemiesDefeated;        // how many enemies have been beaten

    // --- Inventory ---
    Inventory inventory;        // all items the player carries
};

// ============================================================
// FUNCTION DECLARATIONS
// These tell the compiler "this function exists somewhere."
// The actual definitions (bodies) are in player.cpp.
// ============================================================

// -- Setup & Display --
void initializePlayer(Player &p);               // set all starting values
void displayPlayerStatus(const Player &p);      // print stats to screen
void displayInventory(const Player &p);         // print inventory to screen

// -- Stat Management --
void updateHealth(Player &p, int amount);       // add or subtract health
void addGold(Player &p, int amount);            // give gold to player
bool spendGold(Player &p, int amount);          // take gold (returns false if broke)
void updateReputation(Player &p, int amount);   // change reputation up or down
void updateSuspicion(Player &p, int amount);    // change suspicion up or down

// -- Level System --
void gainExperience(Player &p, int exp);        // award XP, check for level-up
void levelUp(Player &p);                        // increase level and stats
void checkLevelUp(Player &p);                   // see if enough XP was earned

// -- Condition Checks --
bool isPlayerAlive(const Player &p);            // returns true if player lives
bool hasWonGame(const Player &p);               // returns true if win conditions met
string getPlayerRank(const Player &p);          // "Novice Wanderer", "Knight", etc.
string getReputationStatus(const Player &p);    // "Heroic", "Neutral", "Despised", etc.

// -- Inventory Management --
bool addToInventory(Player &p, const string &item);     // add an item
bool removeFromInventory(Player &p, const string &item);// remove an item
bool hasItem(const Player &p, const string &item);      // check if item exists
void usePotion(Player &p);                              // drink a potion to heal

// -- Utility --
void clampStats(Player &p);     // enforce valid stat ranges (prevents over/underflow)
void resetPlayer(Player &p);    // reset everything back to starting values

#endif // PLAYER_H
