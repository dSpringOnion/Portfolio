#include <thread>
#include <iostream>
#include <chrono>
#include <random>
#include <string.h>
#include "structs.h"
#include "mbox.h"
#include "loader.h"
#include "constants.h"
#include "rand_functs.h"
#include "constants.h"

using namespace std;

void hotEndCleanup(int file_num)
{
    all_data.average_hop_count = calculateAverageHops(all_data.hop_counts);
    all_data.average_travel_time = calculateAverageTime(all_data.travel_times);
    all_data.num_threads = all_nodes.all_nodes.size();
    all_data.num_messages_per_thread = all_data.messages_sent / all_data.num_threads;
    writeOnDone(all_nodes.number_of_nodes, all_nodes.number_of_edges, all_data.average_hop_count, averageNeighbors(), numNodesOneNeighbor());
    printData(file_num);
    all_data = {};
    thread_statuses = {};
}
void threadLoop(uint16_t mid)
{
    int byte_total = 0;
    int it = 0;
    while (it < NUM_MESSAGES)
    {
        this_thread::sleep_for(std::chrono::milliseconds((int)rand_exponential((double)FIXED_INTERVAL)));
        try
        {
            string sender_id_string = to_string(mid);
            string builder = "start:Hello from -> " + sender_id_string;
            const int len = builder.length();
            char *char_arr = new char[len + 1];
            strcpy(char_arr, builder.c_str());
            byte_total += mbox_send(mid, char_arr, len);
            this_thread::sleep_for(std::chrono::milliseconds((int)rand_exponential((double)FIXED_INTERVAL)));
            byte_total += mbox_recv(mid, char_arr, 1000);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
        }
        it++;
    }
    thread_status_lock.lock();
    thread_statuses.thread_done_with_main_work.push_back(true);
    thread_status_lock.unlock();
    while (!mbox_empty(mid) || thread_statuses.thread_done_with_main_work.size() < thread_statuses.num_threads || all_data.messages_received < all_data.messages_sent)
    {
        this_thread::sleep_for(std::chrono::milliseconds((int)rand_exponential((double)FIXED_INTERVAL / 2)));
        string test = "test";
        const int len = test.length();
        char *char_arr = new char[len + 1];
        strcpy(char_arr, test.c_str());
        mbox_recv(mid, char_arr, 1000);
    }
}