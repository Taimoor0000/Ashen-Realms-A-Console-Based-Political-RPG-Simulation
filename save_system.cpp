#include <iostream>
#include <fstream>
#include <string>
#include "save_system.h"
#include "player.h"

using namespace std;

void saveGame(const Player &p)
{
    ofstream file("save.txt");

    if (!file)
    {
        cout << "\n[X] Error: Could not open save.txt for writing!\n";
        return;
    }

    file << p.name                    << "\n";
    file << p.health << " " << p.maxHealth << "\n";
    file << p.gold                    << "\n";
    file << p.reputation << " " << p.suspicion << "\n";
    file << p.x << " " << p.y        << "\n";
    file << p.level << " " << p.experience
         << " " << p.experienceToNextLevel << "\n";
    file << p.attack << " " << p.defense << "\n";
    file << p.isAlive << " " << p.turnsPlayed
         << " " << p.enemiesDefeated << "\n";

    file << p.inventory.itemCount << " "
         << p.inventory.potions   << " "
         << p.inventory.weapons   << " "
         << p.inventory.artifacts << "\n";

    for (int i = 0; i < p.inventory.itemCount; i++)
    {
        file << p.inventory.items[i] << "\n";
    }

    file.close();
    cout << "\n[OK] Game saved to save.txt\n";
}

bool loadGame(Player &p)
{
    ifstream file("save.txt");

    if (!file)
    {
        cout << "\n[!] No save file found. Starting a new game...\n";
        return false;
    }

    getline(file, p.name);

    file >> p.health >> p.maxHealth;
    file >> p.gold;
    file >> p.reputation >> p.suspicion;
    file >> p.x >> p.y;
    file >> p.level >> p.experience >> p.experienceToNextLevel;
    file >> p.attack >> p.defense;
    file >> p.isAlive >> p.turnsPlayed >> p.enemiesDefeated;

    file >> p.inventory.itemCount
         >> p.inventory.potions
         >> p.inventory.weapons
         >> p.inventory.artifacts;

    file.ignore();

    for (int i = 0; i < p.inventory.itemCount; i++)
    {
        getline(file, p.inventory.items[i]);
    }

    file.close();

    clampStats(p);

    cout << "\n[OK] Game loaded for player: " << p.name << "\n";
    return true;
}

int calculateScore(const Player &p)
{
    return (p.level * 100)
         + (p.reputation * 10)
         + p.gold
         - (p.suspicion * 5);
}

void saveToLeaderboard(const Player &p)
{
    LeaderboardEntry entries[MAX_LEADERBOARD];
    int count = 0;
    loadLeaderboard(entries, count);

    LeaderboardEntry newEntry;
    newEntry.name       = p.name;
    newEntry.level      = p.level;
    newEntry.gold       = p.gold;
    newEntry.reputation = p.reputation;
    newEntry.score      = calculateScore(p);

    if (count < MAX_LEADERBOARD)
    {
        entries[count] = newEntry;
        count++;
    }
    else
    {
        int lowestIdx = 0;
        for (int i = 1; i < count; i++)
        {
            if (entries[i].score < entries[lowestIdx].score)
                lowestIdx = i;
        }

        if (newEntry.score > entries[lowestIdx].score)
            entries[lowestIdx] = newEntry;
        else
        {
            cout << "\n[i] Score " << newEntry.score
                 << " did not make the top " << MAX_LEADERBOARD << ".\n";
            return;
        }
    }

    sortLeaderboard(entries, count);

    ofstream file("leaderboard.txt");
    if (!file)
    {
        cout << "\n[X] Could not save leaderboard!\n";
        return;
    }

    file << count << "\n";

    for (int i = 0; i < count; i++)
    {
        file << entries[i].name       << "\n";
        file << entries[i].level      << " "
             << entries[i].gold       << " "
             << entries[i].reputation << " "
             << entries[i].score      << "\n";
    }

    file.close();
    cout << "\n[OK] Leaderboard updated!\n";
}

void loadLeaderboard(LeaderboardEntry entries[], int &count)
{
    ifstream file("leaderboard.txt");
    count = 0;

    if (!file)
        return;

    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++)
    {
        getline(file, entries[i].name);
        file >> entries[i].level
             >> entries[i].gold
             >> entries[i].reputation
             >> entries[i].score;
        file.ignore();
    }

    file.close();
}

void sortLeaderboard(LeaderboardEntry entries[], int count)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - 1 - i; j++)
        {
            if (entries[j].score < entries[j + 1].score)
            {
                LeaderboardEntry temp = entries[j];
                entries[j]           = entries[j + 1];
                entries[j + 1]       = temp;
            }
        }
    }
}

void displayLeaderboard()
{
    LeaderboardEntry entries[MAX_LEADERBOARD];
    int count = 0;
    loadLeaderboard(entries, count);

    cout << "\n========== LEADERBOARD ==========\n";

    if (count == 0)
    {
        cout << "  No scores recorded yet.\n";
        cout << "=================================\n";
        return;
    }

    cout << "  Rank | Name            | Lvl | Gold | Rep | Score\n";
    cout << "  -----|-----------------|-----|------|-----|------\n";

    for (int i = 0; i < count; i++)
    {
        cout << "   " << (i + 1)
             << "   | " << entries[i].name;

        int padLen = 15 - (int)entries[i].name.length();
        for (int sp = 0; sp < padLen; sp++) cout << " ";

        cout << " | "  << entries[i].level
             << "   | " << entries[i].gold
             << "   | " << entries[i].reputation
             << "  | " << entries[i].score << "\n";
    }

    cout << "=================================\n";
}