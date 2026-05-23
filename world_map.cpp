#include "world_map.h"
#include <cstdlib>      // for rand()

char** makeMap()
{
    char** grid = new char*[map_size];

    for (int i = 0; i < map_size; i++)
    {
        grid[i] = new char[map_size];
    }

    return grid;
}

void freeMap(char** grid)
{
    for (int i = 0; i < map_size; i++)
    {
        delete[] grid[i];
    }

    delete[] grid;
}

void fillMap(char** grid)
{
    for (int i = 0; i < map_size; i++)
    {
        for (int j = 0; j < map_size; j++)
        {
            int roll = rand() % 100;

            if      (roll < 10) grid[i][j] = title_vil;
            else if (roll < 18) grid[i][j] = title_castle;
            else if (roll < 35) grid[i][j] = title_forest;
            else if (roll < 45) grid[i][j] = title_ruins;
            else if (roll < 60) grid[i][j] = title_enemy;
            else                grid[i][j] = title_empty;
        }
    }
}

void showMap(char** grid, Player p)
{
    system("cls");

    cout << "=========== ASHEN REALMS ===========\n\n";

    cout << "  V=Village  C=Castle  F=Forest\n";
    cout << "  R=Ruins    E=Enemy   .=Empty   P=You\n\n";

    for (int i = 0; i < map_size; i++)
    {
        for (int j = 0; j < map_size; j++)
        {
            if (i == p.x && j == p.y)
                cout << title_player << " ";
            else
                cout << grid[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n------------------------------------\n";
    cout << "HP: "    << p.health     << "/" << p.maxHealth
         << " | Gold: "              << p.gold
         << " | Rep: "               << p.reputation
         << " | Susp: "              << p.suspicion
         << " | Lvl: "               << p.level
         << "\n------------------------------------\n";
}

bool handleMove(Player &p, char input)
{
    int nextX = p.x;
    int nextY = p.y;

    if      (input == 'w' || input == 'W') nextX--;
    else if (input == 's' || input == 'S') nextX++;
    else if (input == 'a' || input == 'A') nextY--;
    else if (input == 'd' || input == 'D') nextY++;
    else
    {
        return false;
    }

    if (nextX < 0 || nextX >= map_size ||
        nextY < 0 || nextY >= map_size)
    {
        cout << "\n[!] Cannot move outside the map boundary.\n";
        system("pause");
        return false;
    }

    p.x = nextX;
    p.y = nextY;
    return true;
}

char checkTile(char** grid, Player p)
{
    return grid[p.x][p.y];
}