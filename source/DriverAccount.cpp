#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "DriverAccount.h"

std::vector<std::string> DriverAccount::routeOverview(const std::string& fileName) const noexcept(false)
{
	std::ifstream file(fileName);

	if (file.good())

	{
		std::vector<std::string> locationArray;

		std::string location;
		while (std::getline(file, location, '#'))
			locationArray.push_back(location);

		std::string startLocation = locationArray.front();
		std::string endLocation = locationArray.back();

		for (auto& string : locationArray)
			std::cout << string << " ";

		return locationArray;
	}
	else
		throw std::runtime_error("File not found!");
}

void DriverAccount::generatingTravelOrder(const std::string& travelOrderFileName, const Ride& ride) const noexcept(false)
{
	std::ofstream file(travelOrderFileName);

	if (file.good())                    // rideID, username, startTime, endTime, startLocation, pathLocations, endLocation
	{
		file << ride.getRideID() << "#";
		file << ride.getDriver() << "#";
		file << ride.getStartTime() << "#";
		file << ride.getEndTime() << "#";
		file << ride.getStartLocation() << "#";

		auto route = ride.getPathLocations();
		for (auto& string : route)
			file << string << "#";

		file << ride.geEndLocation() << "#";
	}
	else
		throw std::runtime_error("File could not open!");
}

void DriverAccount::writeReport(const std::string& fileName, const Ride& ride) const noexcept(false)
{
	std::ofstream file(fileName);

	if (file.good())                    // routeID, name(username), content
	{
		std::string report;

		file << ride.getRideID() << "#";
		file << ride.getDriver() << "#";

		std::cout << "Make ride report: " << std::endl;
		std::getline(std::cin, report);

		file << report << "#";
	}
	else
		throw std::runtime_error("File could not open!");
}

void DriverAccount::problemReport(const std::string& fileName, const Ride& ride) const noexcept(false)
{
	std::ofstream file(fileName); /// KAKO ZNATI KOJI JE PROBLEM AKO SAMO UPISUJEM #

	if (file.good())
	{
		int option;

		do {
			std::cout << "Select the problem type, Passenger[1], Bus[2]: ";
			std::cin >> option;
			if (option != 1 && option != 2)
				std::cout << "Invald option, try again!" << std::endl;
		} while (option != 1 && option != 2);

		/*if (option == 1)
			file << "Passenger problem" << std::endl << std::endl;
		else
			file << "Bus problem" << std::endl << std::endl;*/

		std::string problem;
		std::getchar();

		file << ride.getRideID() << "#";
		file << ride.getDriver() << "#";

		std::cout << "Make ride report: " << std::endl;
		std::getline(std::cin, problem);

		file << problem << "#";
	}
	else
		throw std::runtime_error("File could not open!");
}

std::string DriverAccount::overview(const std::string& fileName) const noexcept(false)
{
	return std::string();
}
