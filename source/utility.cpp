#include "utility.h"
#include "UserAccount.h"
#define ROT 17

std::string Utility::encrypt(std::string password)
{
	for (int i = 0; i < password.size(); i++)
	{
		char character = password[i] + ROT;

		if (password[i] >= 'A' && password[i] <= 'Z')
		{
			if (character > 'Z')
				password[i] -= (26 - ROT % 26);
			else
				password[i] += ROT;

		}
		else if (password[i] >= 'a' && password[i] <= 'z')
		{
			if (password[i] + ROT > 'z')
				password[i] -= (26 - ROT % 26);
			else
				password[i] += ROT;
		}
		else if (password[i] == ' ')
			continue;
	}
	return password;
}

std::string Utility::decrypt(std::string password)
{
	for (int i = 0; i < password.size(); i++)
	{
		char character = password[i] - ROT;

		if (password[i] >= 'A' && password[i] <= 'Z')
		{
			if (character >= 'A')
				password[i] -= ROT;
			else
				password[i] += (26 - ROT % 26);
		}
		else if (password[i] >= 'a' && password[i] <= 'z')
		{
			if (password[i] - ROT >= 'a')
				password[i] -= ROT;
			else
				password[i] += (26 - ROT % 26);
		}
		else if (password[i] == ' ')
			continue;
	}
	return password;
}

std::vector<std::string> Utility::returnAdmins()
{
	std::unordered_map<std::string, UserAccount> userDatabase;
	userDatabase = db::loadUsersFromFile();
	std::vector<std::string> admins;
	for (auto& user : userDatabase)
	{
		if (user.second.getAccountType() == "administrator")
			admins.push_back(user.second.getUsername());
	}

	return admins;
}
