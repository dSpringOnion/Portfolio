#include "table.h"
#include "constants.h"
#include <iostream>
#include <vector>
#include <map>
#include <cstdint>

using namespace std;

// Location -> table.cpp
vector<vector<double>> phero_table;

void showPheroTable()
{
    for (int i = 0; i < phero_table.size(); i++)
    {
        for (int j = 0; j < phero_table[i].size(); j++)
        {
            cout << phero_table[i][j] << " ";
        }
        cout << endl;
    }
}
// Location -> table.cpp
void resizePheroTables(int num_nodes)
{
    phero_table.resize(num_nodes);
    for (int i = 0; i < num_nodes; i++)
    {
        phero_table[i].resize(num_nodes, STARTING_PHEROMONE);
    }
}
