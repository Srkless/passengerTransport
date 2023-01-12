#include "gui.h"

void gui::accountSettingsInterface()
{
	std::string bannerMessage = "Account Settings";
	ftxui::Color bannerMessageColor = blue;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	// Account Settings
	auto createAccount = ftxui::Button("Create account", [&] {gui::register_interface(1);});
	auto activateAccount = ftxui::Button("Activate account", [&] {exit(0); });
	auto suspendAccount = ftxui::Button("Suspend account", [&] {exit(0); });
	auto deleteAccount = ftxui::Button("Delete account", [&] {exit(0); });
	auto viewAccounts = ftxui::Button("View accounts", [&] {exit(0); });

	auto suspendPassword = ftxui::Button("Suspend password", [&] {exit(0); });
	auto changePassword = ftxui::Button("Change password", [&] {exit(0); });
	auto backButton = ftxui::Button("Back", [&] {gui::administrator_interface(); });

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
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))}) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);;
		});

	screen.Loop(renderer);
}
