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

void gui::register_interface(int number)
{
	std::string username;
	std::string password;
	std::string againPassword;
	int wrongPassword = 0;
	int wrongUsername = 0;

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	std::string bannerMessage = "Softver za agenciju za prevoz putnika";

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

	int flag = 0;
	auto registerButton = ftxui::Button("Registruj se", [&] {
		flag = 1;
	if (flag && userDatabase[username].getUsername() == "" && password == againPassword && password.size() > 7) {
		bannerMessage = "USPJESNA REGISTRACIJA";
		bannerMessageColor = bright_green;
		wrongPassword = 0;
		wrongUsername = 0;
		UserAccount curr(username, password, "korisnik", 0);
		if (flag)
			db::addUserToFile(curr);
		if (number == 0)
			login_interface();
		else if (number == 1)
			administrator_interface();
	}
	else
	{
		bannerMessage = "NEUSPJESNA REGISTRACIJA";
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
	auto exitButton = ftxui::Button("IZLAZ", [&] { exit(0); });

	auto component = ftxui::Container::Vertical({ inputUsername, inputPassword, inputPasswordAgain, registerButton, exitButton });

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
			center(hbox(center(exitButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) })})) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });

	auto agreePasswordButton = [&]() { wrongPassword = 0; wrongUsername = 0; };
	auto wrongPasswoedContainer = Container::Horizontal({ Button("PONOVO", [&] { agreePasswordButton(); }) });
	auto wrongPasswoedRederer = Renderer(wrongPasswoedContainer, [&] {
		return vbox({
				   text("Loznike se ne poklapaju ili su manje od 8 slova"),
				   separator(),
				   center(hbox(wrongPasswoedContainer->Render())) | color(red),
			}) |
			border;
		});

	auto agreeUsernameButton = [&]() { wrongUsername = 0; wrongPassword = 0; };
	auto wrongUsernameContainer = Container::Horizontal({ Button("PONOVO", [&] { agreeUsernameButton(); }) });
	auto wrongUsernameRenderer = Renderer(wrongUsernameContainer, [&] {
		return vbox({
				   text("Username vec postoji"),
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

void gui::login_interface()
{
	std::string username;
	std::string password;

	std::string bannerMessage = "Softver za agenciju za prevoz putnika";
	int wrongPassword = 0;
	int wrongUsername = 0;
	int suspended = 0;

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

	

	auto logInButton = ftxui::Button("PRIJAVI SE", [&] {
		if (userDatabase[username].getUsername() != "" && Utility::decrypt(userDatabase[username].getPassword()) == Utility::decrypt(password) && !userDatabase[username].getSuspendInfo()) {
			bannerMessage = "USPJESNA PRIJAVA";
			bannerMessageColor = bright_green;
			wrongPassword = 0;
			wrongUsername = 0;
			if (userDatabase[username].getNumOfLogins() >= loginNums)
			{
				change_password(userDatabase[username].getUsername());
				userDatabase[username].resetNumOfLogins();
			}
			else
			{
				userDatabase[username].increaseNumOfLogins();
				db::writeUsersToFile(userDatabase);
			}
			if (userDatabase[username].getAccountType() == "administrator")
				administrator_interface();
		}
		else
		{
			bannerMessage = "NEUSPJESNA PRIJAVA";
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
			else if(userDatabase[username].getSuspendInfo())
				suspended = 1;
		}

		});

	auto exitButton = ftxui::Button("IZLAZ", [&] { exit(0); });
	auto registerButton = ftxui::Button("REGISTRUJ SE", [&] {register_interface(0); });
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
			ftxui::hbox({ftxui::vbox({center(ftxui::hbox(ftxui::text(""), usernameInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(usernameColor)))}) | ftxui::borderRounded,
			center(hbox(ftxui::text(""), passwordInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(passwordColor))) | ftxui::borderRounded}),
			center(hbox({ftxui::hbox({center(logInButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(bright_green),
			center(hbox(center(exitButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) })})), 
			center(hbox(center(registerButton->Render()) | size(WIDTH, EQUAL, 15) | ftxui::color(orange))) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });


	auto agreeUsernameButton = [&]() { wrongUsername = 0; wrongPassword = 0; suspended = 0; };
	auto wrongUsernameContainer = Container::Horizontal({ Button("PONOVO", [&] { agreeUsernameButton(); }) });
	auto wrongUsernameRenderer = Renderer(wrongUsernameContainer, [&] {
		return vbox({
				   text("Username netacan"),
				   separator(),
				   center(hbox(wrongUsernameContainer->Render())) | color(red),
			}) |
			border;
		});

	auto agreePasswordButton = [&]() { wrongPassword = 0; wrongUsername = 0; suspended = 0; };
	auto wrongPasswordContainer = Container::Horizontal({ Button("PONOVO", [&] { agreePasswordButton(); }) });
	auto wrongPasswordRederer = Renderer(wrongPasswordContainer, [&] {
		return vbox({
				   text("Loznika netacna"),
				   separator(),
				   center(hbox(wrongPasswordContainer->Render())) | color(red),
			}) |
			border;
		});
	auto agreeSuspended = [&]() { wrongUsername = 0; wrongPassword = 0; suspended = 0; };
	auto suspendedContainer = Container::Horizontal({ Button("PONOVO", [&] { agreeSuspended(); }) });
	auto suspendedRederer = Renderer(suspendedContainer, [&] {
		return vbox({
				   text("Nalog je suspendovan"),
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

void gui::change_password(std::string username)
{
	std::string oldPassword;
	std::string password;
	std::string confirmPassword;
	ftxui::Color passwordColor = light_gray;
	int passwordControl = 0; // 0 - sve okej, -1 stara sifra ne valja, 1 ne valja nova

	std::string bannerMessage = "Promjena lozinke";
	ftxui::Color bannerMessageColor = light_gray;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	ftxui::InputOption passwordOption;
	passwordOption.password = true;

	ftxui::Component oldPasswordInput = ftxui::Input(&oldPassword, "Old Password", passwordOption);
	ftxui::Component passwordInput = ftxui::Input(&password, "New Password", passwordOption);
	ftxui::Component confirmPasswordInput = ftxui::Input(&confirmPassword, "Confirm Password", passwordOption);

	std::unordered_map<std::string, UserAccount> userDatabase;
	userDatabase = db::loadUsersFromFile();

	auto confirmButton = ftxui::Button("POTVRDI", [&] {
		if (Utility::decrypt(userDatabase[username].getPassword()) == Utility::decrypt(oldPassword) && Utility::decrypt(password) == Utility::decrypt(confirmPassword))
		{
			bannerMessage = "Uspjesna promjena lozinke";
			passwordColor = bright_green;
			bannerMessageColor = bright_green;
			passwordControl = 0;
			userDatabase[username].setPassword(confirmPassword);
			userDatabase[username].resetNumOfLogins();
			db::writeUsersToFile(userDatabase);
		}
		else
		{
			bannerMessage = "Neuspjesna promjena lozinke";
			bannerMessageColor = red;
			passwordColor = red;
			if (Utility::decrypt(userDatabase[username].getPassword()) != Utility::decrypt(oldPassword))
				passwordControl = -1;
			else if(password != confirmPassword)
				passwordControl = 1;
		}
	});
	auto exitButton = ftxui::Button("IZLAZ", [&] { exit(0); });

	auto component = ftxui::Container::Vertical({ oldPasswordInput, passwordInput, confirmPasswordInput , confirmButton, exitButton});

	auto renderer = ftxui::Renderer(component, [&] {

		if (Utility::decrypt(userDatabase[username].getPassword()) == Utility::decrypt(oldPassword) && Utility::decrypt(password) == Utility::decrypt(confirmPassword)) {
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
			center(hbox(center(exitButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) })})),}) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });


	auto agreePasswordButton = [&]() { passwordControl = 0; };
	auto wrongPasswordContainer = Container::Horizontal({ Button("PONOVO", [&] { agreePasswordButton(); }) });
	auto wrongOldPasswordRederer = Renderer(wrongPasswordContainer, [&] {
		return vbox({
				   text("Stara Loznika netacna"),
				   separator(),
				   center(hbox(wrongPasswordContainer->Render())) | color(red),
			}) |
			border;
		});

	auto wrongConfirmPasswordRederer = Renderer(wrongPasswordContainer, [&] {
		return vbox({
				   text("Loznike se ne poklapaju"),
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
	return document;
		});
	screen.Loop(mainRenderer);
}

void gui::administrator_interface()
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Administrator Account";
	ftxui::Color bannerMessageColor = blue;

	auto accountSettings = ftxui::Button("Account settings", [&] {gui::accountSettingsInterface(); });
	auto codeBooksSettings = ftxui::Button("Codebooks settings", [&] {exit(0); });
	auto ScheduleSettings = ftxui::Button("Schedule settings", [&] {exit(0); });
	auto reportsSettings = ftxui::Button("Reports settings", [&] {exit(0); });
	auto generateTravelWarrant = ftxui::Button("Generate Travel Warrant", [&] {exit(0); });
	auto logout = ftxui::Button("Logout", [&] {login_interface(); });


	// CodeBooks Settings
	auto createCodebooks = ftxui::Button("Create codebooks", [&] {exit(0); });
	auto deleteCodebook = ftxui::Button("Create codebooks", [&] {exit(0); });
	auto modifyCodebooks = ftxui::Button("Modify codebooks", [&] {exit(0); });

	// Schedule
	auto addRide = ftxui::Button("Add ride", [&] {exit(0); });
	auto deleteRide = ftxui::Button("Delete ride", [&] {exit(0); });

	auto component = ftxui::Container::Vertical({ accountSettings, codeBooksSettings, ScheduleSettings, reportsSettings, generateTravelWarrant, logout });

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(accountSettings->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				center(hbox(codeBooksSettings->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				center(hbox(ScheduleSettings->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				center(hbox(reportsSettings->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				center(hbox(logout->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(red))) }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);;
		});

	screen.Loop(renderer);
	//| hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150), | size(WIDTH, EQUAL, 12) | ftxui::color(bright_green)))

}