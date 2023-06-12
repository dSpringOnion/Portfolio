#ifndef PRINT_HELPERS
#define PRINT_HELPERS
#include <string>

using namespace std;

string topSeparatorGen(string title, char fill_char);

string bottomSeparatorGen(char fill_char);

void printerSendReceive(string correct_forward_send_receive, uint16_t current_mid, string next_recipient, string recipient_final, char *message, int hop_count, double travel_time_seconds);

#endif