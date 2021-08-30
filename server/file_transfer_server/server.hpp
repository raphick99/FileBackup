#pragma once

#include <cstdint>
#include <boost/asio.hpp>

class Server
{
public:
	Server(uint16_t);
	void serve_forever();

private:
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::acceptor acceptor;
};
