#include <iostream>
#include <string>
#include "player.h"

using namespace std;

void initializePlayer(Player &p)
{
    p.name = "Hero";

    // Core
    p.health    = 100;
    p.maxHealth = 100;
    p.gold      = 50;
    p.reputation = 0;
    p.suspicion  = 0;

    // Pos
    p.x = 0;
    p.y = 0;

    // XP
    p.level                 = 1;
    p.experience            = 0;
    p.experienceToNextLevel = 100;

    // Fight
    p.attack  = 10;
    p.defense = 5;

    // State
    p.isAlive         = true;
    p.turnsPlayed     = 0;
    p.enemiesDefeated = 0;

    // Bag
    p.inventory.itemCount = 0;
    p.inventory.potions   = 0;
    p.inventory.weapons   = 0;
    p.inventory.artifacts = 0;

    addToInventory(p, "Iron Sword");
    addToInventory(p, "Wooden Shield");
    addToInventory(p, "Health Potion");
    addToInventory(p, "Health Potion");
}

void displayPlayerStatus(const Player &p)
{
    cout << "\n+----------------------------------+";
    cout << "\n|        PLAYER STATUS             |";
    cout << "\n+----------------------------------+";
    cout << "\n| Name   : " << p.name;
    cout << "\n| Level  : " << p.level << " (" << getPlayerRank(p) << ")";
    cout << "\n| Health : " << p.health << "/" << p.maxHealth;
    cout << "\n| Gold   : " << p.gold;
    cout << "\n| Rep    : " << p.reputation << " (" << getReputationStatus(p) << ")";
    cout << "\n| Susp   : " << p.suspicion;
    cout << "\n+----------------------------------+";
    cout << "\n| Attack : " << p.attack;
    cout << "\n| Defense: " << p.defense;
    cout << "\n| XP     : " << p.experience << "/" << p.experienceToNextLevel;
    cout << "\n+----------------------------------+";
    cout << "\n| Enemies Defeated : " << p.enemiesDefeated;
    cout << "\n| Turns  Played    : " << p.turnsPlayed;
    cout << "\n+----------------------------------+\n";
}

void displayInventory(const Player &p)
{
    cout << "\n+----------------------------------+";
    cout << "\n|          INVENTORY               |";
    cout << "\n+----------------------------------+";
    cout << "\n| Potions  : " << p.inventory.potions;
    cout << "\n| Weapons  : " << p.inventory.weapons;
    cout << "\n| Artifacts: " << p.inventory.artifacts;
    cout << "\n+----------------------------------+";

    // List
    if (p.inventory.itemCount > 0)
    {
        cout << "\n| Items:";
        for (int i = 0; i < p.inventory.itemCount; i++)
        {
            cout << "\n|  " << (i + 1) << ". " << p.inventory.items[i];
        }
    }
    else
    {
        cout << "\n| No items in inventory.";
    }

    cout << "\n+----------------------------------+\n";
}

void updateHealth(Player &p, int amount)
{
    p.health += amount;

    // Cap
    if (p.health > p.maxHealth)
        p.health = p.maxHealth;

    // Death
    if (p.health <= 0)
    {
        p.health  = 0;
        p.isAlive = false;
        cout << "\n[!] You have been defeated!\n";
    }
}

void addGold(Player &p, int amount)
{
    if (amount > 0)
    {
        p.gold += amount;
        cout << "\n[+] Gained " << amount << " gold. (Total: " << p.gold << ")\n";
    }
}

bool spendGold(Player &p, int amount)
{
    if (p.gold >= amount)
    {
        p.gold -= amount;
        cout << "\n[-] Spent " << amount << " gold. (Remaining: " << p.gold << ")\n";
        return true;
    }
    else
    {
        cout << "\n[X] Not enough gold! (Have: " << p.gold
             << ", Need: " << amount << ")\n";
        return false;
    }
}

void updateReputation(Player &p, int amount)
{
    p.reputation += amount;

    // Clamp
    if (p.reputation >  100) p.reputation =  100;
    if (p.reputation < -100) p.reputation = -100;

    // Echo
    if (amount > 0)
        cout << "\n[^] Reputation +" << amount
             << " (" << getReputationStatus(p) << ")\n";
    else if (amount < 0)
        cout << "\n[v] Reputation " << amount
             << " (" << getReputationStatus(p) << ")\n";
}

void updateSuspicion(Player &p, int amount)
{
    p.suspicion += amount;

    // Clamp
    if (p.suspicion > 100) p.suspicion = 100;
    if (p.suspicion <   0) p.suspicion =   0;

    // Alert
    if (p.suspicion >= 80)
        cout << "\n[!] WARNING: The guards are watching you closely!\n";
}

void gainExperience(Player &p, int exp)
{
    p.experience += exp;
    cout << "\n[*] +" << exp << " XP!\n";

    // Check
    checkLevelUp(p);
}

