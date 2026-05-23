#include <iostream>
#include <cstdlib>
#include <ctime>


#include "player.h"      // Bilal   player stats & inventory
#include "world_map.h"   // Ahmad   map, movement, tiles
#include "combat.h"      // Hadi   combat & enemy AI
#include "save_system.h" // Haris  âsave, load, leaderboard
#include "politics.h"    // Taimoor political events

using namespace std;

int main()
{
    srand(time(0)); // random seed called ONCE here only

    Player       p;   
    PoliticsState ps; 

    cout << "=========== ASHEN REALMS ===========\n";
    cout << "  1. New Game\n";
    cout << "  2. Load Game\n";
    cout << "  3. Leaderboard\n";
    cout << "  4. Quit\n";
    cout << "  Choice: ";

    int menuChoice;
    cin >> menuChoice;

    if (menuChoice == 4) return 0;

    if (menuChoice == 3)
    {
        displayLeaderboard();
        return 0;
    }

    if (menuChoice == 2)
    {
        if (!loadGame(p)) //falls back to new game if no save
            initializePlayer(p); 
    }
    else
    {
        initializePlayer(p); //fresh start
        cout << "\nEnter your name: ";
        cin >> p.name;
    }

    initPolitics(ps); //reset political state

    //Build the map
    char** grid = makeMap();
    fillMap(grid);

    // make sure starting tile is empty so no instant event
    grid[p.x][p.y] = title_empty;

    //Game loop
    while (isPlayerAlive(p))
    {
        // 1. Show map (Ahmad)
        showMap(grid, p);

        // 2. Check win condition (Bilal)
        if (p.reputation >= 80 && p.turnsPlayed >= 20)
        {
            cout << "\n[!!!] YOU WIN! The kingdom hails you as a hero!\n";
            break;
        }

        // 3. Check suspicion lose condition
        if (p.suspicion >= 100)
        {
            cout << "\n[!!!] The guards arrest you. GAME OVER.\n";
            break;
        }

        // 4. Get player input
        cout << "\n  Move (WASD) | I=Inventory | X=Save | Q=Quit: ";
        char input;
        cin >> input;

        // clear bad input so cin doesn't infinite loop
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n[!] Invalid input. Use W/A/S/D to move.\n";
            continue;
        }

        // 5. Handle non-movement keys
        if (input == 'i' || input == 'I')
        {
            displayInventory(p);
            cout << "  Use a potion? (y/n): ";
            char ans; cin >> ans;
            if (ans == 'y' || ans == 'Y') usePotion(p); 
            continue;
        }

        if (input == 'x' || input == 'X')
        {
            saveGame(p);
            continue;
        }

        if (input == 'q' || input == 'Q')
        {
            saveGame(p);            
            saveToLeaderboard(p); 
            cout << "\nGoodbye!\n";
            break;
        }
        bool moved = handleMove(p, input);
        if (!moved) continue;

        p.turnsPlayed++; // count the turn

        // 7. Check tile and trigger event
        char tile = checkTile(grid, p);

        if (tile == title_vil)  //village: political event (Taimoor)
        {
            runVillageEvent(p, ps);
        }
        else if (tile == title_castle)
        {
            runCastleEvent(p, ps);
        }
        else if (tile == title_forest)
        {
            int roll = rand() % 100;
            if (roll < 50)
            {
                cout << "\n[Forest] Something lurks in the trees...\n";
                startCombat(p); 
            }
            else
            {
                cout << "\n[Forest] You find a quiet clearing and rest. (+10 HP)\n";
                updateHealth(p, 10);
            }
        }
        else if (tile == title_ruins) 
        {
            int roll = rand() % 100;
            if (roll < 30)
            {
                cout << "\n[Ruins] You find ancient gold! (+30 gold)\n";
                addGold(p, 30); 
            }
            else if (roll < 60)
            {
                cout << "\n[Ruins] A trap springs! (-15 HP)\n";
                updateHealth(p, -15); 
            }
            else
            {
                cout << "\n[Ruins] You discover a strange artifact.\n";
                addToInventory(p, "Ancient Artifact"); 
            }
        }
        else if (tile == title_enemy)
        {
            cout << "\n[Enemy Camp] You are ambushed!\n";
            startCombat(p); 
        }
        else
        {
            cout << "\n[.] Empty plains. Nothing happens.\n";
        }

        // 8. Check health lose condition
        if (!isPlayerAlive(p))
        {
            cout << "\n[!!!] You have fallen. GAME OVER.\n";
            break;
        }
    }
    //save score to leaderboard
    saveToLeaderboard(p);
    displayLeaderboard();

    freeMap(grid); //free dynamic memory

    return 0;
}
