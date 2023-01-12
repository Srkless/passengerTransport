#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "DriverAccount.h"
#include "Ride.h"
#include "Report.h"
#include "ProblemReport.h"

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

void DriverAccount::writeReport(const std::string& fileName, Report& report) const noexcept(false)
{
	std::ofstream file;
	std::filesystem::path path1 = std::filesystem::current_path();
	path1 += "\\data\\rides";
	std::filesystem::create_directories(path1);
	path1 += fileName;
	file.open(path1, std::ios::app);

	std::ofstream fileArray;
	std::filesystem::path path2 = std::filesystem::current_path();
	path2 += "\\data\\rides";
	path2 += "\\AllReports.txt";
	fileArray.open(path2, std::ios::app);

	if (fileArray.good() && file.good())    // rideID, username, startTime, endTime, startLocation, pathLocations, endLocation
	{
		fileArray << fileName << std::endl;
		//file << report;
	}
	else
		throw std::runtime_error("File could not open!");
}

void DriverAccount::problemReport(const std::string& fileName, ProblemReport& report) const noexcept(false)
{
	std::ofstream file;
	std::filesystem::path path1 = std::filesystem::current_path();
	path1 += "\\data\\rides";
	std::filesystem::create_directories(path1);
	path1 += fileName;
	file.open(path1, std::ios::app);

	std::ofstream fileArray;
	std::filesystem::path path2 = std::filesystem::current_path();
	path2 += "\\data\\rides";
	path2 += "\\AllProblemReports.txt";
	fileArray.open(path2, std::ios::app);

	fileArray << fileName << std::endl;


	if (fileArray.good() && file.good())    // rideID, username, startTime, endTime, startLocation, pathLocations, endLocation
	{
		fileArray << fileName << std::endl;
		file << report;
	}
	else
		throw std::runtime_error("File could not open!");




	/*std::ofstream file(fileName); /// KAKO ZNATI KOJI JE PROBLEM AKO SAMO UPISUJEM #

	if (file.good())
	{
		int option;

		do {
			std::cout << "Select the problem type, Passenger[1], Bus[2]: ";
			std::cin >> option;
			if (option != 1 && option != 2)
				std::cout << "Invald option, try again!" << std::endl;
		} while (option != 1 && option != 2);

		

		std::string problem;
		std::getchar();
		report.setReportAuthor(this->getUsername());

		file << report.getRideID() << "#";
		file << report.getAuthor() << "#";
		
		if (option == 1)
			file << "Passenger problem" << "#";
		else
			file << "Bus problem" << "#";

		std::cout << "Make ride report: " << std::endl;
		std::getline(std::cin, problem);

		report.setContent(problem);

		file << report.getContent() << "#";
	}
	else
		throw std::runtime_error("File could not open!");*/
}

std::string DriverAccount::overview(const std::string& fileName) const noexcept(false)
{
	return std::string();
}
