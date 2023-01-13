#pragma once
#include "UserAccount.h"
#include "database.h"
namespace Utility
{
	std::string encrypt(std::string password);
	std::string decrypt(std::string password);
	std::vector<std::string> returnAdmins();
}