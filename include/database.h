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
#include "UserAccount.h"

namespace db
{

	// loads users from database into an unordered map

	std::unordered_map<std::string, UserAccount> loadUsersFromFile()
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\users";
		std::filesystem::create_directories(path);
		path += "\\userdata.txt";
		std::ifstream iFile(path);
;
		std::unordered_map<std::string, UserAccount> users;

		while(!iFile.eof())
		{
			UserAccount acc;
			iFile >> acc;
			users[acc.getUsername()] = acc;
		}
		iFile.close();
		return users;
	}

	// adds user to database
	// should only be used during registration 

	void addUserToFile(const UserAccount& usr)
	{
		std::ofstream oFile;
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\users";
		std::filesystem::create_directories(path);
		path += "userdata.txt";
		oFile.open(path, std::ios::app);

		oFile << usr;
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

	
}

