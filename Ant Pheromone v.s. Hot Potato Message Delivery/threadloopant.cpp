#include <thread>
#include <chrono>
#include <string.h>
#include "mbox.h"
#include "structs.h"
#include "constants.h"
#include "loader.h"
#include <mutex>
#include <iostream>

using namespace std;

void antEndCleanup(int file_num)
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
void recvThreadLoopAnt(uint16_t mid, chrono::steady_clock::time_point thread_start_time)
{
    this_thread::sleep_until(thread_start_time);
    double recv_interval = FIXED_INTERVAL + OFFSET_RECV;
    int byte_total = 0;
    char *char_arr;
    int it = 0;
    while (!mbox_empty(mid) || thread_statuses.thread_done_with_main_work.size() < thread_statuses.num_threads || all_data.messages_received < all_data.messages_sent)
    {
        double top = mid * recv_interval;
        double top_div_nodes = top / static_cast<double>(all_nodes.number_of_nodes);
        double it_interval = static_cast<double>(it) * static_cast<double>(recv_interval);
        int wake_up_at = top_div_nodes + it_interval;
        auto wake = thread_start_time + chrono::milliseconds(wake_up_at);
        this_thread::sleep_until(wake);
        try
        {
            byte_total += mbox_recv(mid, char_arr, 1000);
        }
        catch (exception &e)
        {
            cerr << e.what() << endl;
        }
        it++;
    }
}
void threadLoopAnt(uint16_t mid, chrono::steady_clock::time_point thread_start_time)
{
    thread recv_thread_ant(recvThreadLoopAnt, mid, thread_start_time);
    this_thread::sleep_until(thread_start_time);
    int byte_total = 0;
    int it = 0;
    string sender_id_string = to_string(mid);
    string builder = "start:Hello from -> " + sender_id_string;
    const int len = builder.length();
    char *char_arr = new char[len + 1];
    strcpy(char_arr, builder.c_str());
    while (it < NUM_MESSAGES)
    {
        double top = mid * FIXED_INTERVAL;
        double top_div_nodes = top / static_cast<double>(all_nodes.number_of_nodes);
        double it_interval = static_cast<double>(it) * static_cast<double>(FIXED_INTERVAL);
        int wake_up_at = top_div_nodes + it_interval;
        auto wake = thread_start_time + chrono::milliseconds(wake_up_at);
        this_thread::sleep_until(wake);
        mbox_send(mid, char_arr, len);
        it++;
    }
    thread_status_lock.lock();
    thread_statuses.thread_done_with_main_work.push_back(true);
    thread_status_lock.unlock();
    recv_thread_ant.join();
}