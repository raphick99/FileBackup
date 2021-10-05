#pragma once

#include <cstdint>
#include <string>

/*
* Config namespace. declares configurations for the project
*/
namespace Config
{
	static const uint16_t port = 13337;
	static const std::string working_directory("C:\\temp\\backupsvr");
	static const uint8_t version = 1;
}