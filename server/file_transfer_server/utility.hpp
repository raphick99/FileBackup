#pragma once

#include <string>
#include <boost/asio.hpp>

class Utility
{
public:
	template<typename T>
	static std::string read_prefixed_string(boost::asio::ip::tcp::socket& socket)
	{
		T buffer_size = read_struct<T>(socket);

		std::string buffer;
		buffer.resize(buffer_size);

		std::size_t reply_length = boost::asio::read(socket, boost::asio::buffer(buffer.data(), buffer_size));
		if (reply_length == 0)
		{
			throw InternalException(InternalStatus::Utility_SocketClosedAtOtherEndpoint);
		}
		if (reply_length != buffer_size)
		{
			throw InternalException(InternalStatus::Utility_IncompleteRead);
		}
		return buffer;
	}

	template<typename T>
	static T read_struct(boost::asio::ip::tcp::socket& socket)
	{
		T t{};
		std::size_t reply_length = boost::asio::read(socket, boost::asio::buffer(&t, sizeof(t)));
		if (reply_length == 0)
		{
			throw InternalException(InternalStatus::Utility_SocketClosedAtOtherEndpoint);
		}
		if (reply_length != sizeof(t))
		{
			throw InternalException(InternalStatus::Utility_IncompleteRead);
		}
		return t;
	}

	template<typename T>
	static void write_prefixed_string(boost::asio::ip::tcp::socket& socket, const std::string& string_to_write)
	{
		write_struct<T>(socket, static_cast<T>(string_to_write.size()));
		std::size_t write_length = boost::asio::write(socket, boost::asio::buffer(string_to_write.data(), string_to_write.size()));
		if (write_length != string_to_write.size())
		{
			throw InternalException(InternalStatus::Utility_IncompleteWrite);
		}
	}

	template<typename T>
	static void write_struct(boost::asio::ip::tcp::socket& socket, const T& t)
	{
		std::size_t write_length = boost::asio::write(socket, boost::asio::buffer(&t, sizeof(t)));
		if (write_length != sizeof(t))
		{
			throw InternalException(InternalStatus::Utility_IncompleteWrite);
		}
	}
};
