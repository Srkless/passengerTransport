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
	config::configure();
	////Administrator admin;
	////admin.CreateCodeBook();
	////admin.CreateCodeBook();
	//////admin.SuspendingUserAccount();
	////admin.ViewUserAccount();
	gui::loginInterface();

	return 0;
}