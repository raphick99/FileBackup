#include <iostream>
#include <boost/asio.hpp>
#include "config.hpp"
#include "server.hpp"

int main()
{
	try
	{
		std::cout << "starting...\n";
		Server server(Config::port);
		server.serve_forever();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception in thread: " << e.what() << "\n";
	}
}

