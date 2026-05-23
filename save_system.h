// ============================================================
// save_system.h  -  Ashen Realms
// MODULE: Data Persistence & Leaderboard System
// AUTHOR: Haris
// DESCRIPTION:
//   Declares all functions for saving, loading, and ranking
//   players.  The system uses TEXT FILES (.txt) for storage so
//   that saves persist between game sessions.
//
//   Two files are used:
//     save.txt        -- stores the current player's full state
//     leaderboard.txt -- stores the top 5 scores ever recorded
// ============================================================

#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include <string>
#include "player.h"     // needs Player struct

using namespace std;

// ============================================================
// LEADERBOARD CONSTANTS & STRUCT
// ============================================================

const int MAX_LEADERBOARD = 5;  // keep only the top 5 scores

// One entry in the leaderboard
struct LeaderboardEntry
{
    string name;        // player's name
    int    level;       // level reached
    int    gold;        // gold at game end
    int    reputation;  // reputation at game end
    int    score;       // combined score (calculated formula)
};

// ============================================================
// FUNCTION DECLARATIONS
// Bodies are in save_system.cpp.
// ============================================================

// Write the full player state to "save.txt"
void saveGame(const Player &p);

// Read "save.txt" back into a Player struct.
// Returns true if a save was found, false if starting fresh.
bool loadGame(Player &p);

// Calculate a numeric score from the player's stats
// Formula: (level * 100) + (reputation * 10) + gold - (suspicion * 5)
int calculateScore(const Player &p);

// Add this player's run to the leaderboard (if score is high enough)
void saveToLeaderboard(const Player &p);

// Read all entries from "leaderboard.txt" into an array
void loadLeaderboard(LeaderboardEntry entries[], int &count);

// Sort entries in descending score order (bubble sort)
void sortLeaderboard(LeaderboardEntry entries[], int count);

// Print the leaderboard table to the console
void displayLeaderboard();

#endif // SAVE_SYSTEM_H
