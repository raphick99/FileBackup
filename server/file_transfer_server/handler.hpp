#pragma once

#include <filesystem>
#include <string>
#include "protocol/request_header.hpp"
#include "protocol/response_header.hpp"

class Handler
{
public:
	static Protocol::InternalResponse BackupFile(const Protocol::InternalRequest&);
	static Protocol::InternalResponse RecoverFile(const Protocol::InternalRequest&);
	static Protocol::InternalResponse DeleteBackedUpFile(const Protocol::InternalRequest&);
	static Protocol::InternalResponse ListFiles(const Protocol::InternalRequest&);

private:
	static std::filesystem::path get_user_directory_path(uint32_t);
	static std::string generate_random_filename();
};
