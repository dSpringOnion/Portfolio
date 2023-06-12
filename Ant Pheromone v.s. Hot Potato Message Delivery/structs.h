#ifndef STRUCTS
#define STRUCTS
#include <vector>
#include <string>
#include <mutex>

using namespace std;

// Location -> structs.h
//  Singular node containing an id and the nodes neighbors
struct node
{
    // he wants node_id and neighbots in strings... change this!
    string node_id;
    vector<string> neighbors;
};

// Location -> structs.h
// -all nodes in existence-
// This contains a vector of nodes with all nodes.
// Each has its own node_id and vector of neighbors.
struct nodes
{
    int number_of_edges;
    int number_of_nodes;
    vector<node> all_nodes;
};

// Location -> structs.h
// struct for keeping track of all data
struct data_threads
{
    vector<double> travel_times;
    vector<int> hop_counts;
    double average_travel_time;
    double average_hop_count;
    int messages_sent;
    int messages_received;
    int num_threads;
    int num_messages_per_thread;
};

struct thread_status
{
    int num_threads;
    vector<bool> thread_done_with_main_work;
};

struct edge
{
    string start_node;
    string end_node;
};

struct edges
{
    int number_of_edges;
    int number_of_nodes;
    vector<edge> all_edges;
};

// Location -> structs.h
// Contains all of the data from our message passing.
extern data_threads all_data;

extern nodes all_nodes;

extern thread_status thread_statuses;

extern mutex thread_status_lock;

nodes createNodes(vector<string> results);

string getNeighborsString(node node);

void printNodes();

void printData(int file_num);

void showStartup();

double calculateAverageTime(vector<double> time_vec);

double calculateAverageHops(vector<int> hop_vec);

double averageNeighbors();

int numNodesOneNeighbor();

#endif