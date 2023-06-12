#ifndef ANTHELPERS
#define ANTHELPERS
#include <chrono>
#include <string>
#include <map>
#include <vector>
#include <queue>

using namespace std;

double getNewPheroValue(chrono::time_point<chrono::system_clock> start, double current_phero_value);

void incremPheromone(uint16_t mid_we_are_at, string previous_sender, chrono::time_point<chrono::system_clock> start);

int chooseAntRecipBASIC(uint16_t mid, string previous_sender, string final_recip);

#endif