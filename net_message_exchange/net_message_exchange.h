/********************************************************************
	created:	2009/06/30
	created:	30:6:2009   0:22
	filename: 	c:\Current Projects\net_message_exchange\net_message_exchange\net_message_exchange.h
	file path:	c:\Current Projects\net_message_exchange\net_message_exchange
	file base:	net_message_exchange
	file ext:	h
	author:		Nick Kyriazis
	
	purpose:	Setup a basic message exchange scheme over a net stream
*********************************************************************/

#pragma once

#if defined(_WIN32) && defined(DLL)
#	if defined(net_message_exchange_EXPORTS)
#		define DLLAPI __declspec(dllexport)
#	else
#		define DLLAPI __declspec(dllimport)
#	endif
#else
#	define DLLAPI
#endif

#include <string>
#include <boost/shared_ptr.hpp>

namespace net_message_exchange
{
	//	Type definitions
	typedef std::string string;

	//	Forward declarations
	struct message_base;
	struct message_name;
	struct message_time;

	struct message_visitor;
	struct message_const_visitor;

	//////////////////////////////////////////////////////////////////////////

	struct DLLAPI message_base
	{
		typedef boost::shared_ptr<message_base> ptr;

		virtual			~message_base	();
		virtual bool	accept			(message_visitor &vis) =0;
		virtual bool	accept			(message_const_visitor &vis) const =0;
	};

	//////////////////////////////////////////////////////////////////////////

	struct DLLAPI message_name :
		public message_base
	{
		string	name;

		bool	accept			(message_visitor &vis);
		bool	accept			(message_const_visitor &vis) const;
	};

	//////////////////////////////////////////////////////////////////////////

	struct DLLAPI message_time :
		public message_base
	{
		size_t	seconds,
				minutes,
				hours;

		bool	accept			(message_visitor &vis);
		bool	accept			(message_const_visitor &vis) const;
	};

	//////////////////////////////////////////////////////////////////////////

	struct DLLAPI message_visitor
	{
		virtual ~message_visitor();

		virtual bool visit(message_name &msg) =0;
		virtual bool visit(message_time &msg) =0;
	};

	//////////////////////////////////////////////////////////////////////////

	struct DLLAPI message_const_visitor
	{
		virtual ~message_const_visitor();

		virtual bool visit(const message_name &msg) =0;
		virtual bool visit(const message_time &msg) =0;
	};

	//////////////////////////////////////////////////////////////////////////

	struct DLLAPI message_stream
	{
							message_stream	();
							message_stream	(const string &server);

		void				send_message	(const message_base::ptr &msg);
		message_base::ptr	receive_message	();

		//////////////////////////////////////////////////////////////////////////
		struct DLLAPI private_data
		{
			typedef boost::shared_ptr<private_data> ptr;
			virtual ~private_data();
		};
		//////////////////////////////////////////////////////////////////////////
	private:
		private_data::ptr	m_privateData;
	};

}	//	namespace net_message_exchange