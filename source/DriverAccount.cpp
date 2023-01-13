#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "DriverAccount.h"
#include "Ride.h"
#include "Report.h"
#include "ProblemReport.h"
#include "database.h"
#include <optional>

inline bool checkName(const std::string& fileDirectory, const std::string& fileArray, const std::string& fileName)
{
	std::vector<std::string> fileData;
	std::filesystem::path path1 = std::filesystem::current_path();
	path1 += "\\data";
	std::filesystem::create_directories(path1);
	path1 += fileDirectory; 
	path1 += fileArray;
	std::string word;
	std::ifstream file(path1);

	while (file >> word)
	{
		fileData.push_back(word);
	};

	file.close();
	std::string name;
	for (int i = 0; i < fileData.size(); i++)
	{
		if (fileName == fileData[i])
			return true;
	}
	return false;
}

Ride DriverAccount::singleRouteOverview(const std::string& fileName) const
{
	std::unordered_map<std::string, Ride> map = db::loadDriverRides(getUsername());

	if (map.count(fileName) > 0)
		return map[fileName];
}

std::unordered_map<std::string, Ride> DriverAccount::allRoutesOverview() const
{
	return db::loadDriverRides(getUsername());
}

std::unordered_map<std::string, Ride> DriverAccount::allDrivenRides() const
{
	std::unordered_map<std::string, Ride> trueMap, map = db::loadDriverRides(getUsername());

	for (auto& ride : map)
	{
		if (ride.second.getDrivenStatus())
		{
			trueMap[ride.first] = ride.second;
		}
	}
	return trueMap;
}

std::unordered_map<std::string, Ride> DriverAccount::allUndrivenRides() const
{
	std::unordered_map<std::string, Ride> falseMap, map = db::loadDriverRides(getUsername());

	for (auto& ride : map)
	{
		if (!ride.second.getDrivenStatus())
		{
			falseMap[ride.first] = ride.second;
		}
	}
	return falseMap;
}



void DriverAccount::writeReport(const std::string& fileName, Report& report) const
{
	std::ofstream file;
	std::filesystem::path path1 = std::filesystem::current_path();
	path1 += "\\data\\reports\\";
	std::filesystem::create_directories(path1);
	path1 += fileName;
	file.open(path1, std::ios::app);

	std::ofstream fileArray;
	std::filesystem::path path2 = std::filesystem::current_path();
	path2 += "\\data\\reports";
	path2 += "\\allReports.txt";
	fileArray.open(path2, std::ios::app);

	fileArray.seekp(0, std::ios::end);
	if (fileArray.tellp() == 0)
	{
		fileArray << fileName;
	}
	else if (fileArray.good() && file.good())
	{
		if (!checkName("\\reports\\", "AllReports.txt", fileName))
		{
			fileArray << std::endl << fileName;
			file << report;
		}
		fileArray.close();
		file.close();
	}
}

void DriverAccount::writeProblemReport(const std::string& fileName, ProblemReport& report) const
{
	std::ofstream file;
	std::filesystem::path path1 = std::filesystem::current_path();
	path1 += "\\data\\problemReports\\";
	std::filesystem::create_directories(path1);
	path1 += fileName;
	file.open(path1, std::ios::app);

	std::ofstream fileArray;
	std::filesystem::path path2 = std::filesystem::current_path();
	path2 += "\\data\\problemReports";
	path2 += "\\AllProblemReports.txt";
	fileArray.open(path2, std::ios::app);


	if (fileArray.good() && file.good())    // rideID, username, startTime, endTime, startLocation, pathLocations, endLocation
	{
		if (!checkName("\\problemReports\\", "AllProblemReports.txt", fileName))
		{
			fileArray << fileName << std::endl;
			file << report;
		}
	}
}

std::unordered_map<std::string, Report> DriverAccount::overviewReports() const
{
	return db::loadReportsFromFile();
}

std::unordered_map<std::string, ProblemReport> DriverAccount::overviewProblemReports() const
{
	return db::loadProblemReportsFromFile();
}

void DriverAccount::driveRoute(const std::string& fileName) // prima RideID (ime .txt fajla)
{
	std::unordered_map<std::string, Ride> driverMap = db::loadDriverRides(getUsername());

	if (driverMap.count(fileName) > 0)
	{
		driverMap[fileName].changeDrivenStatus();

		db::rewriteExistingRide(driverMap[fileName]);
	}
}
