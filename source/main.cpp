#include <iostream>
#include "gui.h"
#include <fstream>
#include <filesystem>
#include "utility.h"
#include "Administrator.h"
#include "Configuration.h"
int main()
{
	config::configure();
	//Administrator admin;
	//admin.CreateCodeBook();
	//admin.CreateCodeBook();
	////admin.SuspendingUserAccount();
	//admin.ViewUserAccount();
	gui::login_interface();
	return 0;
}