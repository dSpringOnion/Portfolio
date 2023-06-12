#ifndef TABLE
#define TABLE
#include <vector>
#include <map>
#include <cstdint>
using namespace std;

extern vector<vector<double> > phero_table;

void resizePheroTables(int num_nodes);

void showPheroTable();

#endif