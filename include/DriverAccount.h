#include <string>
#include <vector>
#include "UserAccount.h"
#include "Ride.h"
#include "Report.h"
#include "ProblemReport.h"

class DriverAccount : public UserAccount
{
public:
	using UserAccount::UserAccount;
	std::vector<std::string> routeOverview(const std::string& fileName) const noexcept(false); //TODO VODNJA.H
	void writeReport(const std::string& fileName, Report& report) const noexcept(false); /// sve ok
	void problemReport(const std::string& fileName, ProblemReport& report) const noexcept(false);	// problemi
	std::string overview(const std::string& fileName) const noexcept(false);
};