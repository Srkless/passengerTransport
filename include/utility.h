#pragma once
#include "UserAccount.h"

namespace Utility
{
	std::string encrypt(std::string password);
	std::string decrypt(std::string password);
}