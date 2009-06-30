// test_client.cpp : Defines the entry point for the console application.
//

#include "../net_message_exchange/net_message_exchange.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		net_message_exchange::message_stream stream(argv[1]);

		{
			net_message_exchange::message_name::ptr msg(new net_message_exchange::message_name);
			dynamic_cast<net_message_exchange::message_name&>(*msg).name = "zabarakatranemia\nileos\nileos";
			stream.send_message(msg);
		}

		{
			net_message_exchange::message_time::ptr msg(new net_message_exchange::message_time);
			dynamic_cast<net_message_exchange::message_time&>(*msg).seconds = 1;
			dynamic_cast<net_message_exchange::message_time&>(*msg).minutes = 2;
			dynamic_cast<net_message_exchange::message_time&>(*msg).hours = 3;
			stream.send_message(msg);
		}
	}
	catch(exception &e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

