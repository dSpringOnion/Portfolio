#include <iostream>
#include <thread>
#include "structs.h"
#include <thread>
#include <random>
#include <string.h>
#include "loader.h"
#include "table.h"
#include "constants.h"
#include "anthelpers.h"
#include "threadloopant.h"
#include "threadloophot.h"
#include "printhelpers.h"
#include "rand_functs.h"
#include "argumentprocessor.h"

/*
    if no coorelation found. The structure of the graph is likely what causes a change in hop count
    if there is a graph cycle with only one exit

    small factor being the average neighbors decreases as hops increases
*/
/*
    Project 3
    Group: 6
    Members: Nicholas Baird, John Leidy, Daniel Park, Zane Smith
*/

// compile -> g++ main.cpp mailbox.cpp mailboxhelpers.cpp structs.cpp loader.cpp table.cpp constants.cpp anthelpers.cpp threadloopant.cpp threadloophot.cpp printhelpers.cpp rand_functs.cpp argumentprocessor.cpp -o routing

using namespace std;

bool single_run = true;

auto start_time = chrono::steady_clock::now();
auto thread_start_time = start_time + chrono::seconds(2);

void single_run_handle()
{
    this_thread::sleep_for(chrono::seconds(2));
    FILENAME = "./graph/A" + to_string(FILE_NUMBER) + ".dat";
    all_nodes = createNodes(loadData(FILENAME));
    vector<thread> threads;
    start_time = chrono::steady_clock::now();
    thread_start_time = start_time + chrono::seconds(2);
    if (HOT_OR_ANT == "ant")
    {
        resizePheroTables(all_nodes.number_of_nodes);
        showPheroTable();
    }
    cout << "Threads have been given a start time, starting in 2 seconds." << endl;
    for (int i = 0; i < all_nodes.all_nodes.size(); i++)
    {
        if (HOT_OR_ANT == "hot")
        {
            threads.emplace_back(thread(threadLoop, i));
            thread_statuses.num_threads++;
        }
        else if (HOT_OR_ANT == "ant")
        {
            threads.emplace_back(thread(threadLoopAnt, i, thread_start_time));
            thread_statuses.num_threads++;
        }
    }
    // showPheroTables();
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
    threads.clear();
    if (HOT_OR_ANT == "hot")
    {
        hotEndCleanup(FILE_NUMBER);
    }
    else if (HOT_OR_ANT == "ant")
    {
        showPheroTable();
        antEndCleanup(FILE_NUMBER);
    }
}

// Location -> main.cpp
// This function takes all_nodes and creates threads for the number of nodes we have in our x.dat file.
// At the end it will print out the data we have calculated.
void thread_handle()
{
    showStartup();
    this_thread::sleep_for(std::chrono::seconds(5));
    // for loop... start at 10 end at 50
    // create file_name string
    // load new file
    for (int i = START_FILE_NUM; i < END_FILE_NUM + 1; i++)
    {
        FILENAME = "./graph/A" + to_string(i) + ".dat";
        all_nodes = createNodes(loadData(FILENAME));
        resizePheroTables(all_nodes.number_of_nodes);
        showPheroTable();
        vector<thread> threads;
        start_time = chrono::steady_clock::now();
        thread_start_time = start_time + chrono::seconds(2);
        cout << "Threads have been given a start time, starting in 2 seconds." << endl;
        for (int i = 0; i < all_nodes.all_nodes.size(); i++)
        {
            if (HOT_OR_ANT == "hot")
            {
                threads.emplace_back(thread(threadLoop, i));
                thread_statuses.num_threads++;
            }
            else if (HOT_OR_ANT == "ant")
            {
                threads.emplace_back(thread(threadLoopAnt, i, thread_start_time));
                thread_statuses.num_threads++;
            }
        }
        // showPheroTables();
        for (int i = 0; i < threads.size(); i++)
        {
            threads[i].join();
        }
        threads.clear();
        if (HOT_OR_ANT == "hot")
        {
            hotEndCleanup(i);
        }
        else if (HOT_OR_ANT == "ant")
        {
            showPheroTable();
            phero_table.clear();
            antEndCleanup(i);
        }
        this_thread::sleep_for(std::chrono::seconds(5));
    }
}
// Location -> main.cpp
// Entry point
int main(int argc, char *argv[])
{
    if (processArguments(argc, argv))
    {
        if (single_run)
        {
            single_run_handle();
        }
        else if (!single_run)
        {
            thread_handle();
        }
    }
    else
    {
        cout << topSeparatorGen("Unable to process arguments.", '_');
        cout << "Arguments are to be entered as follows..." << endl;
        cout << "routing [-d <sec>] [-r hot|ant] <filename>" << endl
             << endl;
        cout << "Example: routing -d 1000 -r ant 12" << endl;
        cout << "sec: int >= 100 (ms)\nhot|ant: the string hot or the string ant\nfilename: the number on the filename you wish to use 10 <= FILE_NUMBER <= 50" << endl;
        cout << bottomSeparatorGen('_');
    }
    return 0;
}