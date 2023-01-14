#include "gui.h"
#include <sstream>

void choiceAccountInterface(UserAccount& administrator)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Account types";
	ftxui::Color bannerMessageColor = blue;
	auto createAdministratorAccount = ftxui::Button("Create administrator account", [&] {gui::registerInterface(administrator.getUsername(), 1); });
	auto createDriverAccount = ftxui::Button("Create driver account", [&] {gui::registerInterface(administrator.getUsername(), 2); });
	auto backButton = ftxui::Button("BACK", [&] {gui::accountSettingsInterface(administrator); });
	auto component = ftxui::Container::Vertical({ createAdministratorAccount,  createDriverAccount, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(),
		   center(hbox(center(createAdministratorAccount->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))),
		   center(hbox(center(createDriverAccount->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))),
		   center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
};
void editAccountInterface(int value, UserAccount& administrator)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = administrator.getUsername() + "'s account settings";
	ftxui::Color bannerMessageColor = blue;
	std::unordered_map<std::string, UserAccount> userDatabase;
	std::vector<std::string> entries;
	userDatabase = db::loadUsersFromFile();
	for (auto& elem : userDatabase)
	{
		if (value == 1)
		{
			if (elem.second.getSuspendInfo() && elem.second.getUsername() != administrator.getUsername())
				entries.push_back(elem.first);
		}
		else if (value == 2)
		{
			if (!elem.second.getSuspendInfo() && elem.second.getUsername() != administrator.getUsername())
				entries.push_back(elem.first);
		}
		else if (elem.second.getUsername() != administrator.getUsername())
		{
			entries.push_back(elem.first);
		}
	}

	int selected = -1;
	auto menu = Radiobox(&entries, &selected);
	auto acceptButton = ftxui::Button("Accept", [&] {(value == 3 && selected != -1) ? (userDatabase.erase(entries[selected]), db::writeUsersToFile(userDatabase), gui::accountSettingsInterface(administrator))
		: (value == 4 && selected != -1) ? (userDatabase[entries[selected]].setPassword("admin"), db::writeUsersToFile(userDatabase), gui::accountSettingsInterface(administrator)) : (userDatabase[entries[selected]].changeSuspensionStatus(), db::writeUsersToFile(userDatabase), gui::accountSettingsInterface(administrator)); });
	auto backButton = ftxui::Button("BACK", [&] {gui::accountSettingsInterface(administrator); });
	auto component = ftxui::Container::Vertical({ menu,acceptButton, backButton });

	auto renderer = ftxui::Renderer(component, [&] {

		if (value > 0 && entries.size() != 0)
		return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
	   separatorDouble(),
	   center(hbox(center(menu->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))),
	   center(hbox(center(acceptButton->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))),
	   center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		else
			return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
			   separatorDouble(),
			   center(hbox(center(menu->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))),
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}
void gui::accountSettingsInterface(UserAccount& administrator)
{
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string currUsername = administrator.getUsername();
	std::string bannerMessage = currUsername + "'s account settings";

	// Account Settings
	auto createAccount = ftxui::Button("Create account", [&] {choiceAccountInterface(administrator); });
	auto activateAccount = ftxui::Button("Activate account", [&] {editAccountInterface(1, administrator); });
	auto suspendAccount = ftxui::Button("Suspend account", [&] {editAccountInterface(2, administrator); });
	auto deleteAccount = ftxui::Button("Delete account", [&] {editAccountInterface(3, administrator); });
	auto viewAccounts = ftxui::Button("View accounts", [&] {editAccountInterface(0, administrator); });

	auto suspendPassword = ftxui::Button("Suspend password", [&] {editAccountInterface(4, administrator); });
	auto changePassword = ftxui::Button("Change password", [&] {gui::changePassword(administrator.getUsername()); });
	auto backButton = ftxui::Button("BACK", [&] {gui::administrator_interface(administrator); });

	auto component = ftxui::Container::Vertical({ createAccount, activateAccount, suspendAccount, deleteAccount, viewAccounts, suspendPassword, changePassword, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(createAccount->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray))),
				center(hbox(activateAccount->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray))),
				center(hbox(suspendAccount->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray))),
				center(hbox(deleteAccount->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray))),
				center(hbox(viewAccounts->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray))),
				center(hbox(suspendPassword->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray))),
				center(hbox(changePassword->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray))),
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});

	screen.Loop(renderer);
}

