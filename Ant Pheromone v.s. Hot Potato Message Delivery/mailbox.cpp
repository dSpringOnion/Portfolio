/*
 * mailbox.cc
 * Copyright(c) 2022 Dongsoo S. Kim
 */
#include <iostream>
#include <string.h>
#include <map>
#include <queue>
#include <mutex>
#include "mbox.h"
#include <thread>
#include <chrono>
#include <random>
#include "structs.h"
#include "loader.h"
#include "mailboxhelpers.h"
#include "table.h"
#include "constants.h"
#include "anthelpers.h"
#include "printhelpers.h"
#include "rand_functs.h"

class MailBox
{
	struct item
	{
		int length;
		char *content;
		int hop_count;
		std::string recipient_final;
		std::string recipient_next;
		std::string sender;
		std::string previous_sender;
		std::chrono::time_point<std::chrono::system_clock> start;
		std::chrono::time_point<std::chrono::system_clock> end;
		double pheromone_level;
		std::queue<int> path;
		std::queue<double> seconds;
	};
	typedef std::queue<item> mailbox_t;
	std::map<uint16_t, mailbox_t> _mailboxes;
	std::mutex mtx;

public:
	MailBox(){};
	~MailBox();
	bool empty(uint16_t mid);
	int send(uint16_t mid, const void *packet, int len);
	int recv(uint16_t mid, void *packet, int max);
};

static MailBox mailbox;

MailBox::~MailBox()
{
	struct item xtem;
	mtx.lock();
	for (auto x : _mailboxes)
	{
		while (!x.second.empty())
		{
			xtem = x.second.front();
			x.second.pop();
			delete[] xtem.content;
		}
	}
	mtx.unlock();
}

// Location -> mailbox.cpp
// Returns true if the queue is empty, false if it has items left in it.
bool MailBox::empty(uint16_t mid)
{
	return mailbox._mailboxes[mid].empty();
}

bool mbox_empty(uint16_t mid)
{
	return mailbox.empty(mid);
}

bool mbox_avail(uint16_t mid)
{
	return true;
}
// Location -> mailbox.cpp
double durationCalc(chrono::time_point<chrono::system_clock> start, chrono::time_point<chrono::system_clock> end)
{
	auto current_travel_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	double seconds_passing = static_cast<double>(current_travel_time.count()) / 1000000;
	return seconds_passing;
}

// Location -> mailbox.cpp
// This sends a message.
// The message structure is defined in the MailBox class.
int MailBox::send(uint16_t mid, const void *packet, int len)
{
	mailbox.mtx.lock();
	int bytes = 0;
	const char *char_ptr_convert = reinterpret_cast<const char *>(packet);
	char *temp = const_cast<char *>(char_ptr_convert);
	std::string start_or_recv = getStartOrReceive(temp);
	// if it is the first time sending the message
	if (getStartOrReceive(temp) == START_STRING)
	{
		std::string recipient_normal_final = generateRecipient(all_nodes, mid);
		std::string recipient_normal_for_this_send = generateRecipient(all_nodes, mid);
		temp = changeSendToReceive(temp);
		item new_message;
		new_message.content = temp;
		new_message.start = std::chrono::system_clock::now();
		new_message.hop_count = 1;
		new_message.length = len;
		new_message.sender = to_string(mid);
		new_message.previous_sender = to_string(mid);
		new_message.recipient_final = recipient_normal_final;
		new_message.recipient_next = recipient_normal_for_this_send;
		bytes = sizeof(new_message);
		if (HOT_OR_ANT == "ant")
		{
			new_message.pheromone_level = STARTING_ANT_PHEROMONE;
			new_message.path.push(mid);
		}
		mailbox._mailboxes[stoi(new_message.recipient_next)].emplace(new_message);
		printerSendReceive("send_first", mid, new_message.recipient_next, new_message.recipient_final, new_message.content, new_message.hop_count, 0);
		all_data.messages_sent++;
	}
	// if the message has recv in it instead of start
	else if (getStartOrReceive(temp) == RECV_STRING)
	{
		item forw_msg = mailbox._mailboxes[mid].front();
		std::string recip;
		if (HOT_OR_ANT == "hot")
		{
			forw_msg.previous_sender = to_string(mid);
			recip = generateNeighborRecipientDeptOne(all_nodes, mailbox._mailboxes[mid].front().sender, mid, forw_msg.recipient_final);
			mailbox._mailboxes[mid].pop();
			forw_msg.sender = to_string(mid);
		}
		else if (HOT_OR_ANT == "ant")
		{
			forw_msg.path.push(mid);
			forw_msg.seconds.push(durationCalc(forw_msg.start, std::chrono::system_clock::now()));
			incremPheromone(mid, forw_msg.previous_sender, forw_msg.start);
			// check the time.. if over 10s reset
			auto time_spent_passing = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - forw_msg.start);
			double time_seconds = static_cast<double>(time_spent_passing.count()) / 1000000;
			if (time_seconds > 10)
			{
				forw_msg.pheromone_level = STARTING_ANT_PHEROMONE;
			}
			recip = to_string(chooseAntRecipBASIC(mid, forw_msg.previous_sender, forw_msg.recipient_final));
			forw_msg.previous_sender = to_string(mid);
			mailbox._mailboxes[mid].pop();
			forw_msg.sender = to_string(mid);
		}

		forw_msg.recipient_next = recip;
		forw_msg.hop_count++;
		bytes = sizeof(forw_msg);
		mailbox._mailboxes[stoi(recip)].emplace(forw_msg);
		printerSendReceive("send_forward", mid, forw_msg.recipient_next, forw_msg.recipient_final, forw_msg.content, forw_msg.hop_count, 0);
	}
	std::cout << "Messages in circulation: " << all_data.messages_sent - all_data.messages_received << endl;

	mailbox.mtx.unlock();
	return bytes;
}

