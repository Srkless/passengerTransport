
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
using namespace ftxui;

namespace {
	ftxui::Color purple = { 93, 39, 93 };
	ftxui::Color red = { 177, 62, 83 };
	ftxui::Color orange = { 239, 125, 87 };
	ftxui::Color yellow = { 255, 205, 117 };
	ftxui::Color bright_green = { 167, 240, 112 };
	ftxui::Color dark_green = { 56, 183, 100 };
	ftxui::Color teal = { 37, 113, 121 };
	ftxui::Color dark_blue = { 41, 54, 111 };
	ftxui::Color blue = { 59, 93, 201 };
	ftxui::Color light_blue = { 65, 166, 246 };
	ftxui::Color cyan = { 115, 239, 247 };
	ftxui::Color white = { 255, 255, 255 };
	ftxui::Color light_gray = { 148, 176, 194 };
	ftxui::Color gray = { 86, 108, 134 };
	ftxui::Color dark_gray = { 51, 60, 87 };

}

void gui::register_interface()
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
		login_interface();
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

	return  vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
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
		if (userDatabase[username].getUsername() != "" && Utility::decrypt(userDatabase[username].getPassword()) == Utility::decrypt(password)) {
			bannerMessage = "USPJESNA PRIJAVA";
			bannerMessageColor = bright_green;
			wrongPassword = 0;
			wrongUsername = 0;
			//Cita se type akaunta i otvaraa odg interfejs
			// Provjerava se broj prijava na akaunt ako je 30 izbacuje promjenu sifre
			//
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
		}

		});

	auto exitButton = ftxui::Button("IZLAZ", [&] { exit(0); });

	auto component = ftxui::Container::Vertical({ usernameInput, passwordInput, logInButton, exitButton });

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
			center(hbox(center(exitButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) })})) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });


	auto agreeUsernameButton = [&]() { wrongUsername = 0; wrongPassword = 0; };
	auto wrongUsernameContainer = Container::Horizontal({ Button("PONOVO", [&] { agreeUsernameButton(); }) });
	auto wrongUsernameRenderer = Renderer(wrongUsernameContainer, [&] {
		return vbox({
				   text("Username netacan"),
				   separator(),
				   center(hbox(wrongUsernameContainer->Render())) | color(red),
			}) |
			border;
		});

	auto agreePasswordButton = [&]() { wrongPassword = 0; wrongUsername = 0; };
	auto wrongPasswordContainer = Container::Horizontal({ Button("PONOVO", [&] { agreePasswordButton(); }) });
	auto wrongPasswordRederer = Renderer(wrongPasswordContainer, [&] {
		return vbox({
				   text("Loznika netacna"),
				   separator(),
				   center(hbox(wrongPasswordContainer->Render())) | color(red),
			}) |
			border;
		});

	

	auto mainPasswordContainer = Container::Tab({ renderer, wrongPasswordRederer }, &wrongPassword);
	auto mainUsernameContainer = Container::Tab({ renderer, wrongUsernameRenderer }, &wrongUsername);
	auto mainContainer = Container::Vertical({ mainPasswordContainer, mainUsernameContainer });

	auto mainRenderer = Renderer(mainContainer, [&] {
		Element document = renderer->Render();

	if (wrongUsername == 1)
		document = dbox({ document, wrongUsernameRenderer->Render() | clear_under | center, });
	else if (wrongPassword == 1)
		document = dbox({ document, wrongPasswordRederer->Render() | clear_under | center, });
	return document;
		});

	screen.Loop(mainRenderer);

}