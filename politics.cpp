#include <iostream>
#include "politics.h"
#include "player.h"

using namespace std;

// Init
void initPolitics(PoliticsState &ps)
{
    ps.counteven = 0;

    for (int i = 0; i < 20; i++)
        ps.firedEvents[i] = -1;
}

// Seen
bool hasEventFired(const PoliticsState &ps, int id)
{
    for (int i = 0; i < ps.counteven; i++)
        if (ps.firedEvents[i] == id)
            return true;

    return false;
}

// Save
void logEvent(PoliticsState &ps, int id)
{
    if (ps.counteven < 20)
    {
        ps.firedEvents[ps.counteven] = id;
        ps.counteven++;
    }
}

// Event
void triggerEvent(Player &p, PoliticsState &ps, const PoliticalEvent &ev)
{
    // Gate
    if (ev.isSecretMission && !(p.reputation > 70 && p.suspicion < 20))
    {
        cout << "\n[!] A secret stirs... but you are not trusted enough yet.\n";
        return;
    }

    if (ev.isRestricted && !(p.reputation > 40 && p.suspicion < 30))
    {
        cout << "\n[!] The locals eye you with suspicion. They won't speak to you.\n";
        return;
    }

    // Show
    cout << "\n============ POLITICAL EVENT ============\n";
    cout << ev.title << "\n";
    cout << "-----------------------------------------\n";
    cout << ev.event << "\n";
    cout << "-----------------------------------------\n";
    cout << "  1. " << ev.choice_1 << "\n";
    cout << "  2. " << ev.choice_2 << "\n";
    cout << "  3. " << ev.choice_3 << "\n";
    cout << "=========================================\n";
    cout << "  Your choice (1/2/3): ";

    int choice;
    cin >> choice;

    // Apply
    switch (choice)
    {
        case 1:
            updateReputation(p, ev.ch_1_rep);
            updateSuspicion(p, ev.ch_1_sus);

            if (ev.ch_1_gold > 0)
                addGold(p, ev.ch_1_gold);

            if (ev.ch_1_gold < 0)
                spendGold(p, -ev.ch_1_gold);

            break;

        case 2:
            updateReputation(p, ev.ch_2_rep);
            updateSuspicion(p, ev.ch_2_sus);

            if (ev.ch_2_gold > 0)
                addGold(p, ev.ch_2_gold);

            if (ev.ch_2_gold < 0)
                spendGold(p, -ev.ch_2_gold);

            break;

        case 3:
            updateReputation(p, ev.ch_3_rep);
            updateSuspicion(p, ev.ch_3_sus);

            if (ev.ch_3_gold > 0)
                addGold(p, ev.ch_3_gold);

            if (ev.ch_3_gold < 0)
                spendGold(p, -ev.ch_3_gold);

            break;

        default:
            cout << "\n[!] Invalid choice. The moment passes.\n";
            return;
    }

    // Mark
    logEvent(ps, ev.id);
    showPoliticalStatus(p);
}

// Village
void runVillageEvent(Player &p, PoliticsState &ps)
{
    PoliticalEvent villageEvents[4];

    // Elder
    villageEvents[0] = {
        1,
        "The Village Elder's Request",
        "An old elder approaches. Bandits raided his village.\nHe begs for your help.",
        "Help him for free",
        "Help him but demand 20 gold",
        "Refuse and walk away",
        10, -5,  0,
         5,  5, 20,
        -10, 10,  0,
        false, false
    };

    // Tax
    villageEvents[1] = {
        2,
        "The King's Tax Collector",
        "A royal tax collector is harassing poor farmers.\nWhat do you do?",
        "Defend the farmers",
        "Stay out of it",
        "Help the collector intimidate them",
        15, -10,  0,
         0,   0,  0,
        -15,  15, 30,
        false, false
    };

    // Spy
    villageEvents[2] = {
        3,
        "The Hooded Messenger",
        "A cloaked figure hands you a sealed letter.\n'Deliver this to the castle,' he whispers.",
        "Accept the mission",
        "Refuse politely",
        "Report him to the guards",
        10,  10,  50,
        -5,   0,   0,
         20, -15,   0,
        true, false
    };

    // Resist
    villageEvents[3] = {
        4,
        "The Resistance Contacts You",
        "A secret group opposing the corrupt king finds you.\n'We need a champion,' they say.",
        "Join their cause",
        "Ask for time to think",
        "Decline - too dangerous",
        25, -5,  100,
         5,  5,    0,
        -5,  0,    0,
        false, true
    };

    // Pick
    bool found = false;

    for (int i = 0; i < 4; i++)
    {
        if (!hasEventFired(ps, villageEvents[i].id))
        {
            triggerEvent(p, ps, villageEvents[i]);
            found = true;
            break;
        }
    }

    if (!found)
        cout << "\n[Village] The villagers have nothing new for you today.\n";
}

// Castle
void runCastleEvent(Player &p, PoliticsState &ps)
{
    PoliticalEvent castleEvents[3];

    // King
    castleEvents[0] = {
        5,
        "Audience with the King",
        "The king summons you. He demands you swear loyalty\nand crush a rebel village.",
        "Swear loyalty and obey",
        "Negotiate - offer another solution",
        "Refuse the king openly",
        -10,  20,  50,
          5,   0,   0,
         20, -20,   0,
        false, false
    };

    // Spy
    castleEvents[1] = {
        6,
        "The Royal Spymaster",
        "The king's spymaster offers you a secret role:\nspy on the northern alliance.",
        "Accept the spy role",
        "Decline but keep the secret",
        "Expose the spymaster publicly",
        -5,  25,  80,
        -5,   5,   0,
        25, -10,   0,
        true, false
    };

    // Prince
    castleEvents[2] = {
        7,
        "The Exiled Prince",
        "A hidden prince reveals himself.\nHe asks you to help reclaim the throne.",
        "Pledge your sword to the prince",
        "Stay neutral but wish him luck",
        "Betray him to the king for reward",
         30, -10, 0,
          0,   0, 0,
        -30,  30, 150,
        false, true
    };

    // Pick
    bool found = false;

    for (int i = 0; i < 3; i++)
    {
        if (!hasEventFired(ps, castleEvents[i].id))
        {
            triggerEvent(p, ps, castleEvents[i]);
            found = true;
            break;
        }
    }

    if (!found)
        cout << "\n[Castle] The court has no new business for you today.\n";
}

// Status
void showPoliticalStatus(const Player &p)
{
    cout << "\n+----------------------------------+\n";
    cout << "|      POLITICAL STANDING          |\n";
    cout << "+----------------------------------+\n";

    cout << "| Reputation : " << p.reputation
         << " (" << getReputationStatus(p) << ")\n";

    cout << "| Suspicion  : " << p.suspicion;

    if (p.suspicion >= 80)
        cout << " [DANGER!]";

    cout << "\n+----------------------------------+\n";
}
