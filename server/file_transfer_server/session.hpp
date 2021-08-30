#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include "internal_exception.hpp"
#include "protocol/request_header.hpp"
#include "protocol/response_header.hpp"

/*
* Takes care of a single session, once a client connects.
* Right after the request is taken care of and a response is send back to the client, the socket is closed.
*/
class Session
{
public:
	Session(boost::asio::ip::tcp::socket);
	void handle_request();

private:
	Protocol::InternalRequest parse_request();
	Protocol::InternalResponse execute_request(const Protocol::InternalRequest&);
	void write_response(Protocol::InternalResponse);

	boost::asio::ip::tcp::socket client_socket;
};
