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
	Ride singleRouteOverview(const std::string& fileName) const; //TODO VODNJA.H
	std::unordered_map <std::string, Ride> allRoutesOverview() const;
	std::unordered_map <std::string, Ride> allDrivenRides() const;
	std::unordered_map <std::string, Ride> allUndrivenRides() const;
	void writeReport(const std::string& fileName, Report& report) const; /// sve ok
	void writeProblemReport(const std::string& fileName, ProblemReport& report) const;	// problemi
	std::unordered_map <std::string, Report> overviewReports() const;
	std::unordered_map<std::string, ProblemReport> overviewProblemReports() const;
	void driveRoute(const std::string &fileName);
	bool checkRoute() const;
};