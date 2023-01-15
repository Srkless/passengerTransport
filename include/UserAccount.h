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
	bool isSuspended = true;
	bool notificationAlert = false;
	double balance = 500;
public:
	UserAccount();
	UserAccount(std::string username, std::string password, std::string accountType, int numOfLogins);
	UserAccount(std::string username, std::string password, std::string accountType, int numOfLogins, double balance);

	bool operator==(const UserAccount& other);
	std::string getUsername() const;
	std::string getPassword() const;
	std::string getAccountType() const;
	int getNumOfLogins() const;
	bool getNotificationAlert() const;
	bool getSuspendInfo() const;
	double getBalance() const;

	void setPassword(std::string newPassword);
	UserAccount createAccount(std::string username, std::string password, std::string accountType, double balance = 500);
	void increaseNumOfLogins();
	void resetNumOfLogins();
	void changeSuspensionStatus();
	void changeNotificationAlert();
	void addBalance(double);
	void setBalance(double);

	friend std::ostream& operator<<(std::ostream& os, const UserAccount& account);
	friend std::istream& operator>>(std::istream& is, UserAccount& account);
};