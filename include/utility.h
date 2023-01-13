#pragma once
#include "UserAccount.h"
#include <vector>

namespace Utility
{
	std::string encrypt(std::string password);
	std::string decrypt(std::string password);
	std::vector<std::string> returnAdmins();
}