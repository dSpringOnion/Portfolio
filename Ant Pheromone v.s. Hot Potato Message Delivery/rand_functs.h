#ifndef RAND_FUNCTS
#define RAND_FUNCTS
#include <vector>
#include <string>

using namespace std;

int rand_unit();

double rand_zero_one_range();

double rand_exponential(double mean);

double rand_range(double min, double max);

double calculateMean(vector<string> nodes);

#endif