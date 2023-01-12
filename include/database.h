#pragma once
#include <unordered_map>
#include <iostream>
#include <memory>
#include <concepts>
#include <functional>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <type_traits>
#include "Ride.h"
#include "UserAccount.h"
#include "Schedule.h"
#include "Report.h"
#include "ProblemReport.h"

namespace db
{
	inline bool checkName(std::string fileArray, std::string fileName)
	{
		std::vector<std::string> fileData;
		std::filesystem::path path1 = std::filesystem::current_path();
		path1 += "\\data";
		std::filesystem::create_directories(path1);
		path1 += fileArray;
		path1 += "\\.txt";
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

	inline Schedule readScheduleFromFile()
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\rides\\schedule.txt";
		std::ifstream iFile(path);
		iFile.open(path, std::ios::app);

		Schedule tmp;
		iFile >> tmp;
		return tmp;
	}

	inline void editScheduleFile(const Schedule& schedule)
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\rides";
		std::filesystem::create_directories(path);
		path += "\\schedule.txt";
		std::ofstream oFile(path);
		oFile.open(path, std::ios::app);

		oFile.seekp(0, std::ios::end);
		if (oFile.tellp() == 0)
		{
			oFile << schedule;
		}
		else
		{
			oFile << std::endl << schedule;
		}
	}

	// read rides from file into an unordered map
	inline std::unordered_map<std::string, Ride> loadRidesFromFile()
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\rides";
		std::filesystem::create_directories(path);
		path += "\\ridedata.txt";
		std::ifstream iFile(path);
		std::unordered_map<std::string, Ride> rides;

		while (!iFile.eof())
		{
			path = std::filesystem::current_path();
			path += "\\data\\rides\\";
			std::string name;
			std::getline(iFile, name);
			path += name;
			std::ifstream iFile2(path);
			Ride newRide;
			iFile2 >> newRide;
			rides[newRide.getRideID()] = newRide;
			iFile2.close();
		}
		iFile.close();
		return rides;
	}

	// add Ride to a file
	inline void addRideToFile(const Ride& ride)
	{
		std::ofstream oFile;
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\rides";
		std::filesystem::create_directories(path);
		path += "\\ridedata.txt";
		oFile.open(path, std::ios::app);

		oFile.seekp(0, std::ios::end);
		if (oFile.tellp() == 0)
		{
			oFile << ride.getRideID() << ".txt";
		}


		oFile.close();

		path = std::filesystem::current_path();
		path += "\\data\\rides\\";
		path += ride.getRideID();
		path += ".txt";
		std::string cmpName = ride.getRideID() + ".txt";
		if (!checkName("ridedata.txt", cmpName))
		{
			oFile << std::endl << ride.getRideID() << ".txt";
			oFile.open(path);
			oFile << ride;
			oFile.close();
		}
		else
			std::invalid_argument("File allready exists!");

	}

	// loads users from database into an unordered map

	inline std::unordered_map<std::string, UserAccount> loadUsersFromFile()
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\users";
		std::filesystem::create_directories(path);
		path += "\\userdata.txt";
		std::ifstream iFile(path);
		std::unordered_map<std::string, UserAccount> users;

		while (!iFile.eof())
		{
			UserAccount acc;
			iFile >> acc;
			users[acc.getUsername()] = acc;
		}
		iFile.close();
		return users;
	}

	inline std::unordered_map<std::string, Report> loadReportsFromFile()
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\reports";
		std::filesystem::create_directories(path);
		path += "\\AllReports.txt";
		std::ifstream iFile(path);
		std::unordered_map<std::string, Report> reports;

		while (!iFile.eof())
		{
			std::filesystem::path path = std::filesystem::current_path();
			path += "\\data\\reports";

			std::string name;
			std::getline(iFile, name);
			path += name;

			Report rep;

			std::ifstream iFile2(path);
			iFile2 >> rep;

			reports[rep.getRideID()] = rep;
			iFile2.close();
		}
		iFile.close();

		return reports;
	}

	inline std::unordered_map<std::string, ProblemReport> loadProblemReportsFromFile()
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\reports";
		std::filesystem::create_directories(path);
		path += "\\AllProblemReports.txt";
		std::ifstream iFile(path);
		std::unordered_map<std::string, ProblemReport> problemReports;

		while (!iFile.eof())
		{
			std::filesystem::path path = std::filesystem::current_path();
			path += "\\data\\reports";

			std::string name;
			std::getline(iFile, name);
			path += name;

			ProblemReport probRep;

			std::ifstream iFile2(path);
			iFile2 >> probRep;

			problemReports[probRep.getRideID()] = probRep;
			iFile2.close();
		}
		iFile.close();

		return problemReports;
	}

	// adds user to database
	// should only be used during registration 

	inline void addUserToFile(const UserAccount& usr)
	{
		std::ofstream oFile;
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\users";
		std::filesystem::create_directories(path);
		path += "\\userdata.txt";
		oFile.open(path, std::ios::app);

		oFile << std::endl << usr;
		oFile.close();
	}

	// write users to database
	inline void writeUsersToFile(std::unordered_map<std::string, UserAccount>& map)
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\users";
		std::filesystem::create_directories(path);
		path += "\\userdata.txt";
		std::ofstream oFile(path);

		for (auto& user : map)
		{
			if (user.second.getUsername() != "")
			{
				oFile.seekp(0, std::ios::end);
				if (oFile.tellp() == 0)
					oFile << user.second;
				else
					oFile << std::endl << user.second;
			}
		}
		oFile.close();
	}

	template<typename T>
	concept Readable = requires(std::istream & is, T & a)
	{
		{is >> a} ->std::convertible_to<std::istream&>;
	};

	// function for reading a specified type from a file
	// assumes that the '#' symbol terminates the data type
	// default type is string
	template<typename T = std::string>
		requires Readable<T> && (std::copy_constructible<T> || std::copyable<T>)
	T readItem(std::ifstream& is)
	{
		std::string line;
		std::getline(is, line, '#');

		std::stringstream stream(line);

		T tmp;

		stream >> tmp;

		return tmp;
	}

	inline std::unordered_map<std::string, UserAccount> loadDriversFromFile()
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\users";
		std::filesystem::create_directories(path);
		path += "\\userdata.txt";
		std::ifstream iFile(path);
		std::unordered_map<std::string, UserAccount> drivers;

		while (!iFile.eof())
		{
			UserAccount acc;
			iFile >> acc;
			if (acc.getAccountType() == "driver")
			{
				drivers[acc.getUsername()] = acc;
			}

		}
		iFile.close();
		return drivers;
	}

	inline std::unordered_map<std::string, Ride> loadDriverRides()
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\rides";
		std::filesystem::create_directories(path);
		path += "\\userdata.txt";
		std::ifstream iFile(path);
		std::unordered_map<std::string, Ride> rides;

		while (!iFile.eof())
		{
			Ride tmpRide;
			iFile >> tmpRide;


		}
		iFile.close();
		return rides;
	}

	// function for reading the whole file
	// can only read one type of data, specified by T
	// accepts two std::function wrappers
	// the first argument takes the path of the file which will be read from
	// the second argument takes a function which will define what will be done with the output
	// the third argument takes a function which defines the method of reading from the file and can be omitted - by default will read one string from the file
	// here is an example of how to call this function from main
	// 
	// this will print out the first word from the test.txt file located in the Documents folder
	// 
	// db::readFileCustom<std::string>("C:/Users/urnm/Documents/test.txt", [](std::string str) {std::cout << str; });
	//
	// template parameters:
	// T is the type of data being read from file
	// O is the return type of the specified work function, if the function returns nothing, shoule be void
	//
	/*template<typename T, typename O = void>
	O readFileCustom(std::filesystem::path path, std::function<O(T& a)> outputFunction, std::function<T(std::ifstream& is)> inputFunction = [](std::ifstream& is) {return readItem<T>(is); })
	{
		std::ifstream inputFile(path);
		T workData;
		std::stringstream sstream;
		std::getline(inputFile, sstream);
		while(!inputFile.eof())
		{
			workData = inputFunction(inputFile);
			O returnVal = outputFunction(workData);
		}
		O returnVal = outputFunction(workData);
		inputFile.close();
		return returnVal;
	}*/


};

