#include "structs.h"
#include "loader.h"
#include <string>
#include <vector>
#include <iostream>
#include <mutex>
#include "constants.h"

/*
    Project 3
    Group: 6
    Members: Nicholas Baird, John Leidy, Daniel Park, Zane Smith
*/

using namespace std;
// Location -> structs.cpp
// Contains all of the data from our message passing.
data_threads all_data;
// Location -> structs.cpp
nodes all_nodes;
// Location -> structs.cpp
thread_status thread_statuses;
// Location -> structs.cpp
mutex thread_status_lock;
// Location -> structs.cpp
// Given the single line string of a node in a graph from x.dat return the node id string.
string getNodeID(string node_string)
{
    int string_len = node_string.size();
    string node_id_string;
    int it = 0;
    while (it < string_len)
    {
        node_id_string += node_string[it];
        it++;
        if (node_string[it] == ':')
        {
            it = string_len;
        }
    }
    return node_id_string;
}
// Location -> structs.cpp
// Given the single line string of a node in a graph return a vector of neighbors for given node string.
vector<string> getNeighbors(string node_string)
{
    vector<string> temp_neighbors;
    int string_len = node_string.size();
    string temp;
    bool after_id = false;
    for (int i = 0; i < string_len + 1; i++)
    {
        // every comma signals a new neighbor
        if (node_string[i] == ':')
        {
            after_id = true;
        }
        else if (after_id)
        {
            if (node_string[i] == ',')
            {
                temp_neighbors.push_back(temp);
                temp = "";
            }
            else
            {
                temp += node_string[i];
            }
        }
    }
    temp_neighbors.push_back(temp);
    return temp_neighbors;
}
// Location -> structs.cpp
// Given the results from reading the entire .dat file create the nodes struct.
nodes createNodes(vector<string> results)
{
    nodes temp_nodes;
    int equal_counter = 0;
    int temp_num_edges = 0;
    int temp_num_nodes = 0;
    int size_of_results = results.size();
    for (int i = 0; i < size_of_results; i++)
    {
        if (results[i] == "=" && equal_counter == 0)
        {
            temp_nodes.number_of_nodes = stoi(results[i + 1]);
            equal_counter++;
        }
        else if (results[i] == "=" && equal_counter == 1)
        {
            temp_nodes.number_of_edges = stoi(results[i + 1]);
            equal_counter++;
        }
        else if (equal_counter == 2 && i > 11)
        {
            // here we will be at our node numbers with their neighbors
            string curr_node_str = results[i];
            node new_node;
            int temp_tens;
            int temp_ones;
            //-48 because numbers in ascii start from 48 -> 48 being 0
            new_node.node_id = getNodeID(curr_node_str);
            new_node.neighbors = getNeighbors(curr_node_str);

            temp_nodes.all_nodes.emplace_back(new_node);
        }
    }
    return temp_nodes;
}
// Location -> structs.cpp
// Calculates the average hops.
double calculateAverageHops(vector<int> hop_vec)
{
    double total, mean;
    int size = hop_vec.size();
    for (int i = 0; i < size; i++)
    {
        total += static_cast<double>(hop_vec[i]);
    }
    double size_double = static_cast<double>(size);
    mean = total / size_double;
    return mean;
}
// Location -> structs.cpp
// Calculates the average times.
double calculateAverageTime(vector<double> time_vec)
{
    double total;
    double mean;
    int size = time_vec.size();
    for (int i = 0; i < time_vec.size(); i++)
    {
        total += time_vec[i];
    }
    double size_double = static_cast<double>(size);
    mean = total / size_double;
    return mean;
}
/*---------PRINTERS BELOW-----------*/
// Location -> structs.cpp
// This returns a string to show the neighbors for a given node.
// More of a print builder than a printer.
string getNeighborsString(node node)
{
    int size = node.neighbors.size();
    string build_string = "\nNeighbors: [";
    for (int i = 0; i < size; i++)
    {
        if (i + 1 == size)
        {
            build_string += node.neighbors[i];
        }
        else
        {
            build_string += node.neighbors[i];
            build_string += ", ";
        }
    }
    build_string += "]";
    return build_string;
}
// Location -> structs.cpp
// This prints out all nodes in our nodes struct.
void printNodes()
{
    for (int i = 0; i < all_nodes.all_nodes.size(); i++)
    {
        cout << "\n\n___________________________________________________________\n\n";
        cout << "Node ID: " << all_nodes.all_nodes[i].node_id << endl;
        cout << "Node Neighbors: " << getNeighborsString(all_nodes.all_nodes[i]) << endl;
        cout << "\n\n___________________________________________________________\n\n";
    }
}
// Location -> structs.cpp
// This function can take different types of vectors and genericly print them.
template <typename T, typename A>
void printVecTemplate(vector<T, A> v)
{
    string build_string = "[ ";
    int size = v.size();
    for (int i = 0; i < size; i++)
    {
        if (i + 1 == size)
        {
            build_string += to_string(v[i]);
        }
        else
        {
            build_string += to_string(v[i]);
            build_string += ", ";
        }
    }
    build_string += "]\n";
    cout << build_string << endl;
}
// Location -> structs.cpp
double averageNeighbors()
{
    double total_edge_two = static_cast<double>(all_nodes.number_of_edges * 2);
    double avg = total_edge_two / static_cast<double>(all_nodes.number_of_nodes);
    return avg;
}

