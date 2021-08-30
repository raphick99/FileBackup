#pragma once

#include <cstdint>

namespace Protocol
{
	enum class Status : uint16_t
	{
		SuccessFileRecovered = 210,
		SuccessFileListReceived = 211,
		SuccessFileBackedUpOrDeleted = 212,
		FailureFileNotFound = 1001,
		FailureNoFilesForClient = 1002,
		FailureGeneralError = 1003,
	};
}
