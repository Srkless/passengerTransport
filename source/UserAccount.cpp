#include "UserAccount.h"
#include "utility.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <sstream>
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

bool UserAccount::getSuspendInfo() const
{
	return this->isSuspended;
}

double UserAccount::getBalance() const
{
	return balance;
}

void UserAccount::setPassword(std::string newPassword)
{
	this->password = newPassword;
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

void UserAccount::changeSuspensionStatus()
{
	this->isSuspended = !this->isSuspended;
}

void UserAccount::addBalance(double value)
{
	balance += value;
}

void UserAccount::setBalance(double newBal)
{
	balance = newBal;
}

std::istream& operator>>(std::istream& is, UserAccount& account)
{
	std::string line;
	std::getline(is, line);
	std::stringstream sstream(line);
	std::vector<std::string> items;
	std::string item;
	size_t count = 0;
	while (std::getline(sstream, item, '#') && count < 5)
	{
		items.push_back(item);
		count++;
	}
	
	account.username = items[0];
	account.accountType = items[1];
	account.password = Utility::decrypt(items[2]);
	account.numOfLogins = std::stoi(items[3]);
	if (std::stoi(items[4]) == 1)
	{
		account.isSuspended = true;
	}
	else
	{
		account.isSuspended = false;
	}

	return is;
}

std::ostream& operator<<(std::ostream& os, const UserAccount& account)
{
	os << account.username << "#" << account.accountType << "#" << Utility::encrypt(account.password) << "#" << account.numOfLogins << "#" << account.isSuspended << "#" << account.balance << "#";
	return os;
}
