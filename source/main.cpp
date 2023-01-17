#include <iostream>
#include "gui.h"
#include <csignal>
#include <fstream>
#include <filesystem>
#include "utility.h"
#include "Administrator.h"
#include "Configuration.h"
#include "UserAccount.h"
#include "Report.h"
#include "ProblemReport.h"
#include "Ride.h"
#include <cstdio>
#include <exception>
#include <cassert>

void resetProgram()
{
	std::filesystem::path path = std::filesystem::current_path();
	path += "\\crash_backup\\";
	db::createSafetyCopy(path);
	remove("data");
	std::filesystem::create_directories("data");
	path += "data\\users";
	std::filesystem::path copyDestination("data");
	std::filesystem::copy(path, copyDestination, std::filesystem::copy_options::recursive);

	char* argv[3];
	argv[0] = strdup("softver-za-prevoz-putnika");
	argv[1] = NULL;
	argv[2] = NULL;
	execvp(argv[0], argv);
}

void breakpoint_handler(int signum)
{
	throw std::exception("Program Crashed Unexpectedly!");
}

void assertion_handler(int sig)
{
	throw std::exception("Program Crashed Unexpectedly!");
}


int main()
{
	// IN CASE THE PROGRAM CRASHES, DELETE THE DATA FOLDER, CREATE A SAFETY BACKUP AND RESTART THE PROGRAM

	try
	{
		config::configure();
		gui::loginInterface();
		signal(SIGILL, breakpoint_handler);
		signal(SIGABRT, assertion_handler);
		assert(false);
	}
	catch (...)
	{
		resetProgram();
		return 0;
	}
	return 0;
}