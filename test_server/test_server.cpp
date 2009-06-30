// test_server.cpp : Defines the entry point for the console application.
//

#include "../net_message_exchange/net_message_exchange.h"
#include <iostream>
#include <boost/asio.hpp>

using namespace std;

struct message_dispatcher :
	public net_message_exchange::message_const_visitor
{
	bool visit(const net_message_exchange::message_name &msg)
	{
		cout << "Received : " << msg.name << endl;
		return true;
	}
	bool visit(const net_message_exchange::message_time &msg)
	{
		cout << "Received : " << msg.hours << " : " << msg.minutes << " : " << msg.seconds << endl;
		return true;
	}
};

int main(int argc, char* argv[])
{
	try
	{
		net_message_exchange::message_stream stream;
		message_dispatcher visitor;
		stream.receive_message()->accept(visitor);
		stream.receive_message()->accept(visitor);
// 		boost::asio::io_service io;
// 		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 12345);
// 		boost::asio::ip::tcp::acceptor acceptor(io, endpoint);
// 		boost::asio::ip::tcp::iostream stream;
// 		acceptor.accept(*stream.rdbuf());
// 
// 		string line;
// 		while(!getline(stream, line).eof())
// 		{
// 			cout << line << endl;
// 		}
	}
	catch(exception &e)
	{
		cout << e.what() << endl;
	}
	return 0;
}