int mbox_send(uint16_t mid, const void *packet, int len)
{
	return mailbox.send(mid, packet, len);
}

// Location -> mailbox.cpp
// Handles the recv function for the mailbox id it is sent.
// Receives and pops if correct recipient. If not send is called and it is forwarded
int MailBox::recv(uint16_t mid, void *packet, int max)
{
	bool is_empty = false;
	int bytes = 0;
	mailbox.mtx.lock();
	is_empty = mailbox._mailboxes[mid].empty();
	mailbox.mtx.unlock();

	if (!is_empty)
	{
		// first check if we are the final recipient. if we are then we receive it... if not forward
		//  we need to take this and forward it to a neighbor
		mailbox.mtx.lock();
		int intended_recip = stoi(mailbox._mailboxes[mid].front().recipient_final);
		cout << "intended recip in recv: " << intended_recip << endl;
		cout << "mid here: " << mid << endl;
		mailbox.mtx.unlock();
		// we are the final recipient
		if (intended_recip == mid)
		{
			mailbox.mtx.lock();
			item done = mailbox._mailboxes[mid].front();
			if (HOT_OR_ANT == "ant")
			{
				done.path.push(mid);
				done.seconds.push(durationCalc(done.start, std::chrono::system_clock::now()));
			}
			bytes = sizeof(done);
			done.end = std::chrono::system_clock::now();
			auto time_spent_passing = std::chrono::duration_cast<std::chrono::milliseconds>(done.end - done.start);
			double time_seconds = static_cast<double>(time_spent_passing.count()) / 1000;
			mailbox._mailboxes[mid].pop();
			printerSendReceive("recv_correct", mid, "none", done.recipient_final, done.content, done.hop_count, time_seconds);
			all_data.hop_counts.push_back(done.hop_count);
			all_data.travel_times.push_back(time_seconds);
			all_data.messages_received++;
			mailbox.mtx.unlock();
		}
		// we are not the final recipient
		else if (intended_recip != mid)
		{
			mailbox.mtx.lock();
			item not_done = mailbox._mailboxes[mid].front();
			bytes = sizeof(not_done);
			printerSendReceive("recv_forward", mid, "none", not_done.recipient_final, not_done.content, 0, 0);
			// phero_table[mid][stoi(not_done.previous_sender)] += pheromone_incremental_constant;
			mailbox.mtx.unlock();
			bytes = mailbox.send(mid, not_done.content, not_done.length);
		}
	}
	// if the mailbox is empty for the given mid we know that we are coming from our thread sending out a new mailbox
	else if (is_empty)
	{
		// show that it is empty
		mailbox.mtx.lock();
		string mid_out = to_string(mid);
		if (mid_out.length() == 1)
		{
			std::cout << "Inside MailBox::recv  MID recv: " << mid << "  || MAILBOX EMPTY" << std::endl;
		}
		else if (mid_out.length() != 1)
		{
			std::cout << "Inside MailBox::recv  MID recv: " << mid << " || MAILBOX EMPTY" << std::endl;
		}
		mailbox.mtx.unlock();
	}
	return bytes;
}

int mbox_recv(uint16_t mid, void *packet, int max)
{
	return mailbox.recv(mid, packet, max);
}
