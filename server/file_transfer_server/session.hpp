#pragma once

#include <iostream>
#include <boost/asio.hpp>
#include "internal_exception.hpp"
#include "protocol/request_header.hpp"
#include "protocol/response_header.hpp"

class Session
{
public:
	Session(boost::asio::ip::tcp::socket);
	static void run(boost::asio::ip::tcp::socket);

private:
	void handle_request();
	Protocol::InternalRequest parse_request();
	Protocol::InternalResponse execute_request(const Protocol::InternalRequest&);
	void write_response(Protocol::InternalResponse);

	template<typename T>
	std::string read_prefixed_string()
	{
		T buffer_size = read_struct<T>();

		std::string buffer;
		buffer.resize(buffer_size);

		boost::system::error_code error_code;
		std::size_t reply_length = boost::asio::read(client_socket, boost::asio::buffer(buffer.data(), buffer_size), error_code);
		if (reply_length == 0)
		{
			throw InternalException(InternalStatus::Session_SocketClosedAtOtherEndpoint);
		}
		if (reply_length != buffer_size)
		{
			throw InternalException(InternalStatus::Session_IncompleteRead);
		}
		if (error_code && error_code == boost::asio::error::eof)
		{
			throw InternalException(InternalStatus::Session_EofReached);
		}
		return buffer;
	}

	template<typename T>
	T read_struct()
	{
		T t{};
		boost::system::error_code error_code;
		std::size_t reply_length = boost::asio::read(client_socket, boost::asio::buffer(&t, sizeof(t)), error_code);
		if (reply_length == 0)
		{
			throw InternalException(InternalStatus::Session_SocketClosedAtOtherEndpoint);
		}
		if (reply_length != sizeof(t))
		{
			throw InternalException(InternalStatus::Session_IncompleteRead);
		}
		if (error_code && error_code == boost::asio::error::eof)
		{
			throw InternalException(InternalStatus::Session_EofReached);
		}
		return t;
	}

	template<typename T>
	void write_prefixed_string(std::string string_to_write)
	{
		write_struct<T>(static_cast<T>(string_to_write.size()));
		std::size_t write_length = boost::asio::write(client_socket, boost::asio::buffer(string_to_write.data(), string_to_write.size()));
		if (write_length != string_to_write.size())
		{
			// TODO throw exception;
		}
	}

	template<typename T>
	void write_struct(const T& t)
	{
		std::size_t write_length = boost::asio::write(client_socket, boost::asio::buffer(&t, sizeof(t)));
		if (write_length != sizeof(t))
		{
			// TODO throw exception;
		}
	}

	boost::asio::ip::tcp::socket client_socket;
};