// Location -> structs.cpp
int numNodesOneNeighbor()
{
    int num = 0;
    for (int i = 0; i < all_nodes.all_nodes.size(); i++)
    {
        if (all_nodes.all_nodes[i].neighbors.size() == 1)
        {
            num++;
        }
    }
    return num;
}

void printNone()
{
    cout << endl;
}
// Location -> structs.cpp
// A printer to show our data after our set number of iterations
// Data shown: avghops, avgtravel, totalmsgs, totalrecv, num_threads, messages_sent_per_thread
void printData(int file_num)
{
    string file_num_string = to_string(file_num);
    cout << "\n\n************************ FINISHED! DATA BELOW (FILE: ./graph/A" + file_num_string + ") ************************\n******************************************************************************************\n\n";
    cout << "____________________________________________________________________________\n";
    cout << "NUMBER OF NODES: " << all_nodes.number_of_nodes << endl;
    cout << "NUMBER OF EDGES: " << all_nodes.number_of_edges << endl;
    cout << "AVERAGE HOP COUNT: " << all_data.average_hop_count << endl;
    (HOT_OR_ANT == "ant") ? cout << endl : cout << "AVERAGE TRAVEL TIME: " << all_data.average_travel_time << "s" << endl;
    cout << "TOTAL MESSAGES SENT: " << all_data.messages_sent << endl;
    cout << "TOTAL MESSAGES RECEIVED: " << all_data.messages_received << endl;
    cout << "NUMBER OF THREADS: " << all_data.num_threads << endl;
    cout << "NUMBER OF MESSAGES ORIGINALLY SENT PER THREAD: " << all_data.num_messages_per_thread << endl;
    cout << "____________________________________________________________________________\n";
    cout << "\n\n\nALL HOPS -> ";
    printVecTemplate(all_data.hop_counts);
    (HOT_OR_ANT == "ant") ? printNone() : printVecTemplate(all_data.travel_times);
    cout << "\n\n************************************SLEEPING FOR 5s**************************************\n******************************************************************************************\n";
}
// Location -> structs.cpp
//  Showing what files we will be running.
void showStartup()
{
    cout << "\n\n**********************************STARTING**********************************\n****************************************************************************\n\n";
    cout << "FILE_START: ./graph/A" << START_FILE_NUM << endl;
    cout << "FILE_END: ./graph/A" << END_FILE_NUM << endl;
    cout << "You may change the starting and ending file in loader.cpp." << endl;
    cout << "You can also change the number original messages sent per thread by \nchanging the number in the while loop condition for each thread." << endl;
    cout << "\n\n****************************************************************************\n****************************************************************************\n\n";
}