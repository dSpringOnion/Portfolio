#ifndef MBOXHELP
#define MBOXHELP

#include "structs.h"
#include <vector>
using namespace std;

extern string START_STRING;
extern string RECV_STRING;

struct neighborOfNeighbor
{
    bool found;
    string first_neighbor;
    string second_neighbor;
};

extern vector<string> curr_path;

extern vector<bool> visited_vec;

string generateRecipient(nodes all_nodes, uint16_t mid);

string generateNeighborRecipientDeptOne(nodes all_nodes, string previous_sender, uint16_t mid, string final_recip);

string getStartOrReceive(char *content);

string getAfterStartOrReceive(char *content);

string topSeparatorGen(string title, char fill_char);

string bottomSeparatorGen(char fill_char);

char *changeSendToReceive(char *content);

vector<string> breadthFirst(nodes all_nodes, uint16_t mid, string intended_recip);

void printerSendReceive(std::string correct_forward_send_receive, uint16_t current_mid, std::string next_recipient, std::string recipient_final, char *message, int hop_count, double travel_time_seconds);

#endif