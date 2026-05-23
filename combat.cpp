#include <iostream>
#include <cstdlib>
#include <ctime>
#include "combat.h"
#include "player.h"

using namespace std;

bool triggerEncounter()
{
    int chance = rand() % 100;

    if (chance < 30)
        return true;

    return false;
}

Enemy generateEnemy(int playerLevel)
{
    Enemy enemy;

    int type = rand() % 4;

    switch (type)
    {
        case 0:
            enemy.name    = "Bandit";
            enemy.health  = 30 + playerLevel * 5;
            enemy.attack  =  8 + playerLevel * 2;
            enemy.defense =  3 + playerLevel;
            break;

        case 1:
            enemy.name    = "Wolf";
            enemy.health  = 25 + playerLevel * 4;
            enemy.attack  = 10 + playerLevel * 2;
            enemy.defense =  2 + playerLevel;
            break;

        case 2:
            enemy.name    = "Mercenary";
            enemy.health  = 40 + playerLevel * 6;
            enemy.attack  = 12 + playerLevel * 2;
            enemy.defense =  5 + playerLevel;
            break;

        case 3:
            enemy.name    = "Corrupt Knight";
            enemy.health  = 50 + playerLevel * 7;
            enemy.attack  = 15 + playerLevel * 3;
            enemy.defense =  7 + playerLevel;
            break;
    }

    return enemy;
}

int calculateDamage(int attackerAttack, int defenderDefense)
{
    int damage = attackerAttack - (defenderDefense / 2);

    if (damage < 1)
        damage = 1;

    return damage;
}

void enemyTurn(Enemy &enemy, Player &p)
{
    int damage = calculateDamage(enemy.attack, p.defense);

    cout << "\n" << enemy.name << " attacks you for " << damage << " damage!\n";

    updateHealth(p, -damage);
}

void startCombat(Player &p)
{
    Enemy enemy = generateEnemy(p.level);

    cout << "\n============ ENCOUNTER ============\n";
    cout << "A " << enemy.name << " blocks your path!\n";
    cout << "  Enemy HP: "  << enemy.health
         << " | ATK: "      << enemy.attack
         << " | DEF: "      << enemy.defense << "\n";
    cout << "===================================\n";

    while (p.health > 0 && enemy.health > 0)
    {
        cout << "\n  Your HP   : " << p.health << "/" << p.maxHealth;
        cout << "\n  Enemy HP  : " << enemy.health;
        cout << "\n\n  1. Attack";
        cout << "\n  2. Defend";
        cout << "\n  3. Flee";
        cout << "\n  4. Use Potion";
        cout << "\n  Choice: ";

        int choice;
        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                int damage = calculateDamage(p.attack, enemy.defense);

                cout << "\nYou strike the " << enemy.name
                     << " for " << damage << " damage!\n";

                enemy.health -= damage;

                if (enemy.health > 0)
                    enemyTurn(enemy, p);

                break;
            }

            case 2:
            {
                cout << "\nYou raise your shield and brace for impact!\n";

                int reducedDamage = calculateDamage(enemy.attack,
                                                    p.defense + 10);

                cout << enemy.name << " attacks for "
                     << reducedDamage << " reduced damage!\n";

                updateHealth(p, -reducedDamage);

                break;
            }

            case 3:
            {
                int fleeRoll = rand() % 100;

                if (fleeRoll < 50)
                {
                    cout << "\nYou escape into the shadows!\n";
                    return;
                }
                else
                {
                    cout << "\nCould not escape! The " << enemy.name
                         << " cuts off your retreat!\n";
                    enemyTurn(enemy, p);
                }

                break;
            }

            case 4:
            {
                usePotion(p);
                if (enemy.health > 0)
                    enemyTurn(enemy, p);
                break;
            }

            default:
                cout << "\nInvalid choice -- enter 1, 2, 3, or 4.\n";
        }
    }

    if (p.health <= 0)
    {
        cout << "\nYou were defeated by the " << enemy.name << "...\n";
    }
    else
    {
        cout << "\nYou defeated the " << enemy.name << "!\n";

        int rewardGold = 20 + rand() % 31;
        int rewardRep  =  5 + rand() % 6;
        int rewardXP   = 30 + rand() % 21;

        addGold(p, rewardGold);
        updateReputation(p, rewardRep);
        gainExperience(p, rewardXP);
        p.enemiesDefeated++;

        cout << "  Gold earned : " << rewardGold << "\n";
        cout << "  Rep gained  : " << rewardRep  << "\n";
        cout << "  XP gained   : " << rewardXP   << "\n";
    }
}