#include "UserAccount.h"
#include "utility.h"
#include <iostream>
UserAccount::UserAccount() {}

UserAccount::UserAccount(std::string username, std::string password, std::string accountType, int numOfLogins)
{
	this->username = username;
	this->password = password;
	this->accountType = accountType;
	this->numOfLogins = numOfLogins;
}

bool UserAccount::operator==(const UserAccount& other)
{
	return this->username == other.username;
}

std::string UserAccount::getUsername() const
{
	return this->username;
}

std::string UserAccount::getPassword() const
{
	return this->password;
}

std::string UserAccount::getAccountType() const
{
	return this->accountType;
}

int UserAccount::getNumOfLogins() const
{
	return this->numOfLogins;
}

void UserAccount::setPassword(std::string newPassword)
{
	this->password = password;
}

UserAccount UserAccount::createAccount(std::string username, std::string password, std::string accountType)
{
	UserAccount account(username, password, accountType, 0);
	*this = account;
	return *this;
}

void UserAccount::increaseNumOfLogins()
{
	numOfLogins++;
}

void UserAccount::resetNumOfLogins()
{
	numOfLogins = 0;
}

std::ostream& operator<<(std::ostream& os, const UserAccount& account)
{
	os << account.username << "#" << account.accountType << "#" << Utility::encrypt(account.password);
	return os;
}
