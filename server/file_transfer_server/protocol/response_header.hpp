#pragma once

#include <cstdint>
#include "status.hpp"

namespace Protocol
{
#pragma pack(push,1)
	struct ResponseHeader
	{
		uint8_t version;
		Status status;
	};
#pragma pack(pop)

	struct InternalResponse
	{
		ResponseHeader response_header;
		std::optional<std::string> filename;
		std::optional<std::string> payload;
	};
}

