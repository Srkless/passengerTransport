#pragma once
#include <iostream>
#include <string>
class UserAccount
{
private:
	std::string username;
	std::string password;
	std::string accountType;
	int numOfLogins;
	bool isSuspended;
public:
	UserAccount();
	UserAccount(std::string username, std::string password, std::string accountType, int numOfLogins);

	bool operator==(const UserAccount& other);
	std::string getUsername() const;
	std::string getPassword() const;
	std::string getAccountType() const;
	int getNumOfLogins() const;

	void setPassword(std::string newPassword);
	UserAccount createAccount(std::string username, std::string password, std::string accountType);
	void increaseNumOfLogins();
	void resetNumOfLogins();

	friend std::ostream& operator<<(std::ostream& os, const UserAccount& account);
	friend std::istream& operator>>(std::istream& is, UserAccount& account);
};