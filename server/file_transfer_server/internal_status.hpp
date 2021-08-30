#pragma once

enum class InternalStatus
{
	Uninitialized = -1,
	Success = 0,
	Session_SocketClosedAtOtherEndpoint,
	Session_UnrecognizedOpCode,
	Session_IncompleteRead,
	Session_EofReached,
	BackupFile_InvalidArguments,
	RecoverFile_InvalidArguments,
};
