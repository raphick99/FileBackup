#include <iostream>
#include <exception>
#include <optional>
#include <string>
#include <cstdint>
#include "session.hpp"
#include "handler.hpp"

Session::Session(boost::asio::ip::tcp::socket client_socket_) :
	client_socket(std::move(client_socket_))
{}

void Session::run(boost::asio::ip::tcp::socket client_socket_)
{
	Session current_session{ std::move(client_socket_) };
	std::cout << "client connected!\n";
	while (1)
	{
		try
		{
			current_session.handle_request();
		}
		catch (const InternalException& e)
		{
			if (e.status == InternalStatus::Session_SocketClosedAtOtherEndpoint)
			{
				std::cout << "Client Closed Connection! killing thread...\n";
			}
			else
			{
				std::cout << "internal exception thrown\nstatus: " << static_cast<std::underlying_type_t<InternalStatus>>(e.status) << "\n";
			}
			return;
		}
	}
}

void Session::handle_request()
{
	auto internal_request = parse_request();
	Protocol::InternalResponse internal_response = execute_request(internal_request);
	write_response(internal_response);
}

Protocol::InternalRequest Session::parse_request()
{
	Protocol::InternalRequest internal_request{};

	internal_request.request_header = read_struct<Protocol::RequestHeader>();
	
	switch (internal_request.request_header.op)
	{
	case Protocol::Op::BackupFile:
		internal_request.filename = read_prefixed_string<uint16_t>();
		internal_request.payload = read_prefixed_string<uint32_t>();
		break;

	case Protocol::Op::RecoverFile:
	case Protocol::Op::DeleteBackedUpFile:
		internal_request.filename = read_prefixed_string<uint16_t>();
		break;

	case Protocol::Op::ListFiles:
		break;

	default:
		throw InternalException(InternalStatus::Session_UnrecognizedOpCode);
		break;
	}
	return internal_request;
}

Protocol::InternalResponse Session::execute_request(const Protocol::InternalRequest& internal_request)
{
	Protocol::InternalResponse internal_response{};
	switch (internal_request.request_header.op)
	{
	case Protocol::Op::BackupFile:
		internal_response = Handler::BackupFile(internal_request);
		break;

	case Protocol::Op::RecoverFile:
		internal_response = Handler::RecoverFile(internal_request);
		break;

	case Protocol::Op::DeleteBackedUpFile:
		internal_response = Handler::DeleteBackedUpFile(internal_request);
		break;

	case Protocol::Op::ListFiles:
		internal_response = Handler::ListFiles(internal_request);
		break;

	default:
		// shouldnt get there
		throw InternalException(InternalStatus::Session_UnrecognizedOpCode);
		break;
	}
	return internal_response;
}

void Session::write_response(Protocol::InternalResponse internal_response)
{
	write_struct(internal_response.response_header);
	if (internal_response.filename)
	{
		write_prefixed_string<uint16_t>(internal_response.filename.value());
	}
	if (internal_response.payload)
	{
		write_prefixed_string<uint32_t>(internal_response.payload.value());
	}
}