void gui::addRideInterface(UserAccount& administrator)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	ftxui::Color bannerMessageColor = blue;
	std::string bannerMessage = "Add new ride";
	std::string RideID, Driver, BusRegistration, StartTime, EndTime, Location;

	ftxui::Component RideIDComponent = ftxui::Input(&RideID, "RideID");
	ftxui::Component DriverComponent = ftxui::Input(&Driver, "Driver");
	ftxui::Component BusRegistrationComponent = ftxui::Input(&BusRegistration, "BusRegistration");
	ftxui::Component StartTimeComponent = ftxui::Input(&StartTime, "StartTime");
	ftxui::Component EndTimeComponent = ftxui::Input(&EndTime, "EndTime");
	ftxui::Component LocationComponent = ftxui::Input(&Location, "Location");

	std::vector<std::string> allLocations;

	std::string item;

	int flag = 0;

	auto enterButton = ftxui::Button("DONE", [&] {
		std::stringstream ss(Location);
	while (std::getline(ss, item, ','))
		allLocations.push_back(item);

	if(allLocations.size() >= 3)
	{
		auto start = allLocations.begin() + 1;
		auto end = allLocations.end() - 1;
		std::vector<std::string> locations(start, end);
		Ride ride(RideID, Driver, BusRegistration, StartTime, EndTime, allLocations[0], locations, allLocations[locations.size() - 1]);
		db::addRideToFile(ride);
	}
	else if (allLocations.size() == 2)
	{
		Ride ride(RideID, Driver, BusRegistration, StartTime, EndTime, allLocations[0], allLocations[1]);
		db::addRideToFile(ride);
	}
	gui::scheduleSettings(administrator);
		});

	auto backButton = ftxui::Button("BACK", [&] {gui::scheduleSettings(administrator); });

	ftxui::Color inputColor = light_gray;
	auto component = ftxui::Container::Vertical({ RideIDComponent, DriverComponent, BusRegistrationComponent, StartTimeComponent, EndTimeComponent, LocationComponent, enterButton, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		if (Location == "")
		flag = 0;
		else
			flag = 1;
	return  vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(),
			center(vbox({center(hbox({hbox(center(RideIDComponent->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray))) | ftxui::borderRounded,
			center(hbox(center(DriverComponent->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))) | ftxui::borderRounded,
			center(hbox(center(BusRegistrationComponent->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))) | ftxui::borderRounded})) | ftxui::borderRounded,
			center(hbox({hbox(center(StartTimeComponent->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray))) | ftxui::borderRounded,
			center(hbox(center(EndTimeComponent->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))) | ftxui::borderRounded,
			center(hbox(center(LocationComponent->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))) | ftxui::borderRounded})) | ftxui::borderRounded})),
			(flag == 1) ? (center(hbox(hbox(center(enterButton->Render() | size(WIDTH, EQUAL, 12) | ftxui::color(bright_green)))))) :
			(center(hbox(hbox(center(backButton->Render() | size(WIDTH, EQUAL, 12) | ftxui::color(bright_green)))))) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });


	screen.Loop(renderer);
}

int deleteRideFile(std::string name)
{
	std::filesystem::path path = std::filesystem::current_path();

	path += "\\data\\rides\\";
	path += name;

	std::filesystem::remove(path);

	std::filesystem::path pathAllRides = std::filesystem::current_path();
	pathAllRides += "\\data\\rides";
	std::filesystem::create_directories(pathAllRides);
	pathAllRides += "\\allRides.txt";


	std::vector<std::string> allRides;

	std::string files;
	std::ifstream rides(pathAllRides);

	while (rides >> files)
	{
		allRides.push_back(files);
	};
	rides.close();

	auto n = std::find(allRides.begin(), allRides.end(), name);

	if (n != allRides.end())
	{
		allRides.erase(n);
	};


	std::ofstream newRides;
	newRides.open(pathAllRides);

	for (int i = 0; i < allRides.size(); i++)
	{
		newRides << allRides[i] << std::endl;
	};
	newRides.close();
	return 1;
};

void gui::deleteRideInterface(UserAccount& administrator)
{
	std::filesystem::path pathAllRides = std::filesystem::current_path();
	pathAllRides += "\\data\\rides";
	std::filesystem::create_directories(pathAllRides);
	pathAllRides += "\\allRides.txt";

	std::string files;
	std::ifstream Data(pathAllRides);

	std::vector<std::string> allRides;

	while (Data >> files)
	{
		allRides.push_back(files);
	};

	Data.close();

	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Choose file!";

	int selected = -1;
	int flag = 0;
	if (allRides.size() != 0)
		flag = 1;
	auto menu = Radiobox(&allRides, &selected);

	auto enterButton = ftxui::Button("ENTER", [&] { deleteRideFile(allRides[selected]), gui::createCodeBooksInterface(administrator); });
	auto backButton = ftxui::Button("BACK", [&] {gui::scheduleSettings(administrator); });
	//auto Enter = ftxui::Button("Enter", [&] {exit(0); });

	auto component = ftxui::Container::Vertical({ menu, enterButton, backButton});


	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(menu->Render())),
				(flag == 1) ? (center(hbox(enterButton->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green)))):
				(center(hbox(backButton->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))))
				}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});
	screen.Loop(renderer);

};


void gui::scheduleSettings(UserAccount& administrator)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	ftxui::Color bannerMessageColor = blue;
	std::string bannerMessage = administrator.getUsername() + "'s schedule Settings";

	auto addRide = ftxui::Button("Add ride", [&] {gui::addRideInterface(administrator); });
	auto deleteRide = ftxui::Button("Delete ride", [&] {gui::deleteRideInterface(administrator); });
	auto backButton = ftxui::Button("BACK", [&] {gui::administrator_interface(administrator); });

	auto component = ftxui::Container::Vertical({ addRide, deleteRide, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(addRide->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray))),
				center(hbox(deleteRide->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray))),
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});

	screen.Loop(renderer);
}

void viewReportInterface(UserAccount& administrator)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	std::unordered_map<std::string, Report> reportDatabase;
	reportDatabase = db::loadReportsFromFile();
	std::vector<std::string> entries;

	std::string bannerMessage = administrator.getUsername() + "'s account settings";
	ftxui::Color bannerMessageColor = blue;
	int flag = 0;
	if (reportDatabase.size() != 0)
		for (auto& elem : reportDatabase)
		{
			entries.push_back(elem.first);
			flag = 1;
		}
	int selected = -1;

	auto menu = Radiobox(&entries, &selected);
	auto backButton = ftxui::Button("BACK", [&] {gui::reportsSettings(administrator); });
	auto acceptButton = ftxui::Button("Accept", [&] {});
	auto component = ftxui::Container::Vertical({ menu, backButton, acceptButton });
	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(menu->Render())),
				center(hbox(backButton->Render() | size(WIDTH, EQUAL, 25) | ftxui::color(bright_green))),
				center(hbox(text("") | size(WIDTH, EQUAL, 25) | ftxui::color(bright_green))),
				(selected != -1 && flag == 1) ? (hbox(text(reportDatabase[entries[selected]].getContent()) | size(WIDTH, GREATER_THAN, 10) | ftxui::color(white))) : (hbox() | size(WIDTH, GREATER_THAN, 10))}) })
				| color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}
