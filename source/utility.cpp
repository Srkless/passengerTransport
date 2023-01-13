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
