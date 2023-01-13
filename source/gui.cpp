#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "gui.h"
#include "utility.h"
#include "database.h"
#include <fstream>

#define loginNums 20

int pressed = 0;

void gui::registerInterface(int number)
{
	std::string username;
	std::string password;
	std::string againPassword;
	int wrongPassword = 0;
	int wrongUsername = 0;

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	std::string bannerMessage = "SOFTWARE FOR A PASSENGER TRANSPORT AGENCY";

	ftxui::Color bannerMessageColor = blue;
	ftxui::Color inputUsernameColor = light_gray;
	ftxui::Color inputPasswordColor = light_gray;

	ftxui::Component inputUsername = ftxui::Input(&username, "Username");

	ftxui::InputOption passwordOption;
	passwordOption.password = true;
	ftxui::Component inputPassword = ftxui::Input(&password, "Password", passwordOption);
	ftxui::Component inputPasswordAgain = ftxui::Input(&againPassword, "Password again", passwordOption);

	std::unordered_map<std::string, UserAccount> userDatabase;
	userDatabase = db::loadUsersFromFile();

	std::vector<std::string> admins = Utility::returnAdmins();

	int flag = 0;
	auto registerButton = ftxui::Button("Register", [&] {
		flag = 1;
	if (flag && userDatabase[username].getUsername() == "" && password == againPassword && password.size() > 7) {
		bannerMessage = "Successful registration!";
		bannerMessageColor = bright_green;
		wrongPassword = 0;
		wrongUsername = 0;
		std::string currUsername = userDatabase[username].getUsername();
		if (number == 1)
		{
			UserAccount curr(username, password, "administrator", 0);
			curr.changeSuspensionStatus();
			db::addUserToFile(curr);
			administrator_interface(userDatabase[currUsername]);
		}
		else if (number == 2)
		{
			UserAccount curr(username, password, "driver", 0);
			curr.changeSuspensionStatus();
			db::addUserToFile(curr);
			administrator_interface(userDatabase[currUsername]);
		}
		else
		{
			UserAccount curr(username, password, "user", 0);
			for (auto& admin : admins)
				userDatabase[admin].changeNotificationAlert();
			userDatabase[username] = curr;
			userDatabase[username].changeNotificationAlert();
			db::writeUsersToFile(userDatabase);
			loginInterface();
		}
	}
	else
	{
		bannerMessage = "Failed registration!";
		bannerMessageColor = red;
		if (userDatabase[username].getUsername() != "")
		{
			inputUsernameColor = red;
			wrongUsername = 1;
		}
		else
		{
			inputUsernameColor = bright_green;
			wrongUsername = 0;
		}

		if (password != againPassword || password.size() < 8)
		{
			inputPasswordColor = red;
			wrongPassword = 1;
		}
		else
		{
			inputPasswordColor = bright_green;
			wrongPassword = 0;
		}
	}
		});
	auto exitButton = ftxui::Button("EXIT", [&] { exit(0); });
	auto backButton = ftxui::Button("BACK", [&] {gui::loginInterface(); });
	auto component = ftxui::Container::Vertical({ inputUsername, inputPassword, inputPasswordAgain, registerButton, exitButton, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		if (userDatabase[username].getUsername() == "" && password == againPassword && password.size() > 7)
		{
			inputUsernameColor = bright_green;
			inputPasswordColor = bright_green;
			wrongPassword = 0;
		}

	return  vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
			separatorDouble(),
			center(hbox(center(inputUsername->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(inputUsernameColor)))) | ftxui::borderRounded,
			center(hbox(center(inputPassword->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(inputPasswordColor)))) | ftxui::borderRounded,
			center(hbox(hbox(center(inputPasswordAgain->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(inputPasswordColor))))) | ftxui::borderRounded,
			center(hbox({ftxui::hbox({center(registerButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(bright_green),
			center(hbox(center(backButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(dark_green))) })})),
			center(hbox(center(exitButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });

	auto agreePasswordButton = [&]() { wrongPassword = 0; wrongUsername = 0; };
	auto wrongPasswoedContainer = Container::Horizontal({ Button("Retry", [&] { agreePasswordButton(); }) });
	auto wrongPasswoedRederer = Renderer(wrongPasswoedContainer, [&] {
		return vbox({
				   text("Passwords do not match or are less than 8 characters"),
				   separator(),
				   center(hbox(wrongPasswoedContainer->Render())) | color(red),
			}) |
			border;
		});

	auto agreeUsernameButton = [&]() { wrongUsername = 0; wrongPassword = 0; };
	auto wrongUsernameContainer = Container::Horizontal({ Button("Retry", [&] { agreeUsernameButton(); }) });
	auto wrongUsernameRenderer = Renderer(wrongUsernameContainer, [&] {
		return vbox({
				   text("Username already exists!"),
				   separator(),
				   center(hbox(wrongUsernameContainer->Render())) | color(red),
			}) |
			border;
		});

	auto mainPasswordContainer = Container::Tab({ renderer, wrongPasswoedRederer }, &wrongPassword);
	auto mainUsernameContainer = Container::Tab({ renderer, wrongUsernameRenderer }, &wrongUsername);
	auto mainContainer = Container::Vertical({ mainPasswordContainer, mainUsernameContainer });

	auto mainRenderer = Renderer(mainContainer, [&] {
		Element document = renderer->Render();

	if (wrongPassword == 1) {
		document = dbox({
			document,
			wrongPasswoedRederer->Render() | clear_under | center,
			});
	}
	else if (wrongUsername == 1) document = dbox({
			document,
			wrongUsernameRenderer->Render() | clear_under | center,
		});
	return document;
		});

	screen.Loop(mainRenderer);
}

void gui::loginInterface()
{
	std::string username;
	std::string password;

	std::string bannerMessage = "SOFTWARE FOR A PASSENGER TRANSPORT AGENCY";
	int wrongPassword = 0;
	int wrongUsername = 0;
	int suspended = 0;

	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data";
	std::filesystem::create_directories(path);
	path += "\\config.txt";

	std::ifstream config(path);

	int configNum;
	config >> configNum;

	config.close();

	std::unordered_map<std::string, UserAccount> userDatabase;
	userDatabase = db::loadUsersFromFile();

	ftxui::Component usernameInput = ftxui::Input(&username, "Username");

	ftxui::InputOption passwordOption;
	passwordOption.password = true;
	ftxui::Component passwordInput = ftxui::Input(&password, "Password", passwordOption);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	ftxui::Color bannerMessageColor = blue;
	ftxui::Color usernameColor = light_gray;
	ftxui::Color passwordColor = light_gray;

	auto logInButton = ftxui::Button("SIGN IN", [&] {
		if (userDatabase[username].getUsername() != "" && Utility::decrypt(userDatabase[username].getPassword()) == Utility::decrypt(password) && !userDatabase[username].getSuspendInfo()) {
			bannerMessage = "Successful login";
			bannerMessageColor = bright_green;
			wrongPassword = 0;
			wrongUsername = 0;
			if (userDatabase[username].getUsername() == "admin" && configNum == 0)
			{
				std::ofstream config(path);
				config << 1;
				config.close();
				changePassword(userDatabase[username].getUsername());
			}
			if (userDatabase[username].getNumOfLogins() >= loginNums)
			{
				changePassword(userDatabase[username].getUsername());
				userDatabase[username].resetNumOfLogins();
			}
			else
			{
				userDatabase[username].increaseNumOfLogins();
				db::writeUsersToFile(userDatabase);
			}
			if (userDatabase[username].getAccountType() == "administrator")
				administrator_interface(userDatabase[username]);
		}
		else
		{
			bannerMessage = "Failed login!";
			if (userDatabase[username].getUsername() == "")
			{
				usernameColor = red;
				wrongUsername = 1;
			}
			else if (Utility::decrypt(userDatabase[username].getPassword()) != Utility::decrypt(password))
			{
				passwordColor = red;
				wrongPassword = 1;
			}
			else if (userDatabase[username].getSuspendInfo())
				suspended = 1;
		}

		});

	auto exitButton = ftxui::Button("EXIT", [&] { exit(0); });
	auto registerButton = ftxui::Button("SIGN UP", [&] {registerInterface(0); });
	auto component = ftxui::Container::Vertical({ usernameInput, passwordInput, logInButton, exitButton, registerButton });

	auto renderer = ftxui::Renderer(component, [&] {

		if (userDatabase[username].getUsername() != "" && Utility::decrypt(userDatabase[username].getPassword()) == Utility::decrypt(password)) { // Provjera da li posotji username ako jeste provjerava da li je lozinka tacna
			usernameColor = bright_green;
			passwordColor = bright_green;
			wrongPassword = 0;
			wrongUsername = 0;
		}
		else
		{
			usernameColor = light_gray;
			passwordColor = light_gray;
		}
	return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(),
			ftxui::hbox({vbox({center(ftxui::hbox(ftxui::text(""), usernameInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(usernameColor)))}) | ftxui::borderRounded,
			center(hbox(text(""), passwordInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(passwordColor))) | ftxui::borderRounded}),
			center(hbox({hbox({center(logInButton->Render()) | size(WIDTH, EQUAL, 9) | ftxui::color(bright_green),
			center(hbox(center(exitButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) })})),
			(configNum == 1) ? (center(hbox(center(registerButton->Render()) | size(WIDTH, EQUAL, 15) | ftxui::color(orange)))) : (center(hbox())) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });


	auto agreeUsernameButton = [&]() { wrongUsername = 0; wrongPassword = 0; suspended = 0; };
	auto wrongUsernameContainer = Container::Horizontal({ Button("Retry", [&] { agreeUsernameButton(); }) });
	auto wrongUsernameRenderer = Renderer(wrongUsernameContainer, [&] {
		return vbox({
				   text("Username incorrect!"),
				   separator(),
				   center(hbox(wrongUsernameContainer->Render())) | color(red),
			}) |
			border;
		});

	auto agreePasswordButton = [&]() { wrongPassword = 0; wrongUsername = 0; suspended = 0; };
	auto wrongPasswordContainer = Container::Horizontal({ Button("Retry", [&] { agreePasswordButton(); }) });
	auto wrongPasswordRederer = Renderer(wrongPasswordContainer, [&] {
		return vbox({
				   text("Incorrect password!"),
				   separator(),
				   center(hbox(wrongPasswordContainer->Render())) | color(red),
			}) |
			border;
		});
	auto agreeSuspended = [&]() { wrongUsername = 0; wrongPassword = 0; suspended = 0; };
	auto suspendedContainer = Container::Horizontal({ Button("Retry", [&] { agreeSuspended(); }) });
	auto suspendedRederer = Renderer(suspendedContainer, [&] {
		return vbox({
				   text("This account has been suspended"),
				   separator(),
				   center(hbox(wrongPasswordContainer->Render())) | color(red),
			}) |
			border;
		});

	auto mainPasswordContainer = Container::Tab({ renderer, wrongPasswordRederer }, &wrongPassword);
	auto mainUsernameContainer = Container::Tab({ renderer, wrongUsernameRenderer }, &wrongUsername);
	auto mainSuspendContainer = Container::Tab({ renderer, suspendedRederer }, &suspended);
	auto mainContainer = Container::Vertical({ mainPasswordContainer, mainUsernameContainer,mainSuspendContainer });

	auto mainRenderer = Renderer(mainContainer, [&] {
		Element document = renderer->Render();

	if (wrongUsername == 1)
		document = dbox({ document, wrongUsernameRenderer->Render() | clear_under | center, });
	else if (wrongPassword == 1)
		document = dbox({ document, wrongPasswordRederer->Render() | clear_under | center, });
	else if (suspended == 1)
		document = dbox({ document, suspendedRederer->Render() | clear_under | center, });
	return document;
		});

	screen.Loop(mainRenderer);

}

void gui::changePassword(std::string username)
{
	std::string oldPassword;
	std::string password;
	std::string confirmPassword;
	ftxui::Color passwordColor = light_gray;
	int passwordControl = 0; // 0 - sve okej, -1 stara sifra ne valja, 1 ne valja nova

	std::string bannerMessage = "Password change";
	ftxui::Color bannerMessageColor = light_gray;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	ftxui::InputOption passwordOption;
	passwordOption.password = true;

	ftxui::Component oldPasswordInput = ftxui::Input(&oldPassword, "Old password", passwordOption);
	ftxui::Component passwordInput = ftxui::Input(&password, "New password", passwordOption);
	ftxui::Component confirmPasswordInput = ftxui::Input(&confirmPassword, "Confirm password", passwordOption);

	std::unordered_map<std::string, UserAccount> userDatabase;
	userDatabase = db::loadUsersFromFile();

	auto confirmButton = ftxui::Button("CONFIRM", [&] {
		if (Utility::decrypt(userDatabase[username].getPassword()) == Utility::decrypt(oldPassword) && Utility::decrypt(password) == Utility::decrypt(confirmPassword) && Utility::decrypt(oldPassword) != Utility::decrypt(confirmPassword))
		{
			bannerMessage = "Successful password change";
			passwordColor = bright_green;
			bannerMessageColor = bright_green;
			passwordControl = 0;
			userDatabase[username].setPassword(confirmPassword);
			userDatabase[username].resetNumOfLogins();
			db::writeUsersToFile(userDatabase);
			loginInterface();
		}
		else
		{
			bannerMessage = "Failed password change";
			bannerMessageColor = red;
			passwordColor = red;
			if (Utility::decrypt(userDatabase[username].getPassword()) != Utility::decrypt(oldPassword))
				passwordControl = -1;
			else if (password != confirmPassword)
				passwordControl = 1;
			else if (Utility::decrypt(oldPassword) == Utility::decrypt(confirmPassword))
				passwordControl = 2;

		}
		});
	auto exitButton = ftxui::Button("EXIT", [&] { exit(0); });

	auto component = ftxui::Container::Vertical({ oldPasswordInput, passwordInput, confirmPasswordInput , confirmButton, exitButton });

	auto renderer = ftxui::Renderer(component, [&] {

		if (Utility::decrypt(userDatabase[username].getPassword()) == Utility::decrypt(oldPassword) && Utility::decrypt(password) == Utility::decrypt(confirmPassword) && Utility::decrypt(oldPassword) != Utility::decrypt(confirmPassword)) {
			passwordColor = bright_green;
			bannerMessageColor = bright_green;
			passwordControl = 0;
		}
		else
		{
			passwordColor = light_gray;
		}
	return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(),
			ftxui::hbox({ftxui::vbox({center(ftxui::hbox(ftxui::text(""), oldPasswordInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(passwordColor)))}) | ftxui::borderRounded,
			center(hbox(ftxui::text(""), passwordInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(passwordColor))) | ftxui::borderRounded,
			center(hbox(ftxui::text(""), confirmPasswordInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(passwordColor))) | ftxui::borderRounded}),
			center(hbox({ftxui::hbox({center(confirmButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(bright_green),
			center(hbox(center(exitButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) })})), }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });


	auto agreePasswordButton = [&]() { passwordControl = 0; };
	auto wrongPasswordContainer = Container::Horizontal({ Button("Retry", [&] { agreePasswordButton(); }) });
	auto wrongOldPasswordRederer = Renderer(wrongPasswordContainer, [&] {
		return vbox({
				   text("Incorrect old password!"),
				   separator(),
				   center(hbox(wrongPasswordContainer->Render())) | color(red),
			}) |
			border;
		});

	auto wrongConfirmPasswordRederer = Renderer(wrongPasswordContainer, [&] {
		return vbox({
				   text("Passwords do not match!"),
				   separator(),
				   center(hbox(wrongPasswordContainer->Render())) | color(red),
			}) |
			border;
		});

	auto samePasswordRederer = Renderer(wrongPasswordContainer, [&] {
		return vbox({
					(text("Old and new passwords are same!")),
				   separator(),
				   center(hbox(wrongPasswordContainer->Render())) | color(red),
			}) |
			border;
		});

	auto mainOldPasswordContainer = Container::Tab({ renderer, wrongPasswordContainer }, &passwordControl);

	auto mainRenderer = Renderer(mainOldPasswordContainer, [&] {
		Element document = renderer->Render();

	if (passwordControl == -1)
		document = dbox({ document, wrongOldPasswordRederer->Render() | clear_under | center, });
	else if (passwordControl == 1)
		document = dbox({ document, wrongConfirmPasswordRederer->Render() | clear_under | center, });
	else if (passwordControl == 2)
		document = dbox({ document, samePasswordRederer->Render() | clear_under | center, });
	return document;
		});
	screen.Loop(mainRenderer);
}

void noticationInterface(UserAccount& administrator)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::vector<std::string> admins = Utility::returnAdmins();
	std::vector<std::string> driversNotification;
	std::vector<std::string> usersNotification;

	std::unordered_map<std::string, UserAccount> userDatabase;
	userDatabase = db::loadUsersFromFile();

	std::string bannerMessage = "Notifications";
	ftxui::Color userNotificationColor = light_gray;
	ftxui::Color driverNotificationColor = light_gray;

	
	for (auto& user : userDatabase)
	{
		if (user.second.getAccountType() == "user" && user.second.getNotificationAlert() == 1)
			usersNotification.push_back(user.second.getUsername() + " registered on system");
		else if (user.second.getAccountType() == "driver" && user.second.getNotificationAlert() == 1)
			driversNotification.push_back(user.second.getUsername() + " didn't complete the report");
	}

	for (auto& user : userDatabase)
	{
		if(user.second.getNotificationAlert() == 1)
			userDatabase[user.second.getUsername()].changeNotificationAlert();
	}
	db::writeUsersToFile(userDatabase);

	auto exitButton = ftxui::Button("Exit", [&] { exit(0); });
	auto backButton = ftxui::Button("Back", [&] { gui::administrator_interface(userDatabase[administrator.getUsername()]); });

	int selected = -1;
	auto driverNotificationBox = Radiobox(&driversNotification, &selected);
	auto usersNotificationBox = Radiobox(&usersNotification, &selected);
	int flag = 0;

	if (driversNotification.size())
		driverNotificationColor = orange;
	else if (usersNotification.size())
		userNotificationColor = orange;

	auto userButton = ftxui::Button("Notifications from users", [&] {flag = 1;});
	auto driverButton = ftxui::Button("Notifications from drivers", [&] { flag = 2; });

	auto component = ftxui::Container::Vertical({ exitButton, backButton, userButton, driverButton, usersNotificationBox, driverNotificationBox });

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(blue)),
				separatorDouble(),
				center(hbox(userButton->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(userNotificationColor))),
				center(hbox(driverButton->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(driverNotificationColor))),
				((flag == 1) ? (hbox(usersNotificationBox->Render())) : (center(hbox(driverNotificationBox->Render())))) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 50),
				center(hbox({hbox({center(backButton->Render()) | size(WIDTH, EQUAL, 9) | ftxui::color(bright_green),
				center(hbox(center(exitButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) })})) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });

	screen.Loop(renderer);
}
void gui::administrator_interface(UserAccount& administrator)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Administrator Account";
	ftxui::Color bannerMessageColor = blue;

	auto accountSettings = ftxui::Button("Account settings", [&] {gui::accountSettingsInterface(administrator); }); // done
	auto codeBooksSettings = ftxui::Button("Codebooks settings", [&] {gui::createCodeBooksInterface(administrator); });
	auto ScheduleSettings = ftxui::Button("Schedule settings", [&] {gui::scheduleSettings(administrator); }); // ostale metode
	auto reportsSettings = ftxui::Button("Reports settings", [&] {gui::reportsSettings(administrator); }); // done
	auto generateTravelWarrant = ftxui::Button("Generate Travel Warrant", [&] {exit(0); });
	auto logout = ftxui::Button("SIGN OUT", [&] {loginInterface(); }); // done

	auto notBox = ftxui::Button("", [&] {noticationInterface(administrator); }); // done

	// CodeBooks Settings
	auto createCodebooks = ftxui::Button("Create codebooks", [&] {exit(0); });
	auto deleteCodebook = ftxui::Button("Create codebooks", [&] {exit(0); });
	auto modifyCodebooks = ftxui::Button("Modify codebooks", [&] {exit(0); });

	auto component = ftxui::Container::Vertical({ notBox, accountSettings, codeBooksSettings, ScheduleSettings, reportsSettings, generateTravelWarrant, logout });

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ hbox({center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			(administrator.getNotificationAlert()) ? hbox(text("             "), notBox->Render() | size(WIDTH, EQUAL, 3) | ftxui::color(yellow) | hcenter) : (hbox() | ftxui::color(dark_gray))}),
			separatorDouble(), vbox({
				center(hbox(accountSettings->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				center(hbox(codeBooksSettings->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				center(hbox(ScheduleSettings->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				center(hbox(reportsSettings->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				center(hbox(logout->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(red))) }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});
	if(pressed == 0 && administrator.getNotificationAlert())
	{
		int agree = 1;
		auto agreeButton = [&]() { agree = 0; pressed = 1; };

		auto agreeContainer = Container::Horizontal({ Button("OK", [&] {agreeButton(); }) });
		auto openNotContainer = Container::Horizontal({ Button("OPEN", [&] {pressed = 1; noticationInterface(administrator); }) });

		auto ButtonsContainer = Container::Horizontal({ agreeContainer, openNotContainer });

		auto agreeRederer = Renderer(ButtonsContainer, [&] {
			return vbox({
					   text("You have notifications!"),
					   separator(),
					   center(hbox({center(hbox(agreeContainer->Render())) | color(bright_green),
						center(hbox(text("  "), openNotContainer->Render())) | color(orange)})),
				}) |
				border;
			});

		auto mainAgreeContainer = Container::Tab({ renderer, agreeRederer }, &agree);

		auto mainRenderer = Renderer(mainAgreeContainer, [&] {
			Element document = renderer->Render();

		if (agree == 1) {
			document = dbox({
				document,
				agreeRederer->Render() | clear_under | center,
				});
		}
		return document;
			});
	screen.Loop(mainRenderer);
	}
	else
		screen.Loop(renderer);
	
}