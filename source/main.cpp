#include <iostream>
#include "gui.h"
#include <fstream>
#include <filesystem>
#include "utility.h"
#include "Administrator.h"
#include "Configuration.h"
#include "UserAccount.h"
#include "DriverAccount.h"
#include "Report.h"
#include "ProblemReport.h"
#include "Ride.h"

int main()
{

	DriverAccount driver("Marko", "12345", "Driver", 2);

	std::stringstream ss;

	auto sT = std::chrono::system_clock::now();
	ss << sT;
	std::string startTime = ss.str();

	auto eT = std::chrono::system_clock::now();
	std::string endTime = ss.str();

	std::string startLocation = "Banja_Luka";
	std::string endLocation = "Bijeljina";

	std::vector < std::string> path = { "Prnjavor", "Derventa", "Doboj" };


	Ride ride("N126", driver.getUsername(), "N299", startTime, endTime, startLocation, path, endLocation);


	db::addRideToFile(ride);

	std::unordered_map <std::string, Ride> map = db::loadRidesFromFile();


	//std::cout << driver.singleRouteOverview("N126");

	//Report report("N875", "vojislav", "Djenerale djenerale slava tvojoj majci!");

	//Report report1("L2K5", "vojislav", "Djenerale djenerale slava tvojoj majci!");

	////driver.writeReport("probaReport.txt", report);

	////driver.writeReport("proba2Report.txt", report1);

	//ProblemReport problemReport("KK23", "draza", "kvar busa", "djenerale linija je osigurana!");

	////driver.writeProblemReport("probaProblemReport.txt", problemReport);

	//Ride ride2("N126", driver.getUsername(), "N299", startTime, endTime, startLocation, path, endLocation);

	//db::addRideToFile(ride);


	//std::vector<std::string> routes = driver.routeOverview("Path.txt");

	//driver.writeReport("Report.txt", ride);

	//driver.problemReport("ProblemReport.txt", ride);


	//config::configure();
	////Administrator admin;
	////admin.CreateCodeBook();
	////admin.CreateCodeBook();
	//////admin.SuspendingUserAccount();
	////admin.ViewUserAccount();
	//gui::login_interface();

	return 0;
}