#pragma once

#include <cstdint>

namespace Protocol
{
	enum class Op : uint8_t
	{
		BackupFile = 100,
		RecoverFile = 200,
		DeleteBackedUpFile = 201,
		ListFiles = 202,
	};
}
