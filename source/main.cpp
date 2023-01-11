#include <iostream>
#include "gui.h"
#include <fstream>
#include <filesystem>
#include "utility.h"
#include "Administrator.h"
int main()
{
	// check if the application is being run for the first time
	// if yes, generate the necessary config and data files
	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data";
	std::filesystem::create_directories(path);
	path += "\\config.txt";
	std::ifstream config(path);

	if (!config.is_open())
	{
		// if the file could not be opened, it's the first time the programm is running
		// generate the necessary files
		std::ofstream config(path);
		config << 0;
		config.close();
		
		path = std::filesystem::current_path();
		path += "\\data\\users";
		std::filesystem::create_directories(path);
		path += "\\userdata.txt";
		std::ofstream users(path);
		users << "admin#administrator#" << Utility::encrypt("admin") << "#0#0";
		users.close();
	}
	else
	{
		int value;
		config >> value;
		if (value == 0)
		{
			// the application is being run for the first time
			// generate the necessary files
			std::ofstream config(path);
			config << 0;
			config.close();

			path = std::filesystem::current_path();
			path += "\\data\\users";
			std::filesystem::create_directories(path);
			path += "\\userdata.txt";
			std::ofstream users(path);
			users << "admin#administrator#" << Utility::encrypt("admin") << "#0#0";
			users.close();
		}
	}

	Administrator admin;
	admin.CreateCodeBook();
	admin.CreateCodeBook();
	admin.ModificationCodeBoks();
	admin.DeleteCodeBook();

	gui::register_interface();
	return 0;
}