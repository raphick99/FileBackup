#pragma once

enum class InternalStatus
{
	Uninitialized = -1,
	Success = 0,
	Session_UnrecognizedOpCode,
	Utility_SocketClosedAtOtherEndpoint,
	Utility_IncompleteRead,
	BackupFile_InvalidArguments,
	RecoverFile_InvalidArguments,
	Utility_IncompleteWrite,
};
