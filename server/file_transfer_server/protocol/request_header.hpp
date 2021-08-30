#pragma once

#include <optional>
#include <string>
#include <cstdint>
#include "op.hpp"

namespace Protocol
{
#pragma pack(push, 1)
	struct RequestHeader
	{
		uint32_t user_id;
		uint8_t version;
		Op op;
	};
#pragma pack(pop)

	struct InternalRequest
	{
		RequestHeader request_header;
		std::optional<std::string> filename;
		std::optional<std::string> payload;
	};
}
