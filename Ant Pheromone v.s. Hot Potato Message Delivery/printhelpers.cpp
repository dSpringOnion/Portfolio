#include "printhelpers.h"
#include <iostream>

using namespace std;

int TOTAL_WIDTH = 80;

char INTENSE_CHARACTER = '*';

string TRIPPLE_NEW_LINE = "\n\n\n";

void printerSendReceive(string correct_forward_send_receive, uint16_t current_mid, string next_recipient, string recipient_final, char *message, int hop_count, double travel_time_seconds)
{

    if (correct_forward_send_receive == "recv_correct")
    {
        cout << topSeparatorGen("CORRECT RECIPIENT", '*');
        cout << "CURRENT MID: " << current_mid << endl;
        cout << "MID STORED IN ITEM: " << recipient_final << endl;
        cout << "MESSAGE: " << message << endl;
        cout << "HOP COUNT: " << hop_count << endl;
        cout << "TIME SPENT: " << travel_time_seconds << endl;
        cout << bottomSeparatorGen('*');
    }
    else if (correct_forward_send_receive == "recv_forward")
    {
        cout << topSeparatorGen("In MailBox::recv || FORWARDING", '_');
        cout << "MID FORWARDING: " << current_mid << endl;
        cout << "INTENDED RECIP: " << recipient_final << endl;
        cout << "MESSAGE: " << message << endl;
        cout << bottomSeparatorGen('_');
    }
    else if (correct_forward_send_receive == "send_first" || correct_forward_send_receive == "send_forward")
    {
        string title;
        if (correct_forward_send_receive == "send_first")
        {
            title = "MESSAGE SENT (FIRST)";
        }
        else if (correct_forward_send_receive == "send_forward")
        {
            title = "MESSAGE SENT (FORWARD)";
        }
        cout << topSeparatorGen(title, '_');
        cout << "MID SENDING: " << current_mid << endl;
        cout << "FINAL RECIP: " << recipient_final << endl;
        cout << "RECIP FOR THIS SEND: " << next_recipient << endl;
        cout << "MESSAGE: " << message << endl;
        cout << bottomSeparatorGen('_');
    }
}

string topSeparatorGen(string title, char fill_char)
{
    int text_width = title.length();
    int filler_width = TOTAL_WIDTH - text_width;
    int left_width = filler_width / 2;
    int right_width = filler_width - left_width;
    string left_fill(left_width, fill_char);
    string right_fill(right_width, fill_char);
    string top_separator = TRIPPLE_NEW_LINE + left_fill + title + right_fill + TRIPPLE_NEW_LINE;
    return top_separator;
}

string bottomSeparatorGen(char fill_char)
{

    string bottom_separator(TOTAL_WIDTH, fill_char);
    return TRIPPLE_NEW_LINE + bottom_separator + TRIPPLE_NEW_LINE;
}