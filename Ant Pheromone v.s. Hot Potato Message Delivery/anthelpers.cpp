#include <iostream>
#include <chrono>
#include <vector>
#include "constants.h"
#include "structs.h"
#include "table.h"
#include <math.h>
#include "mailboxhelpers.h"
#include <queue>
#include "rand_functs.h"
#include <thread>

using namespace std;

// remove current_phero_value... it is not needed
double getNewPheroValue(chrono::time_point<chrono::system_clock> start, double current_phero_value)
{
    chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
    auto current_travel_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    double seconds_passing = static_cast<double>(current_travel_time.count()) / 1000;
    double exp_inv = -DILUTION_HALF_LIFE;
    double exponent_value = exp_inv * seconds_passing;
    double e_to_power = exp(exponent_value);
    double new_phero = STARTING_ANT_PHEROMONE * e_to_power;
    return new_phero;
}

double getSecondsTraveled(chrono::time_point<chrono::system_clock> start)
{
    chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
    auto current_travel_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    double seconds_passing = static_cast<double>(current_travel_time.count()) / 1000;
    return seconds_passing;
}

double getNewPheroReverse(double seconds_traveled, chrono::time_point<chrono::system_clock> start)
{
    chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
    auto current_travel_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double seconds_passing = static_cast<double>(current_travel_time.count() - 1000) / 1000000;
    seconds_passing = seconds_passing - seconds_traveled;
    double exp_inv = -DILUTION_HALF_LIFE;
    double exponent_value = exp_inv * seconds_passing;
    double e_to_power = exp(exponent_value);
    double new_phero = STARTING_ANT_PHEROMONE * e_to_power;
    return new_phero;
}

double calcProb(uint16_t mid, int neighbor_node_id, int neighbors_size)
{
    double probability = 0.0;
    double top = pow(phero_table[mid][neighbor_node_id], POWER_COEFFICIENT);
    double bottom;
    for (int i = 0; i < neighbors_size; i++)
    {
        double value = pow(phero_table[mid][stoi(all_nodes.all_nodes[mid].neighbors[i])], POWER_COEFFICIENT);
        bottom += value;
    }
    probability = top / bottom;
    return probability;
}

vector<double> calculateProbabilities(uint16_t mid)
{
    double test = 0.0;
    vector<double> out;
    int neighbors_size = all_nodes.all_nodes[mid].neighbors.size();
    for (int i = 0; i < neighbors_size; i++)
    {
        out.push_back(calcProb(mid, stoi(all_nodes.all_nodes[mid].neighbors[i]), neighbors_size));
    }
    for (int i = 0; i < out.size(); i++)
    {
        test += out[i];
    }
    cout << getNeighborsString(all_nodes.all_nodes[mid]) << endl;
    return out;
}

bool allProbsEqual(vector<double> probs_vec)
{
    bool out = true;
    double first = probs_vec[0];
    for (int i = 0; i < probs_vec.size(); i++)
    {
        if (probs_vec[i] != first)
        {
            out = false;
        }
    }
    return out;
}

double sumProbs(vector<double> probs)
{
    double out = 0.0;
    for (int i = 0; i < probs.size(); i++)
    {
        out += probs[i];
    }
    return out;
}

void incremPheromone(uint16_t mid_we_are_at, string previous_sender, chrono::time_point<chrono::system_clock> start)
{
    auto end = std::chrono::system_clock::now();
    auto current_travel_time = chrono::duration_cast<std::chrono::microseconds>(end - start);
    double seconds_passing = static_cast<double>(current_travel_time.count()) / 1000000;
    double exp_inv = -DILUTION_HALF_LIFE;
    double exponent_value = exp_inv * seconds_passing;
    double e_to_power = exp(exponent_value);
    double new_phero = STARTING_ANT_PHEROMONE * e_to_power;
    phero_table[stoi(previous_sender)][mid_we_are_at] += new_phero;
}

int chooseAntRecipBASIC(uint16_t mid, string previous_sender, string final_recip)
{
    int recip = mid;
    vector<double> probs = calculateProbabilities(mid);
    cout << "Probs: ";
    for (int i = 0; i < probs.size(); i++)
    {
        cout << probs[i] << " ";
    }
    cout << endl;
    bool equal = allProbsEqual(probs);
    if (equal)
    {
        cout << "All equal" << endl;
        recip = stoi(generateNeighborRecipientDeptOne(all_nodes, previous_sender, mid, final_recip));
        cout << "Recip" << endl;
    }
    else if (!equal)
    {
        if (all_nodes.all_nodes[mid].neighbors.size() == 1)
        {
            recip = stoi(generateNeighborRecipientDeptOne(all_nodes, previous_sender, mid, final_recip));
        }
        else if (all_nodes.all_nodes[mid].neighbors.size() > 1)
        {

            while (recip == mid || recip == stoi(previous_sender))
            {
                double cumulative_prob = 0.0;
                double random_value_for_prob = rand_zero_one_range();
                cout << "Current mid: " << mid << " Previous: " << previous_sender << " Prob value we need to reach: " << random_value_for_prob << endl;
                for (int i = 0; i < probs.size(); i++)
                {
                    cumulative_prob += probs[i];
                    if (cumulative_prob >= random_value_for_prob)
                    {
                        recip = stoi(all_nodes.all_nodes[mid].neighbors[i]);
                        break;
                    }
                }
            }
        }
    }
    return recip;
}
