#ifndef LOADER
#define LOADER
#include <string>
#include <vector>

using namespace std;

extern string FILENAME;

extern int START_FILE_NUM;

extern int END_FILE_NUM;

vector<string> loadData(string filename);

void writeOnDone(int num_nodes, int num_edges, double avg_hop_count, double avg_neighbors, int single_neighbor_node_count);

#endif