void viewProblemsInterface(UserAccount& administrator, int value)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	std::unordered_map<std::string, ProblemReport> problemDatabase;
	problemDatabase = db::loadProblemReportsFromFile();
	std::vector<std::string> entries;

	std::string bannerMessage = administrator.getUsername() + "'s account settings";
	ftxui::Color bannerMessageColor = blue;
	int flag = 0;
	for (auto& elem : problemDatabase)
	{
		if (value == 0 && elem.second.getTypeOfProblem() == "passenger")
			entries.push_back(elem.first);
		else if (value == 1 && elem.second.getTypeOfProblem() == "bus")
			entries.push_back(elem.first);
		flag = 1;
	}

	int selected = -1;

	auto menu = Radiobox(&entries, &selected);

	auto backButton = ftxui::Button("BACK", [&] {gui::reportsSettings(administrator); });
	auto component = ftxui::Container::Vertical({ menu, backButton });
	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(menu->Render())),
				center(hbox(backButton->Render() | size(WIDTH, EQUAL, 25) | ftxui::color(bright_green))),
				center(hbox(text("") | size(WIDTH, EQUAL, 25) | ftxui::color(bright_green))),
				(selected != -1 && flag == 1) ? (hbox(text(problemDatabase[entries[selected]].getContent()) | size(WIDTH, GREATER_THAN, 10) | ftxui::color(white))) : (hbox())}) })
				| color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}

void gui::reportsSettings(UserAccount& administrator)
{
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = administrator.getUsername() + "'s account Settings";

	auto viewReport = ftxui::Button("View reports", [&] { viewReportInterface(administrator); });
	auto viewPassengerProblems = ftxui::Button("View passenger problems", [&] {viewProblemsInterface(administrator, 0); });
	auto viewRideProblems = ftxui::Button("View ride problems", [&] {viewProblemsInterface(administrator, 1); });
	auto backButton = ftxui::Button("BACK", [&] {gui::administrator_interface(administrator); });

	auto component = ftxui::Container::Vertical({ viewReport, viewPassengerProblems, viewRideProblems, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(viewReport->Render() | size(WIDTH, EQUAL, 25) | ftxui::color(light_gray))),
				center(hbox(viewPassengerProblems->Render() | size(WIDTH, EQUAL, 25) | ftxui::color(light_gray))),
				center(hbox(viewRideProblems->Render() | size(WIDTH, EQUAL, 25) | ftxui::color(light_gray))),
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 25) | ftxui::color(bright_green)))}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});

	screen.Loop(renderer);
}

std::vector<std::string> CodeBook;



inline bool is_equalFile(std::string name)
{
	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data\\codebooks\\";
	path += name;
	path += ".txt";
	int i = 0;
	std::vector<std::string> strings;
	std::vector<std::string> strings2;
	std::string word, word2;
	std::ifstream file, file2;

	if (CodeBook.size() == 0)
		return false;

	while (i < CodeBook.size())
	{
		int check2 = 0;

		std::filesystem::path path1 = std::filesystem::current_path();
		path1 += "\\data\\codebooks\\";
		path1 += CodeBook[i++];
		path1 += ".txt";
		file2.open(path1);

		while (getline(file2, word))
		{
			strings.push_back(word);
		}
		file2.close();


		file.open(path);

		while (getline(file, word2))
		{
			strings2.push_back(word2);
		}

		file.close();




		int check = 0;
		for (int j = 0; j < strings.size(); j++)
		{

			word = strings2[j];

			if (strings.size() != strings2.size())
				break;


			for (int k = 0; k < strings.size(); k++)
			{
				if (word == strings[k])
					check++;
			}
			if (check != 0)
				check2++;

			check = 0;
		}

		if (check2 == strings2.size())
		{
			return true;
		};

		check2 = 0;

	};

	return false;
}
void writeLocation(std::string name, std::string country, std::filesystem::path path, std::ofstream& data)
{
	data.open(path, std::ios::app);
	if (name != "" && country != "")
		data << name << "#" << country << std::endl;
	data.close();
};

void writeinFile(std::string name, std::ofstream& data)
{
	data << name << std::endl;
};

int IsEqual(std::ofstream& data, std::filesystem::path path, std::string name)
{
	if (is_equalFile(name))
	{
		if (data.is_open())
			data.close();
		std::filesystem::remove(path);

		std::vector<std::string>CodeBooks2;

		std::filesystem::path word1 = std::filesystem::current_path();
		word1 += "\\data\\codebooks";
		std::filesystem::create_directories(word1);
		word1 += "\\data.txt";
		std::string word;
		std::ifstream Data(word1);

		while (Data >> word)
		{
			CodeBooks2.push_back(word);
		};
		Data.close();

		std::ofstream Data1;
		Data1.open(word1);
		auto it = std::find(CodeBooks2.begin(), CodeBooks2.end(), name);
		CodeBooks2.erase(it);
		for (int i = 0; i < CodeBooks2.size(); i++)
		{
			Data1 << CodeBooks2[i] << std::endl;
		}
		Data1.close();

		std::vector<std::string>Code;
		std::filesystem::path location3 = std::filesystem::current_path();
		location3 += "\\data\\codebooks";
		std::filesystem::create_directories(location3);
		location3 += "\\location.txt";

		std::ifstream location33;
		location33.open(location3);

		while (location33 >> word)
		{
			Code.push_back(word);
		};
		location33.close();

		std::ofstream Data2;
		Data2.open(location3);

		it = std::find(Code.begin(), Code.end(), name);
		Code.erase(it);
		for (int i = 0; i < Code.size(); i++)
		{
			Data2 << Code[i] << std::endl;
		}
		Data2.close();



	};

	return 1;
}






