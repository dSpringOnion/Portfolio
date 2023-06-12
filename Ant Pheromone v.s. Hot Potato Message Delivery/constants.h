#ifndef CONSTANTS
#define CONSTANTS
#include <string>
#include <map>
#include <condition_variable>
#include <mutex>
#include <vector>

using namespace std;

extern string HOT_OR_ANT;
extern int FIXED_INTERVAL;
extern int OFFSET_RECV;
extern double POWER_COEFFICIENT;
extern int NUM_MESSAGES;
extern double DILUTION_HALF_LIFE;
extern double STARTING_PHEROMONE;
extern double STARTING_ANT_PHEROMONE;
extern int FILE_NUMBER;

extern map<uint16_t, condition_variable> cv_map;
extern map<uint16_t, mutex> mutex_map;
extern map<uint16_t, bool> notification_map;

#endif