void levelUp(Player &p)
{
    p.level++;
    p.maxHealth += 20;
    p.health     = p.maxHealth;
    p.attack    += 5;
    p.defense   += 3;

    // Next
    p.experienceToNextLevel = p.level * 100;

    cout << "\n[!!!] LEVEL UP! You are now Level " << p.level << "!\n";
    cout << "  Max HP -> " << p.maxHealth
         << " | ATK -> "   << p.attack
         << " | DEF -> "   << p.defense << "\n";

    // Token
    addToInventory(p, "Level " + to_string(p.level) + " Token");
}

void checkLevelUp(Player &p)
{
    // Loop
    while (p.experience >= p.experienceToNextLevel)
    {
        p.experience -= p.experienceToNextLevel;
        levelUp(p);
    }
}

bool isPlayerAlive(const Player &p)
{
    return p.isAlive && p.health > 0;
}

bool hasWonGame(const Player &p)
{
    bool levelOk = p.level >= 10;
    bool repOk   = p.reputation >= 70;
    bool suspOk  = p.suspicion <= 30;
    bool goldOk  = p.gold >= 500;

    return levelOk && repOk && suspOk && goldOk;
}

string getPlayerRank(const Player &p)
{
    if (p.level >= 15) return "Legendary Hero";
    if (p.level >= 12) return "Elite Knight";
    if (p.level >= 10) return "Veteran Warrior";
    if (p.level >=  7) return "Seasoned Fighter";
    if (p.level >=  5) return "Skilled Adventurer";
    if (p.level >=  3) return "Aspiring Knight";

    return "Novice Wanderer";
}

string getReputationStatus(const Player &p)
{
    if (p.reputation >= 80)  return "Heroic";
    if (p.reputation >= 50)  return "Respected";
    if (p.reputation >= 20)  return "Trusted";
    if (p.reputation >= -20) return "Neutral";
    if (p.reputation >= -50) return "Suspicious";
    if (p.reputation >= -80) return "Disliked";

    return "Despised";
}

bool addToInventory(Player &p, const string &item)
{
    // Full
    if (p.inventory.itemCount >= 10)
    {
        cout << "\n[X] Inventory full! Cannot add: " << item << "\n";
        return false;
    }

    // Store
    p.inventory.items[p.inventory.itemCount] = item;
    p.inventory.itemCount++;

    // Type
    if (item.find("Potion") != string::npos)
        p.inventory.potions++;
    else if (item.find("Sword") != string::npos ||
             item.find("Axe")   != string::npos ||
             item.find("Bow")   != string::npos ||
             item.find("Shield")!= string::npos)
        p.inventory.weapons++;
    else if (item.find("Artifact") != string::npos ||
             item.find("Token")    != string::npos)
        p.inventory.artifacts++;

    cout << "\n[+] Added \"" << item << "\" to inventory.\n";
    return true;
}

bool removeFromInventory(Player &p, const string &item)
{
    // Scan
    for (int i = 0; i < p.inventory.itemCount; i++)
    {
        if (p.inventory.items[i] == item)
        {
            // Shift
            for (int j = i; j < p.inventory.itemCount - 1; j++)
            {
                p.inventory.items[j] = p.inventory.items[j + 1];
            }

            p.inventory.itemCount--;

            // Type
            if (item.find("Potion") != string::npos)
                p.inventory.potions--;
            else if (item.find("Sword")  != string::npos ||
                     item.find("Axe")    != string::npos ||
                     item.find("Bow")    != string::npos ||
                     item.find("Shield") != string::npos)
                p.inventory.weapons--;
            else if (item.find("Artifact") != string::npos ||
                     item.find("Token")    != string::npos)
                p.inventory.artifacts--;

            cout << "\n[-] Removed \"" << item << "\" from inventory.\n";
            return true;
        }
    }

    // Miss
    cout << "\n[X] Item not found: " << item << "\n";
    return false;
}

bool hasItem(const Player &p, const string &item)
{
    for (int i = 0; i < p.inventory.itemCount; i++)
    {
        if (p.inventory.items[i] == item)
            return true;
    }

    return false;
}

void usePotion(Player &p)
{
    if (p.inventory.potions > 0)
    {
        // Sip
        if (removeFromInventory(p, "Health Potion"))
        {
            updateHealth(p, 30);
            cout << "\n[*] Drank a Health Potion! Restored 30 HP.\n";
        }
    }
    else
    {
        cout << "\n[X] No potions available!\n";
    }
}

void clampStats(Player &p)
{
    // HP
    if (p.health > p.maxHealth) p.health = p.maxHealth;
    if (p.health < 0) p.health = 0;

    // Rep
    if (p.reputation >  100) p.reputation =  100;
    if (p.reputation < -100) p.reputation = -100;

    // Susp
    if (p.suspicion > 100) p.suspicion = 100;
    if (p.suspicion <   0) p.suspicion =   0;

    // Gold
    if (p.gold < 0) p.gold = 0;
}

void resetPlayer(Player &p)
{
    initializePlayer(p);

    // Reset
    cout << "\n[~] Player has been reset to starting state.\n";
}
