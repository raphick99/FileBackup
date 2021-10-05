#include <thread>
#include "server.hpp"
#include "session.hpp"

Server::Server(uint16_t port) : 
	io_context{},
	acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{}

void Server::serve_forever()
{
	while (1)
	{
		std::thread(&Session::handle_request, Session{ acceptor.accept() }).detach();
	}
}
