#include <iostream>
#include "gui.h"
#include <fstream>
#include <filesystem>
#include "utility.h"
#include "Administrator.h"
#include "Configuration.h"
#include "DriverAccount.h"
int main()
{
	config::configure();
	gui::loginInterface();
	return 0;
}