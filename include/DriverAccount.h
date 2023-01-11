#include <iostream>
#include <string>
#include <vector>
#include "UserAccount.h"
#include "Ride.h"

class DriverAccount : public UserAccount
{
public:
	using UserAccount::UserAccount;
	std::vector<std::string> routeOverview(const std::string& fileName) const noexcept(false); //TODO VODNJA.H
	void generatingTravelOrder(const std::string& travelOrderFileName, const Ride& ride) const noexcept(false); /// podaci naloga, putanja
	void writeReport(const std::string& fileName, const Ride& ride) const noexcept(false); /// sve ok
	void problemReport(const std::string& fileName, const Ride& ride) const noexcept(false);	// problemi
	std::string overview(const std::string& fileName) const noexcept(false);
};