void gui::EnterLocation(UserAccount& administrator, std::string name)
{
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Location";
	std::string location;
	std::string country;

	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data\\codebooks\\";
	path += name;
	path += ".txt";
	std::ofstream data;


	ftxui::Component locationInput = ftxui::Input(&location, "Enter city");
	ftxui::Component countryInput = ftxui::Input(&country, "Enter country");

	int t = 0;
	auto backButton = ftxui::Button("BACK", [&] {(t == 1) ? IsEqual(data, path, name) : t = t, gui::createCodeLocation(administrator); });

	auto Enter = ftxui::Button("ENTER", [&] {writeLocation(country, location, path, data), gui::EnterLocation(administrator, name), t = 1; });


	auto component = ftxui::Container::Vertical({ locationInput,backButton,Enter,countryInput });



	auto renderer = ftxui::Renderer(component, [&] {
		{
			/*	if (location != "" && country != "" && t==0)
				{
					writeLocation(country, location,data);

						t = 1;
				}*/


			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({
					center(hbox(locationInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))| borderRounded,
					center(hbox(countryInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(Enter->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
					center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),

					
					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);


};

void gui::createCodeLocation(UserAccount& administrator)
{
	std::filesystem::path word1 = std::filesystem::current_path();
	word1 += "\\data\\codebooks";
	std::filesystem::create_directories(word1);
	word1 += "\\data.txt";
	std::string word;
	std::ifstream Data(word1);

	while (Data >> word)
	{
		CodeBook.push_back(word);
	};

	Data.close();

	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Location";
	std::string name;
	std::ofstream data;
	std::string type;
	data.open(word1, std::ios::app);




	std::filesystem::path location3 = std::filesystem::current_path();
	location3 += "\\data\\codebooks";
	std::filesystem::create_directories(location3);
	location3 += "\\location.txt";

	std::ofstream location33;
	location33.open(location3, std::ios::app);

	auto backButton = ftxui::Button("BACK", [&] { gui::administrator_interface(administrator); });
	ftxui::Component usernameInput = ftxui::Input(&name, "Name codebook");

	int t = 0;
	auto Enter = ftxui::Button("ENTER", [&] {writeinFile(name, data), writeinFile(name, location33), EnterLocation(administrator, name), t = 1; });


	auto component = ftxui::Container::Vertical({ usernameInput,backButton,Enter });


	auto renderer = ftxui::Renderer(component, [&] {


		t = 0;
	std::string bannerMessage = "Location";
	for (int i = 0; i < CodeBook.size(); i++)
	{
		if (name == CodeBook[i])
		{
			bannerMessageColor = red;
			t = 1;
		}
	};


	if (t == 0)
	{
		bannerMessageColor = blue;



		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(usernameInput->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green)))| borderRounded,
				center(hbox(Enter->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),
				center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green)))
				}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
	}
	else
	{
		std::string bannerMessage = "Already Exist!";
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
		separatorDouble(), vbox({
			center(hbox(usernameInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
			center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
			}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);

	}
		}
	);


	screen.Loop(renderer);


}


void writeLocationBus(std::string brand, std::string model, std::string god, std::string registration, std::string NumSeats, std::filesystem::path path, std::ofstream& data)
{
	data.open(path, std::ios::app);
	if (brand != "" && model != "" && god != "" && registration != "" && NumSeats != "")
		data << brand << "#" << model << "#" << god << "#" << registration << "#" << NumSeats << std::endl;
	data.close();
};
int IsEqualBus(std::ofstream& data, std::filesystem::path path, std::string name)
{
	if (is_equalFile(name))
	{
		if (data.is_open())
			data.close();
		std::filesystem::remove(path);

		std::vector<std::string>CodeBooks2;

		std::filesystem::path word1 = std::filesystem::current_path();
		word1 += "\\data\\codebooks";
		std::filesystem::create_directories(word1);
		word1 += "\\data.txt";
		std::string word;
		std::ifstream Data(word1);

		while (Data >> word)
		{
			CodeBooks2.push_back(word);
		};
		Data.close();

		std::ofstream Data1;
		Data1.open(word1);
		auto it = std::find(CodeBooks2.begin(), CodeBooks2.end(), name);
		CodeBooks2.erase(it);
		for (int i = 0; i < CodeBooks2.size(); i++)
		{
			Data1 << CodeBooks2[i] << std::endl;
		}
		Data1.close();

		std::vector<std::string>Code;
		std::filesystem::path location3 = std::filesystem::current_path();
		location3 += "\\data\\codebooks";
		std::filesystem::create_directories(location3);
		location3 += "\\Bus.txt";

		std::ifstream location33;
		location33.open(location3);

		while (location33 >> word)
		{
			Code.push_back(word);
		};
		location33.close();

		std::ofstream Data2;
		Data2.open(location3);

		it = std::find(Code.begin(), Code.end(), name);
		Code.erase(it);
		for (int i = 0; i < Code.size(); i++)
		{
			Data2 << Code[i] << std::endl;
		}
		Data2.close();



	};
	return 1;

}

void EnterBusInfo(UserAccount& administrator, std::string name)
{
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Bus";
	std::string brand, model, god, regis, Numseats;

	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data\\codebooks\\";
	path += name;
	path += ".txt";
	std::ofstream data;


	ftxui::Component brandInput = ftxui::Input(&brand, "Enter brand");
	ftxui::Component modelInput = ftxui::Input(&model, "Enter model");
	ftxui::Component YearInput = ftxui::Input(&god, "Enter year of production");
	ftxui::Component RegistrationInput = ftxui::Input(&regis, "Enter registration");
	ftxui::Component SeatsInput = ftxui::Input(&Numseats, "Enter number of seats");


	int t = 0;
	auto backButton = ftxui::Button("     BACK", [&] {(t == 1) ? IsEqualBus(data, path, name) : (t=t), gui::createCodeBus(administrator); });

	auto Enter = ftxui::Button("     ENTER", [&] {writeLocationBus(brand, model, god, regis, Numseats, path, data), EnterBusInfo(administrator, name), t = 1; });


	auto component = ftxui::Container::Vertical({ brandInput,backButton,Enter,modelInput,YearInput,RegistrationInput,SeatsInput });



	auto renderer = ftxui::Renderer(component, [&] {
		{



			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({vbox({
					center(hbox(brandInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(modelInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(YearInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(RegistrationInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(SeatsInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded})| borderRounded,
					hbox({center(hbox(Enter->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),
					center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green)))}),
					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);

}

//NOW YOU ARE DOING THIS!
void gui::createCodeBus(UserAccount& administrator)
{
	std::filesystem::path word1 = std::filesystem::current_path();
	word1 += "\\data\\codebooks";
	std::filesystem::create_directories(word1);
	word1 += "\\data.txt";
	std::string word;
	std::ifstream Data(word1);

	while (Data >> word)
	{
		CodeBook.push_back(word);
	};

	Data.close();

	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Bus";
	std::string name;
	std::ofstream data;
	std::string type;
	data.open(word1, std::ios::app);




	std::filesystem::path location3 = std::filesystem::current_path();
	location3 += "\\data\\codebooks";
	std::filesystem::create_directories(location3);
	location3 += "\\Bus.txt";

	std::ofstream location33;
	location33.open(location3, std::ios::app);

	auto backButton = ftxui::Button("BACK", [&] { gui::administrator_interface(administrator); });
	ftxui::Component usernameInput = ftxui::Input(&name, "Name codebook");

	int t = 0;
	auto Enter = ftxui::Button("ENTER", [&] {writeinFile(name, data), writeinFile(name, location33), EnterBusInfo(administrator, name), t = 1; });


	auto component = ftxui::Container::Vertical({ usernameInput,backButton,Enter });


	auto renderer = ftxui::Renderer(component, [&] {


		t = 0;
	std::string bannerMessage = "Bus";
	for (int i = 0; i < CodeBook.size(); i++)
	{
		if (name == CodeBook[i])
		{
			bannerMessageColor = red;
			t = 1;
		}
	};


	if (t == 0)
	{
		bannerMessageColor = blue;



		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(usernameInput->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(Enter->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
	}
	else
	{
		std::string bannerMessage = "Already Exist!";
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
		separatorDouble(), vbox({
			center(hbox(usernameInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) | borderRounded,
			center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
			}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);

	}
		}
	);


	screen.Loop(renderer);







}

int IsEqualTour(std::ofstream& data, std::filesystem::path path, std::string name)
{
	if (is_equalFile(name))
	{
		if (data.is_open())
			data.close();
		std::filesystem::remove(path);

		std::vector < std::string>CodeBooks2;

		std::filesystem::path word1 = std::filesystem::current_path();
		word1 += "\\data\\codebooks";
		std::filesystem::create_directories(word1);
		word1 += "\\data.txt";
		std::string word;
		std::ifstream Data(word1);

		while (Data >> word)
		{
			CodeBooks2.push_back(word);
		};
		Data.close();

		std::ofstream Data1;
		Data1.open(word1);
		auto it = std::find(CodeBooks2.begin(), CodeBooks2.end(), name);
		CodeBooks2.erase(it);
		for (int i = 0; i < CodeBooks2.size(); i++)
		{
			Data1 << CodeBooks2[i] << std::endl;
		}
		Data1.close();

		std::vector<std::string>Code;
		std::filesystem::path location3 = std::filesystem::current_path();
		location3 += "\\data\\codebooks";
		std::filesystem::create_directories(location3);
		location3 += "\\Tour.txt";

		std::ifstream location33;
		location33.open(location3);

		while (location33 >> word)
		{
			Code.push_back(word);
		};
		location33.close();

		std::ofstream Data2;
		Data2.open(location3);

		it = std::find(Code.begin(), Code.end(), name);
		Code.erase(it);
		for (int i = 0; i < Code.size(); i++)
		{
			Data2 << Code[i] << std::endl;
		}
		Data2.close();



	};

	return 1;
};


void writeTour(std::string Location, std::filesystem::path path, std::ofstream& data)
{
	data.open(path, std::ios::app);
	if (Location != "")
		data << Location << "#";
	data.close();
};

void EnterTourInfo(UserAccount& administrator, std::string name)
{
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Tour";
	std::string brand;

	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data\\codebooks\\";
	path += name;
	path += ".txt";
	std::ofstream data;


	ftxui::Component brandInput = ftxui::Input(&brand, "Enter destination");



	int t = 0;
	auto backButton = ftxui::Button("DONE", [&] {(t==1) ? IsEqualTour(data, path, name) : t=t, gui::createCodeTour(administrator); });//IS EQual
	auto brandButton = ftxui::Button("ENTER", [&] {writeTour(brand, path, data), EnterTourInfo(administrator, name), t = 1; });

	//auto Enter = ftxui::Button("Exit", [&] { EnterBusInfo(username, name), t = 0; });//ENTER


	auto component = ftxui::Container::Vertical({ brandInput,backButton,brandButton });



	auto renderer = ftxui::Renderer(component, [&] {
		{



			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({
					center(hbox(brandInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(brandButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),
					center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),

					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);

}

void gui::createCodeTour(UserAccount& administrator)
{



	std::filesystem::path word1 = std::filesystem::current_path();
	word1 += "\\data\\codebooks";
	std::filesystem::create_directories(word1);
	word1 += "\\data.txt";
	std::string word;
	std::ifstream Data(word1);

	while (Data >> word)
	{
		CodeBook.push_back(word);
	};

	Data.close();


	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Tour";
	std::string name;
	std::ofstream data;
	std::string type;
	data.open(word1, std::ios::app);




	std::filesystem::path location3 = std::filesystem::current_path();
	location3 += "\\data\\codebooks";
	std::filesystem::create_directories(location3);
	location3 += "\\Tour.txt";

	std::ofstream location33;
	location33.open(location3, std::ios::app);

	auto backButton = ftxui::Button("DONE", [&] { gui::administrator_interface(administrator); });
	ftxui::Component usernameInput = ftxui::Input(&name, "Name codebook");

	int t = 0;
	auto Enter = ftxui::Button("ENTER", [&] {writeinFile(name, data), writeinFile(name, location33), EnterTourInfo(administrator, name), t = 1; });//Napraviti tour info


	auto component = ftxui::Container::Vertical({ usernameInput,backButton,Enter });


	auto renderer = ftxui::Renderer(component, [&] {


		t = 0;
	std::string bannerMessage = "Tour";
	for (int i = 0; i < CodeBook.size(); i++)
	{
		if (name == CodeBook[i])
		{
			bannerMessageColor = red;
			t = 1;
		}
	};


	if (t == 0)
	{
		bannerMessageColor = blue;



		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(usernameInput->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(Enter->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
	}
	else
	{
		std::string bannerMessage = "Already exist!";
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
		separatorDouble(), vbox({
			center(hbox(usernameInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
			center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
			}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);

	}
		}
	);


	screen.Loop(renderer);



};


int DeleteFile1(std::string name)
{
	std::filesystem::path path = std::filesystem::current_path();

	path += "\\data\\codebooks\\";
	path += name;
	path += ".txt";


	std::filesystem::remove(path);

	std::filesystem::path word1 = std::filesystem::current_path();
	word1 += "\\data\\codebooks";
	std::filesystem::create_directories(word1);
	word1 += "\\data.txt";

	std::filesystem::path word2 = std::filesystem::current_path();
	word2 += "\\data\\codebooks";
	std::filesystem::create_directories(word2);
	word2 += "\\location.txt";

	std::filesystem::path word3 = std::filesystem::current_path();
	word3 += "\\data\\codebooks";
	std::filesystem::create_directories(word3);
	word3 += "\\Bus.txt";

	std::filesystem::path word4 = std::filesystem::current_path();
	word4 += "\\data\\codebooks";
	std::filesystem::create_directories(word4);
	word4 += "\\Tour.txt";

	std::vector<std::string> CodeBooks2;
	std::vector<std::string> CodeBooks3;
	std::vector<std::string> CodeBooks4;
	std::vector<std::string> CodeBooks5;

	std::string word;
	std::ifstream Data(word1);

	while (Data >> word)
	{
		CodeBooks2.push_back(word);
	};
	Data.close();
	Data.open(word2);

	while (Data >> word)
	{
		CodeBooks3.push_back(word);
	};
	Data.close();

	Data.open(word3);
	while (Data >> word)
	{
		CodeBooks4.push_back(word);
	};

	Data.close();

	Data.open(word4);
	while (Data >> word)
	{
		CodeBooks5.push_back(word);
	};


	auto n = std::find(CodeBooks2.begin(), CodeBooks2.end(), name);

	if (n != CodeBooks2.end())
	{
		CodeBooks2.erase(n);
	};
	n = std::find(CodeBooks3.begin(),
		CodeBooks3.end(), name);

	if (n != CodeBooks3.end())
	{
		CodeBooks3.erase(n);
	};
	n = std::find(CodeBooks4.begin(), CodeBooks4.end(), name);
	if (n != CodeBooks4.end())
	{
		CodeBooks4.erase(n);
	};
	n = std::find(CodeBooks4.begin(), CodeBooks4.end(), name);
	if (n != CodeBooks4.end())
	{
		CodeBooks4.erase(n);
	};
	n = std::find(CodeBooks5.begin(), CodeBooks5.end(), name);
	if (n != CodeBooks5.end())
	{
		CodeBooks5.erase(n);
	};

	std::ofstream DATA;
	DATA.open(word1);

	for (int i = 0; i < CodeBooks2.size(); i++)
	{
		DATA << CodeBooks2[i] << std::endl;
	};
	DATA.close();

	DATA.open(word2);
	for (int i = 0; i < CodeBooks3.size(); i++)
	{
		DATA << CodeBooks3[i] << std::endl;
	};
	DATA.close();

	DATA.open(word3);
	for (int i = 0; i < CodeBooks4.size(); i++)
	{
		DATA << CodeBooks4[i] << std::endl;
	};
	DATA.close();

	DATA.open(word4);
	for (int i = 0; i < CodeBooks5.size(); i++)
	{
		DATA << CodeBooks5[i] << std::endl;
	};
	DATA.close();
	return 1;
};

void gui::DeleteCodeBooks(UserAccount& administrator)
{
	std::filesystem::path word1 = std::filesystem::current_path();
	word1 += "\\data\\codebooks";
	std::filesystem::create_directories(word1);
	word1 += "\\data.txt";
	std::string word;
	std::ifstream Data(word1);

	std::vector<std::string> CodeBooks2;

	while (Data >> word)
	{
		CodeBooks2.push_back(word);
	};

	Data.close();

	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Choose file!";

	int selected = -1;
	auto menu = Radiobox(&CodeBooks2, &selected);

	auto Enter = ftxui::Button("ENTER", [&] { DeleteFile1(CodeBooks2[selected]), gui::createCodeBooksInterface(administrator); });
	auto Back = ftxui::Button("BACK", [&] { gui::createCodeBooksInterface(administrator); });
	

	auto component = ftxui::Container::Vertical({ menu,Enter,Back });


	auto renderer = ftxui::Renderer(component, [&] {
		{



			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({
					center(hbox(menu->Render())),
					hbox({
					center(hbox(Enter->Render() | size(WIDTH, EQUAL, 10) | ftxui::color(bright_green))),
					center(hbox(Back->Render() | size(WIDTH,EQUAL, 10) | ftxui::color(bright_green)))})

					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);

};


void EnterLocationModify(std::string name, UserAccount& administrator)
{
	
	std::string location;
	std::string country;
	std::ofstream data;
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Add Location!";

	std::filesystem::path path = std::filesystem::current_path();

	path += "\\data\\codebooks\\";
	path += name;
	path += ".txt";

	ftxui::Component locationInput = ftxui::Input(&location, "Enter city");
	ftxui::Component countryInput = ftxui::Input(&country, "Enter country");

	int t = 0;
	auto backButton = ftxui::Button("BACK", [&] { gui::LocationModify(administrator); });

	auto Enter = ftxui::Button("ENTER", [&] {writeLocation(country, location, path, data), EnterLocationModify(name,administrator), t = 0; });


	auto component = ftxui::Container::Vertical({ locationInput,backButton,Enter,countryInput });



	auto renderer = ftxui::Renderer(component, [&] {
		{
			

			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({
					center(hbox(locationInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(countryInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
					center(hbox(Enter->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);


};


void gui::LocationModify(UserAccount& administrator)
{
	std::filesystem::path word1 = std::filesystem::current_path();
	word1 += "\\data\\codebooks";
	std::filesystem::create_directories(word1);
	word1 += "\\location.txt";
	std::string word;
	std::ifstream Data(word1);

	std::vector<std::string> CodeBooks2;

	while (Data >> word)
	{
		CodeBooks2.push_back(word);
	};

	Data.close();

	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Choose file!";

	int selected = -1;
	auto menu = Radiobox(&CodeBooks2, &selected);

	auto Enter = ftxui::Button("ENTER", [&] { EnterLocationModify(CodeBooks2[selected], administrator), gui::createCodeBooksInterface(administrator); });
	auto Back = ftxui::Button("BACK", [&] { gui::createCodeBooksInterface(administrator); });



	auto component = ftxui::Container::Vertical({ menu,Enter,Back });


	auto renderer = ftxui::Renderer(component, [&] {
		{

			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({
					center(hbox(menu->Render())),
					hbox({
					center(hbox(Enter->Render() | size(WIDTH, EQUAL, 10) | ftxui::color(bright_green))),
					center(hbox(Back->Render() | size(WIDTH,EQUAL, 10) | ftxui::color(bright_green)))})

					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);

};



void EnterBusModify(std::string name, UserAccount& administrator)
{

	std::ofstream data;
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Add Location!";
	std::string brand, model, god, regis, Numseats;

	std::filesystem::path path = std::filesystem::current_path();

	path += "\\data\\codebooks\\";
	path += name;
	path += ".txt";


	ftxui::Component brandInput = ftxui::Input(&brand, "Enter brand");
	ftxui::Component modelInput = ftxui::Input(&model, "Enter model");
	ftxui::Component YearInput = ftxui::Input(&god, "Enter year of production");
	ftxui::Component RegistrationInput = ftxui::Input(&regis, "Enter registration");
	ftxui::Component SeatsInput = ftxui::Input(&Numseats, "Enter number of seats");


	int t = 0;
	auto backButton = ftxui::Button("BACK", [&] { gui::BusModify(administrator); });

	auto Enter = ftxui::Button("ENTER", [&] {writeLocationBus(brand, model, god, regis, Numseats, path, data), EnterBusModify(name, administrator), t = 0; });


	auto component = ftxui::Container::Vertical({ brandInput,backButton,Enter,modelInput,YearInput,RegistrationInput,SeatsInput });



	auto renderer = ftxui::Renderer(component, [&] {
		{

			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({vbox({
					center(hbox(brandInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(modelInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(YearInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(RegistrationInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(SeatsInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded})| borderRounded,
					center(hbox(Enter->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),
					center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),
					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});



	screen.Loop(renderer);

};

void gui::BusModify(UserAccount& administrator)
{
	std::filesystem::path word1 = std::filesystem::current_path();
	word1 += "\\data\\codebooks";
	std::filesystem::create_directories(word1);
	word1 += "\\Bus.txt";
	std::string word;
	std::ifstream Data(word1);

	std::vector<std::string> CodeBooks2;

	while (Data >> word)
	{
		CodeBooks2.push_back(word);
	};

	Data.close();

	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Choose file!";

	int selected = -1;
	auto menu = Radiobox(&CodeBooks2, &selected);

	auto Enter = ftxui::Button("ENTER", [&] { EnterBusModify(CodeBooks2[selected], administrator), gui::createCodeBooksInterface(administrator); });

	auto Back = ftxui::Button("BACK", [&] { gui::createCodeBooksInterface(administrator); });



	auto component = ftxui::Container::Vertical({ menu,Enter,Back });


	auto renderer = ftxui::Renderer(component, [&] {
		{

			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({
					center(hbox(menu->Render())),
					hbox({
					center(hbox(Enter->Render() | size(WIDTH, EQUAL, 10) | ftxui::color(bright_green))),
					center(hbox(Back->Render() | size(WIDTH,EQUAL, 10) | ftxui::color(bright_green)))})

					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);

};


void EnterTourModify(std::string name, UserAccount& administrator)
{
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Tour";
	std::string brand;

	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data\\codebooks\\";
	path += name;
	path += ".txt";
	std::ofstream data;


	ftxui::Component brandInput = ftxui::Input(&brand, "Enter destination");



	int t = 0;
	auto backButton = ftxui::Button("DONE", [&] {gui::TourModify(administrator); });//IS EQual
	auto brandButton = ftxui::Button("ENTER", [&] {writeTour(brand, path, data), EnterTourModify(name,administrator), t = 0; });

	


	auto component = ftxui::Container::Vertical({ brandInput,backButton,brandButton });



	auto renderer = ftxui::Renderer(component, [&] {
		{



			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({
					center(hbox(brandInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(bright_green))) | borderRounded,
					center(hbox(brandButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),
					center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green))),

					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);
}

void gui::TourModify(UserAccount& administrator)
{
	std::filesystem::path word1 = std::filesystem::current_path();
	word1 += "\\data\\codebooks";
	std::filesystem::create_directories(word1);
	word1 += "\\Tour.txt";
	std::string word;
	std::ifstream Data(word1);

	std::vector<std::string> CodeBooks2;

	while (Data >> word)
	{
		CodeBooks2.push_back(word);
	};

	Data.close();

	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Choose file!";

	int selected = -1;
	auto menu = Radiobox(&CodeBooks2, &selected);

	auto Enter = ftxui::Button("ENTER", [&] { EnterTourModify(CodeBooks2[selected], administrator), gui::createCodeBooksInterface(administrator); });

	auto Back = ftxui::Button("BACK", [&] { gui::createCodeBooksInterface(administrator); });



	auto component = ftxui::Container::Vertical({ menu,Enter,Back });


	auto renderer = ftxui::Renderer(component, [&] {
		{

			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({
					center(hbox(menu->Render())),
					hbox({
					center(hbox(Enter->Render() | size(WIDTH, EQUAL, 10) | ftxui::color(bright_green))),
					center(hbox(Back->Render() | size(WIDTH,EQUAL, 10) | ftxui::color(bright_green)))})

					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);
};

void gui::ModifyCodeBooks(UserAccount& administrator)
{
	
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Choose type of file!";

	std::vector<std::string>CodeBooks2 = {"Location","Bus","Tour"};
	int selected = -1;

	auto Location = ftxui::Button("Location", [&] {gui::LocationModify(administrator), gui::createCodeBooksInterface(administrator); });
	auto Bus= ftxui::Button(   "  Bus", [&] {gui::BusModify(administrator), gui::createCodeBooksInterface(administrator); });
	auto Tour = ftxui::Button("  Tour", [&] {gui::TourModify(administrator), gui::createCodeBooksInterface(administrator); });
	auto Back = ftxui::Button("            BACK", [&] { gui::createCodeBooksInterface(administrator); });


	auto component = ftxui::Container::Vertical({Location,Bus,Tour,Back });


	auto renderer = ftxui::Renderer(component, [&] {
		{

			return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
				separatorDouble(), vbox({
					hbox({
					center(hbox(Location->Render() | size(WIDTH, EQUAL, 10) | ftxui::color(bright_green))),
					center(hbox(Bus->Render() | size(WIDTH, EQUAL, 10) | ftxui::color(bright_green))),
					center(hbox(Tour->Render() | size(WIDTH, EQUAL, 10) | ftxui::color(bright_green))),
					}),center(hbox(Back->Render() | size(WIDTH,EQUAL, 30) | ftxui::color(bright_green)))

					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);


};


void gui::createCodeBooksInterface(UserAccount& administrator)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Choise type of codebook";
	ftxui::Color bannerMessageColor = blue;
	auto Location = ftxui::Button("Create location codebook", [&] {createCodeLocation(administrator); });//Dodati interfejs
	auto Bus = ftxui::Button("Create bus codebook", [&] {createCodeBus(administrator); });//Dodati interfejs
	auto Tour = ftxui::Button("Create tour codebook", [&] {createCodeTour(administrator); });//Dodati interfejs
	auto Delete = ftxui::Button("Delete codebook", [&] {gui::DeleteCodeBooks(administrator); });//Dodati interfejs
	auto Modify = ftxui::Button("Modify codebooks", [&] {ModifyCodeBooks(administrator); });//Dodati interfejs
	auto Back = ftxui::Button("BACK", [&] {gui::administrator_interface(administrator); });//Dodati interfejs
	auto component = ftxui::Container::Vertical({ Location,  Bus, Tour,Delete,Modify,Back });

	//Buttons
	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(),
		   center(hbox(center(Location->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))),
		   center(hbox(Bus->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray))),
		   center(hbox(Tour->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray))),
		   center(hbox(Delete->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray))),
			center(hbox(Modify->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray))),
		   center(hbox(Back->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});
	screen.Loop(renderer);
};