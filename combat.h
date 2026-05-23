// ============================================================
// combat.h  -  Ashen Realms
// MODULE: Combat & Enemy AI System
// AUTHOR: Hadi
// DESCRIPTION:
//   Declares the Enemy struct and all combat-related functions.
//   The combat system uses the shared Player struct from player.h
//   so that changes to player stats (health, gold, reputation)
//   are automatically reflected in the rest of the game.
// ============================================================

#ifndef COMBAT_H
#define COMBAT_H

#include <string>
#include "player.h"     // we need Player& in the function signatures

using namespace std;

// ============================================================
// ENEMY STRUCTURE
// Holds the stats for a single enemy during a combat encounter.
// ============================================================
struct Enemy
{
    string name;    // e.g. "Bandit", "Corrupt Knight"
    int health;     // enemy's current HP
    int attack;     // damage the enemy deals per hit
    int defense;    // damage reduction for the enemy
};

// ============================================================
// FUNCTION DECLARATIONS
// Bodies are in combat.cpp.
// ============================================================

// Returns true 30% of the time -- used to randomly start a fight
bool triggerEncounter();

// Creates an enemy whose stats scale with the player's level
Enemy generateEnemy(int playerLevel);

// Calculates net damage: attackerAttack minus half of defenderDefense
// Always returns at least 1 (attacks always deal some damage)
int calculateDamage(int attackerAttack, int defenderDefense);

// The enemy's turn: it attacks the player and updates p.health
void enemyTurn(Enemy &enemy, Player &p);

// Full turn-based combat loop: player attacks/defends/flees.
// Directly modifies the Player struct (gold, reputation, health, etc.)
void startCombat(Player &p);

#endif // COMBAT_H
