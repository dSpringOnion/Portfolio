#include <thread>
#include <iostream>
#include <chrono>
#include <random>
#include <string.h>
#include "structs.h"
#include "mbox.h"

using namespace std;

void hotEndCleanup(int file_num);

void threadLoop(uint16_t mid);