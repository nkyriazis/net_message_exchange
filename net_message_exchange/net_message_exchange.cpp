/********************************************************************
created:	2009/06/30
created:	30:6:2009   0:25
filename: 	c:\Current Projects\net_message_exchange\net_message_exchange\net_message_exchange.cpp
file path:	c:\Current Projects\net_message_exchange\net_message_exchange
file base:	net_message_exchange
file ext:	cpp
author:		Nick Kyriazis

purpose:	Implementation
*********************************************************************/

#include "net_message_exchange.h"
#include <boost/asio.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/export.hpp>
#include <iostream>
#include <sstream>
#include <fstream>

//	for xml serialization the next line should be commented
#define make_nvp(a,b) (b)

using namespace std;
using namespace boost;
using namespace archive;
using namespace serialization;
using namespace asio;

//////////////////////////////////////////////////////////////////////////

static io_service g_ioService;
static const char *g_port = "12345";

//////////////////////////////////////////////////////////////////////////

size_t port_from_string(const std::string &str)
{
	istringstream stream(str.c_str());
	size_t ret;
	stream >> ret;
	return ret;
}

//////////////////////////////////////////////////////////////////////////
//	Handle serialization of data

namespace boost
{
	namespace serialization
	{
		using namespace net_message_exchange;

		template < class Archive >
		void serialize(Archive &ar, message_base &msg, const size_t version)
		{
		}

		template < class Archive >
		void serialize(Archive &ar, message_name &msg, const size_t version)
		{
			ar
				& make_nvp("base", base_object<message_base>(msg))
				& make_nvp("name", msg.name)
				;
		}

		template < class Archive >
		void serialize(Archive &ar, message_time &msg, const size_t version)
		{
			ar
				& make_nvp("base", base_object<message_base>(msg))
				& make_nvp("seconds", msg.seconds)
				& make_nvp("minutes", msg.minutes)
				& make_nvp("hours", msg.hours)
				;
		}
	}
}

//	For each class direct instantiation of template
//	code and appropriate registration is required
BOOST_CLASS_EXPORT_GUID(net_message_exchange::message_name, "message_name")
BOOST_CLASS_EXPORT_GUID(net_message_exchange::message_time, "message_time")

namespace net_message_exchange
{

	//////////////////////////////////////////////////////////////////////////

	message_base::~message_base()
	{

	}

	//////////////////////////////////////////////////////////////////////////

	bool message_name::accept( message_visitor &vis )
	{
		return vis.visit(*this);
	}

	bool message_name::accept( message_const_visitor &vis ) const
	{
		return vis.visit(*this);
	}

	//////////////////////////////////////////////////////////////////////////

	bool message_time::accept( message_visitor &vis )
	{
		return vis.visit(*this);
	}

	bool message_time::accept( message_const_visitor &vis ) const
	{
		return vis.visit(*this);
	}

	//////////////////////////////////////////////////////////////////////////

	message_visitor::~message_visitor()
	{

	}

	message_const_visitor::~message_const_visitor()
	{

	}

	//////////////////////////////////////////////////////////////////////////

	struct message_stream_private_data_base :
		public message_stream::private_data,
		public ip::tcp::iostream
	{
		message_stream_private_data_base() {}
		message_stream_private_data_base(const string &server):
		ip::tcp::iostream(server, g_port) {}
	};

	struct server_private_data :
		public message_stream_private_data_base
	{
		server_private_data():
		endpoint(ip::tcp::v4(), port_from_string(g_port)),
		acceptor(g_ioService, endpoint)
		{
			acceptor.accept(*rdbuf());
		}

		ip::tcp::endpoint	endpoint;
		ip::tcp::acceptor	acceptor;
	};

	struct client_private_data :
		public message_stream_private_data_base
	{
		client_private_data(const string &server):
		message_stream_private_data_base(server)
		{
		}
	};

	//////////////////////////////////////////////////////////////////////////
	
	message_stream::private_data::~private_data()
	{

	}

	message_stream::message_stream()
	{
		m_privateData.reset(new server_private_data);
	}

	message_stream::message_stream( const string &server )
	{
		m_privateData.reset(new client_private_data(server));
	}

	void message_stream::send_message( const message_base::ptr &msg )
	{
		binary_oarchive ar(*dynamic_cast<message_stream_private_data_base*>(m_privateData.get()), no_header | no_codecvt);
		ar & make_nvp("message", msg);
	}

	message_base::ptr message_stream::receive_message()
	{
		binary_iarchive ar(*dynamic_cast<message_stream_private_data_base*>(m_privateData.get()), no_header | no_codecvt);
		message_base::ptr msg;
		ar & make_nvp("message", msg);
		return msg;
	}

}