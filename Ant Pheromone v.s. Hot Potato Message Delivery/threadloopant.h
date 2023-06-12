#include <thread>
#include <chrono>
#include "mbox.h"
#include "structs.h"
#include "constants.h"

using namespace std;

void antEndCleanup(int file_num);

void recvThreadLoopAnt(uint16_t mid, chrono::steady_clock::time_point thread_start_time);

void threadLoopAnt(uint16_t mid, chrono::steady_clock::time_point thread_start_time);