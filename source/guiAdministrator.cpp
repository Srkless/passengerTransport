#include "gui.h"

void choiceAccountInterface(std::string username)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Choise account";
	ftxui::Color bannerMessageColor = blue;
	auto createAdministratorAccount = ftxui::Button("Create administrator account", [&] {gui::register_interface(1); });
	auto createDriverAccount = ftxui::Button("Create driver account", [&] {gui::register_interface(2); });
	auto backButton = ftxui::Button("Back", [&] {gui::accountSettingsInterface(username); });
	auto component = ftxui::Container::Vertical({ createAdministratorAccount,  createDriverAccount, backButton });
	
	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(),
		   center(hbox(center(createAdministratorAccount->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))),
		   center(hbox(center(createDriverAccount->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray)))),
		   center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))) })| hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
};

void editAccountInterface(int value, std::string currUsername)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = currUsername + "'s account settings";
	ftxui::Color bannerMessageColor = blue;
	std::unordered_map<std::string, UserAccount> userDatabase;
	std::vector<std::string> entries;
	userDatabase = db::loadUsersFromFile();
	for (auto& elem : userDatabase)
	{
		if(value == 1)
		{
			if (elem.second.getSuspendInfo() && elem.second.getUsername() != currUsername)
				entries.push_back(elem.first);
		}
		else if (value == 2)
		{
			if (!elem.second.getSuspendInfo() && elem.second.getUsername() != currUsername)
				entries.push_back(elem.first);
		}
		else if(elem.second.getUsername() != currUsername)
		{
			entries.push_back(elem.first);
		}
	}		

	int selected = -1;
	auto menu = Radiobox(&entries, &selected);
	auto acceptButton = ftxui::Button("Accept", [&] {(value == 3 && selected != -1) ? (userDatabase.erase(entries[selected]), db::writeUsersToFile(userDatabase), gui::accountSettingsInterface(currUsername))
			: (value == 4 && selected != -1) ? (userDatabase[entries[selected]].setPassword("admin"), db::writeUsersToFile(userDatabase), gui::accountSettingsInterface(currUsername)): (userDatabase[entries[selected]].changeSuspensionStatus(), db::writeUsersToFile(userDatabase), gui::accountSettingsInterface(currUsername)); });
	auto backButton = ftxui::Button("Back", [&] {gui::accountSettingsInterface(currUsername); });
	auto component = ftxui::Container::Vertical({ menu,acceptButton, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		
		if(value > 0 && entries.size() != 0)
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


void gui::accountSettingsInterface(std::string username)
{
	
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string currUsername = username;
	std::string bannerMessage = currUsername + "'s account Settings";

	// Account Settings
	auto createAccount = ftxui::Button("Create account", [&] {choiceAccountInterface(username); });
	auto activateAccount = ftxui::Button("Activate account", [&] {editAccountInterface(1, username); });
	auto suspendAccount = ftxui::Button("Suspend account", [&] {editAccountInterface(2, username); });
	auto deleteAccount = ftxui::Button("Delete account", [&] {editAccountInterface(3, username); });
	auto viewAccounts = ftxui::Button("View accounts", [&] {editAccountInterface(0, username); });

	auto suspendPassword = ftxui::Button("Suspend password", [&] {editAccountInterface(4, username); });
	auto changePassword = ftxui::Button("Change password", [&] {gui::changePassword(username); });
	auto backButton = ftxui::Button("Back", [&] {gui::administrator_interface(username); });

	

	auto component = ftxui::Container::Vertical({ createAccount, activateAccount, suspendAccount, deleteAccount, viewAccounts, suspendPassword, changePassword, backButton});

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
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))})}) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});

	screen.Loop(renderer);
}
