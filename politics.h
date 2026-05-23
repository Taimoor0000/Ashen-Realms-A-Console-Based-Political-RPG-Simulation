// ============================================================
// politics.h  -  Ashen Realms
// MODULE: Political Events & Decision Logic System
// AUTHOR: Taimoor
// ============================================================

#ifndef POLITICS_H
#define POLITICS_H

#include <string>
#include "player.h"

using namespace std;

// ── One political event (a card with 3 choices) ─────────────
struct PoliticalEvent
{
    int    id;           // unique event ID (used to avoid repeats)
    string title;        // short name shown on screen
    string event;        // story text the player reads

    string choice_1;     // text for option 1
    string choice_2;     // text for option 2
    string choice_3;     // text for option 3

    // stat changes per choice (rep, suspicion, gold)
    int ch_1_rep; int ch_1_sus; int ch_1_gold;
    int ch_2_rep; int ch_2_sus; int ch_2_gold;
    int ch_3_rep; int ch_3_sus; int ch_3_gold;

    bool isRestricted;    // true = needs rep>40 and sus<30
    bool isSecretMission; // true = needs rep>70 and sus<20
};

// ── Tracks which events already happened ────────────────────
struct PoliticsState
{
    int firedEvents[20]; // IDs of events that fired
    int counteven;       // how many events have fired
};

// ── Function declarations (bodies in politics.cpp) ──────────
void initPolitics(PoliticsState &ps);                          // reset state
bool hasEventFired(const PoliticsState &ps, int id);          // check repeat
void logEvent(PoliticsState &ps, int id);                     // record fired
void triggerEvent(Player &p, PoliticsState &ps,
                  const PoliticalEvent &ev);                  // show & handle event
void runVillageEvent(Player &p, PoliticsState &ps);           // called on V tile
void runCastleEvent (Player &p, PoliticsState &ps);           // called on C tile
void showPoliticalStatus(const Player &p);                    // print rep & sus

#endif // POLITICS_H
