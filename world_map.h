// ============================================================
// world_map.h  -  Ashen Realms
// MODULE: World Map & Dynamic Memory System
// AUTHOR: Ahmad
// DESCRIPTION:
//   This header declares all constants and functions for the
//   world map system.  The Player struct is NO LONGER defined
//   here -- it comes from player.h so the whole project shares
//   one consistent definition.
//
//   The map is a dynamically allocated 2D char array (char**).
//   Each cell holds one tile character representing a location.
// ============================================================

#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <iostream>
#include "player.h"     // brings in the shared Player struct

using namespace std;

// ============================================================
// MAP CONSTANTS
// 'const' means these values can never be changed by accident.
// Using named constants makes the code easier to read and update.
// ============================================================
const int map_size = 8;         // the map is 8 x 8 tiles

// ============================================================
// TILE CHARACTERS
// Each location on the map is stored as a single character.
// When the map is printed these characters tell the player
// what kind of place they are looking at.
//
//  '.'  = empty plain (nothing happens)
//  'P'  = the player's current position
//  'V'  = village (trade, reputation boost)
//  'C'  = castle  (reputation & experience)
//  'F'  = forest  (random encounter or item)
//  'R'  = ruins   (find gold, gain experience)
//  'E'  = enemy camp (triggers combat)
// ============================================================
const char title_empty  = '.';
const char title_player = 'P';
const char title_vil    = 'V';
const char title_castle = 'C';
const char title_forest = 'F';
const char title_ruins  = 'R';
const char title_enemy  = 'E';

// ============================================================
// FUNCTION DECLARATIONS
// Bodies are in world_map.cpp.
// ============================================================

// Allocate memory for a new map grid and return a pointer to it
char** makeMap();

// Free all allocated memory when the game ends (prevents leaks)
void freeMap(char** grid);

// Randomly fill every cell with a tile character
void fillMap(char** grid);

// Clear the screen and draw the current map + player stats
void showMap(char** grid, Player p);

// Read the player's input and move them on the map.
// Returns true if the move was valid, false otherwise.
bool handleMove(Player &p, char input);

// Return the tile character at the player's current position
char checkTile(char** grid, Player p);

#endif // WORLD_MAP_H
