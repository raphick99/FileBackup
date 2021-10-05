#pragma once

#include <exception>
#include "internal_status.hpp"

/*
* class for our internal project exceptions.
*/
class InternalException : public std::exception
{
public:
	const InternalStatus status;
	InternalException(InternalStatus);
};
