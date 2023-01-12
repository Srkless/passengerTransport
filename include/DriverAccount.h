#include <string>
#include <vector>
#include "UserAccount.h"
#include "Ride.h"
#include "Report.h"
#include "ProblemReport.h"
#include <unordered_map>

class DriverAccount : public UserAccount
{
public:
	using UserAccount::UserAccount;
	Ride& routeOverview(const std::string& fileName, std::unordered_map<std::string, Ride>& map) const noexcept(false); //TODO VODNJA.H
	void writeReport(const std::string& fileName, Report& report) const noexcept(false); /// sve ok
	void writeProblemReport(const std::string& fileName, ProblemReport& report) const noexcept(false);	// problemi
	std::unordered_map <std::string, Report> overviewReports() const noexcept(false);
	std::unordered_map<std::string, ProblemReport> overviewProblemReports() const noexcept(false);
};