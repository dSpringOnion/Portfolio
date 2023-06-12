#include "constants.h"
#include <string>
#include <vector>
#include <map>
#include <condition_variable>
#include <mutex>
using namespace std;
// Location -> constants.cpp
string HOT_OR_ANT = "ant";
// Location -> constants.cpp
int NUM_MESSAGES = 10;
// fixed time interval located in constants.cpp
int FIXED_INTERVAL = 200;
int OFFSET_RECV = 30;
// good power coeff = 4
double POWER_COEFFICIENT = 2.1;
// good phero for fixed of 20k.... 0.001
// good half life = 0.5
double DILUTION_HALF_LIFE = 20;
double STARTING_PHEROMONE = 0.1;
// good starting phero = 1.2
double STARTING_ANT_PHEROMONE = 0.1;

int FILE_NUMBER = 0;

map<uint16_t, condition_variable> cv_map;
map<uint16_t, mutex> mutex_map;
map<uint16_t, bool> notification_map;
