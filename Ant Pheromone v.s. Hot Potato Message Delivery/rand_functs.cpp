#include "rand_functs.h"
#include <random>
#include <mutex>
#include <iostream>
#include <string>

using namespace std;

random_device rand_d;
mt19937 gen(rand_d());
uniform_real_distribution<double> dis(0, 1);

random_device o_l;
mt19937 o_l_gen(o_l());
uniform_int_distribution<int> dist(0, 1);

mutex rand_lock;
// Location -> rand_functs.cpp
// This rand_unit will be used for rand_exp and rand_range
// All threads will share the same random space
double rand_zero_one_range()
{
    return dis(gen);
}

int rand_unit()
{
    return dist(o_l_gen);
}
// Location -> rand_functs.cpp
double rand_exponential(double mean)
{
    rand_lock.lock();
    double out = -mean * log(rand_zero_one_range());
    rand_lock.unlock();
    return out;
}
// Location -> rand_functs.cpp
double rand_range(double min, double max)
{
    rand_lock.lock();
    double out = rand_zero_one_range() * (max - min) + min;
    rand_lock.unlock();
    return out;
}
// Location -> rand_functs.cpp
double calculateMean(vector<string> nodes)
{
    double total = 0.0;
    int nodes_size = nodes.size();
    for (int i = 0; i < nodes_size; i++)
    {
        total += stoi(nodes[i]);
    }
    return total / nodes_size;
}