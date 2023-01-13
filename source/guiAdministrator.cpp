#include "gui.h"

void choiceAccountInterface(UserAccount& administrator)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Account types";
	ftxui::Color bannerMessageColor = blue;
	auto createAdministratorAccount = ftxui::Button("Create administrator account", [&] {gui::registerInterface(1); });
	auto createDriverAccount = ftxui::Button("Create driver account", [&] {gui::registerInterface(2); });
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

void addRideInterface() // TODO
{
	exit(0);
}
void deleteRideInterface() // TODO
{
	exit(0);
}

void gui::scheduleSettings(UserAccount& administrator)
{
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = administrator.getUsername() + "'s account Settings";

	auto addRide = ftxui::Button("Add ride", [&] {addRideInterface(); });
	auto deleteRide = ftxui::Button("Delete ride", [&] {deleteRideInterface(); });
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
				(selected != -1 && flag == 1) ? (hbox(text(reportDatabase[entries[selected]].getContent()) | size(WIDTH, GREATER_THAN, 10) | ftxui::color(white))) : (hbox() | size(WIDTH, GREATER_THAN, 10))})})
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

	auto backButton = ftxui::Button("Back", [&] {gui::reportsSettings(administrator); });
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


//
//inline bool is_equal(std::string name)
//{
//    std::filesystem::path path = std::filesystem::current_path();
//    path += "\\data\\codebooks\\";
//    path += name;
//    path += ".txt";
//    int i = 0;
//    std::vector<std::string> strings;
//    std::vector<std::string> strings2;
//    std::string word, word2;
//    std::ifstream file, file2;
//
//    if (CodeBook.size() == 0)
//        return false;
//
//    while (i < CodeBook.size())
//    {
//        int check2 = 0;
//
//        std::filesystem::path path1 = std::filesystem::current_path();
//        path1 += "\\data\\codebooks\\";
//        path1 += CodeBook[i++];
//        path1 += ".txt";
//        file2.open(path1);
//
//        while (getline(file2, word))
//        {
//            strings.push_back(word);
//        }
//        file2.close();
//
//
//        file.open(path);
//
//        while (getline(file, word2))
//        {
//            strings2.push_back(word2);
//        }
//
//        file.close();
//
//
//
//
//        int check = 0;
//        for (int j = 0; j < strings.size(); j++)
//        {
//
//            word = strings2[j];
//
//            if (strings.size() != strings2.size())
//                break;
//
//
//            for (int k = 0; k < strings.size(); k++)
//            {
//                if (word == strings[k])
//                    check++;
//            }
//            if (check != 0)
//                check2++;
//
//            check = 0;
//        }
//
//        if (check2 == strings2.size())
//        {
//            return true;
//        };
//
//        check2 = 0;
//
//    };
//
//    return false;
//}
void writeLocation(std::string name, std::string country, std::ofstream& data)
{
	data << name << "#" << country << std::endl;
};

void writeinFile(std::string name, std::ofstream& data)
{
	data << name << std::endl;
};



void gui::EnterLocation(std::string username, std::string name)
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
	data.open(path, std::ios::app);

	ftxui::Component locationInput = ftxui::Input(&location, "Enter City");
	ftxui::Component countryInput = ftxui::Input(&country, "Enter Country");

	int t = 0;
	auto backButton = ftxui::Button("Back", [&] { gui::createCodeLocation(username); });//Dodaj konju

	auto Enter = ftxui::Button("Enter", [&] {writeLocation(country, location, data), gui::EnterLocation(username, name), t = 0; });


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
					center(hbox(locationInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
					center(hbox(countryInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
					center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
					center(hbox(Enter->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
					}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		}});


	screen.Loop(renderer);


};

void gui::createCodeLocation(std::string username)
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

	auto backButton = ftxui::Button("Back", [&] { /*gui::administrator_interface(username);*/ });
	ftxui::Component usernameInput = ftxui::Input(&name, "Name codebook");
	/*
	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data\\codebooks\\";
	std::ofstream file;
	path += ".txt";
	file.open(path, std::ios::app);*/
	int t = 0;
	auto Enter = ftxui::Button("Enter", [&] {writeinFile(name, data), writeinFile(name, location33), EnterLocation(username, name), t = 1; });


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
				center(hbox(usernameInput->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				center(hbox(Enter->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))
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
void createCodeBus()
{
	exit(0);
}
void createCodeTour()
{
	exit(0);
};
void gui::createCodeBooksInterface(std::string username)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Choise type of Codebook";
	ftxui::Color bannerMessageColor = blue;
	auto Location = ftxui::Button("Location", [&] {createCodeLocation(username); });//Dodati interfejs
	auto Bus = ftxui::Button("Bus", [&] {createCodeBus(); });//Dodati interfejs
	auto Tour = ftxui::Button("Tour", [&] {createCodeTour(); });//Dodati interfejs
	auto Back = ftxui::Button("Back", [&] {/*gui::administrator_interface(username);*/ });//Dodati interfejs
	auto component = ftxui::Container::Vertical({ Location,  Bus, Tour,Back });

	//Buttons
	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(),
		   center(hbox(center(Location->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))),
		   center(hbox(Bus->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray))),
		   center(hbox(Tour->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray))),
		   center(hbox(Back->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});
	screen.Loop(renderer);
};

