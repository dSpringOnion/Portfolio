#include <iostream>
#include <fstream>
#include <vector>
#include "constants.h"
/*
    Project 3
    Group: 6
    Members: Nicholas Baird, John Leidy, Daniel Park, Zane Smith
*/
using namespace std;
// Location -> loader.cpp
// Starting file number from ./graph/
int START_FILE_NUM = 10;
// Location -> loader.cpp
// Ending file number from ./graph/
int END_FILE_NUM = 50;
// Location -> loader.cpp
// You can change the string associated with this variable in order to load different .dat files for testing.
string FILENAME = "./graph/A40.dat";
// Location -> loader.cpp
// This loads data from a .dat file. stores the results in a vector<string> of results.
vector<string> loadData(string filename)
{
    ifstream f;
    string data;
    vector<string> results;
    f.open(filename);
    while (f >> data)
    {
        results.push_back(data);
    }
    f.close();

    return results;
}
void writeOnDone(int num_nodes, int num_edges, double avg_hop_count, double avg_neighbors, int single_neighbor_node_count)
{
    string filename;
    if (HOT_OR_ANT == "hot")
    {
        filename = "newtest.csv";
    }
    else if (HOT_OR_ANT == "ant")
    {
        filename = "anttest.csv";
    }
    cout << "filename writing to: " << filename << endl;
    fstream outFile;
    outFile.open(filename, fstream::app);
    outFile << to_string(num_nodes);
    outFile << ", ";
    outFile << to_string(num_edges);
    outFile << ", ";
    outFile << to_string(avg_hop_count);
    outFile << ", ";
    outFile << to_string(avg_neighbors);
    outFile << ", ";
    outFile << to_string(single_neighbor_node_count);
    outFile << ", ";
    outFile << "\n";
    outFile.close();
}