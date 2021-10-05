#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "config.hpp"
#include "handler.hpp"
#include "internal_exception.hpp"
#include "protocol/status.hpp"

Protocol::InternalResponse Handler::BackupFile(const Protocol::InternalRequest& internal_request)
{
	Protocol::InternalResponse internal_response;
	internal_response.response_header.version = Config::version;

	if ((!internal_request.filename.has_value()) || !internal_request.payload.has_value())
	{
		internal_response.response_header.status = Protocol::Status::FailureGeneralError;
		return internal_response;
	}

	auto user_dir = get_user_directory_path(internal_request.request_header.user_id);
	auto file_path = user_dir / internal_request.filename.value();

	if (!std::filesystem::exists(user_dir))
	{
		std::filesystem::create_directory(user_dir);
	}
	else if (std::filesystem::exists(file_path))
	{
		std::filesystem::remove(file_path);
	}

	std::ofstream file(file_path);
	file << internal_request.payload.value();
	internal_response.response_header.status = Protocol::Status::SuccessFileBackedUpOrDeleted;
	internal_response.filename = internal_request.filename.value();
	return internal_response;
}

Protocol::InternalResponse Handler::RecoverFile(const Protocol::InternalRequest& internal_request)
{
	Protocol::InternalResponse internal_response;
	internal_response.response_header.version = Config::version;

	if (!internal_request.filename.has_value())
	{
		internal_response.response_header.status = Protocol::Status::FailureGeneralError;
		return internal_response;
	}

	auto user_dir = get_user_directory_path(internal_request.request_header.user_id);
	auto file_path = user_dir / internal_request.filename.value();

	if (!std::filesystem::exists(file_path))
	{
		internal_response.response_header.status = Protocol::Status::FailureFileNotFound;
		internal_response.filename = internal_request.filename.value();
		return internal_response;
	}

	internal_response.response_header.status = Protocol::Status::SuccessFileRecovered;
	internal_response.filename = internal_request.filename.value();

	internal_response.payload = read_whole_file(file_path);
	return internal_response;
}

Protocol::InternalResponse Handler::DeleteBackedUpFile(const Protocol::InternalRequest& internal_request)
{
	Protocol::InternalResponse internal_response;
	internal_response.response_header.version = Config::version;

	if (!internal_request.filename.has_value())
	{
		internal_response.response_header.status = Protocol::Status::FailureGeneralError;
		return internal_response;
	}

	auto user_dir = get_user_directory_path(internal_request.request_header.user_id);
	auto file_path = user_dir / internal_request.filename.value();

	if (!std::filesystem::exists(file_path))
	{
		internal_response.response_header.status = Protocol::Status::FailureFileNotFound;
		internal_response.filename = internal_request.filename.value();
		return internal_response;
	}

	std::filesystem::remove(file_path);

	internal_response.response_header.status = Protocol::Status::SuccessFileBackedUpOrDeleted;
	internal_response.filename = internal_request.filename.value();

	return internal_response;
}

Protocol::InternalResponse Handler::ListFiles(const Protocol::InternalRequest& internal_request)
{
	Protocol::InternalResponse internal_response;
	internal_response.response_header.version = Config::version;

	auto user_dir = get_user_directory_path(internal_request.request_header.user_id);

	if (!std::filesystem::exists(user_dir))
	{
		internal_response.response_header.status = Protocol::Status::FailureNoFilesForClient;
		return internal_response;
	}

	std::stringstream file_list;

	for (auto const& file : std::filesystem::directory_iterator{ user_dir })
	{
		file_list << file;
	}

	internal_response.response_header.status = Protocol::Status::SuccessFileListReceived;
	internal_response.filename = generate_random_filename();
	internal_response.payload = file_list.str();
	return internal_response;
}

std::filesystem::path Handler::get_user_directory_path(uint32_t user_id)
{
	std::stringstream serialized_user_id;
	serialized_user_id << user_id;

	return std::filesystem::path(Config::working_directory) / serialized_user_id.str();
}

std::string Handler::generate_random_filename()
{
	std::srand(std::time(nullptr));  // Seed random. If 2 clients connect in the same second, they will have the same filename.
	std::stringstream random_filename;
	char letters_and_numbers[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	for (int i = 0; i < 32; i++)
	{
		random_filename << letters_and_numbers[std::rand() % (sizeof(letters_and_numbers) - 1)];
	}

	return random_filename.str();
}

std::string Handler::read_whole_file(std::filesystem::path file_path)
{
	std::ifstream file(file_path);
	std::stringstream payload;
	payload << file.rdbuf();
	return payload.str();
}
