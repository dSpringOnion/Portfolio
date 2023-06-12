#include <random>
#include <iostream>
#include "structs.h"
#include "mailboxhelpers.h"
#include "printhelpers.h"
#include <chrono>
#include <cstring>
#include <mutex>
#include <algorithm>
#include <queue>
#include "rand_functs.h"
#include <thread>

/*
    Project 3
    Group: 6
    Members: Nicholas Baird, John Leidy, Daniel Park, Zane Smith
*/

using namespace std;
// Location -> mailboxhelpers.cpp
// START_STRING is used to identify that we are sending a message for the first time.
// This means we are not forwarding from the recv function.
string START_STRING = "start";
// Location -> mailboxhelpers.cpp
// RECV_STRING is used to identify that we are NOT sending a messsage for the first time.
// This means we are forwarding from the recv function.
string RECV_STRING = "recv";
// Location -> mailboxhelpers.cpp
// generates a random recipient from all of the mailboxes
// this is only to be used on the first send
string generateRecipient(nodes all_nodes, uint16_t mid)
{
    string current = to_string(mid);
    string recipient = current;
    while (recipient == current)
    {
        recipient = all_nodes.all_nodes[(int)rand_range(0.0, (double)all_nodes.all_nodes.size())].node_id;
    }
    return recipient;
}

string generateNeighborRecipientDeptOne(nodes all_nodes, string previous_sender, uint16_t mid, string final_recip)
{
    neighborOfNeighbor non;
    uint16_t recipient = mid;
    if (all_nodes.all_nodes[mid].neighbors.size() <= 1)
    {
        recipient = stoi(all_nodes.all_nodes[mid].neighbors[0]);
    }
    else if (all_nodes.all_nodes[mid].neighbors.size() == 2)
    {
        while (recipient == mid || recipient == stoi(previous_sender))
        {
            recipient = stoi(all_nodes.all_nodes[mid].neighbors[rand_unit()]);
        }
    }
    else if (all_nodes.all_nodes[mid].neighbors.size() > 2)
    {
        while (recipient == mid || recipient == stoi(previous_sender))
        {
            recipient = stoi(all_nodes.all_nodes[mid].neighbors[(int)rand_range(0.0, (double)all_nodes.all_nodes[mid].neighbors.size())]);
        }
    }

    return to_string(recipient);
}

// Location -> mailboxhelpers.cpp
// This parses the message we are sent and returns the string before the colon in the message.
// Return value will be either "start" or "recv"
// In mbox send this allows us to know whether a message is already in a mailbox or is a new message.
string getStartOrReceive(char *content)
{
    int parameter_counter = 0;
    string start_recv;
    for (int i = 0; content[i] != '\0'; i++)
    {
        if (content[i] == ':')
        {
            parameter_counter++;
        }
        else if (content[i] != ':' && parameter_counter == 0)
        {
            start_recv += content[i];
        }
    }
    return start_recv;
}
// Location -> mailboxhelpers.cpp
// This function gets the content that is after the start or receive identifier in the message.
// We return everything after including the colon.
string getAfterStartOrReceive(char *content)
{
    int parameter_counter = 0;
    string all_after;
    for (int i = 0; content[i] != '\0'; i++)
    {
        if (content[i] == ':' && parameter_counter == 0)
        {
            all_after += ':';
            parameter_counter++;
        }
        else if (parameter_counter > 0)
        {
            all_after += content[i];
        }
    }
    return all_after;
}
// Location -> mailboxhelpers.cpp
// This will change the content before the first colon to know this message is being forwarded
char *changeSendToReceive(char *content)
{
    string all_after = getAfterStartOrReceive(content);
    string new_content;
    new_content += RECV_STRING;
    new_content += all_after;
    const int len = new_content.length();
    char *char_arr = new char[len + 1];
    strcpy(char_arr, new_content.c_str());
    return char_arr;
}
