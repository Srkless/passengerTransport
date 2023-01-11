
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

	std::string usernameFile;
	std::string passwordFile;
	
	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	std::string welcome_message{ "Softver za agenciju za prevoz putnika" };
	ftxui::Color welcomeMess_color = blue;
	std::string label = "";
	bool checked = false;

	ftxui::Component inputUsername = ftxui::Input(&username, "Username");

	ftxui::InputOption password_option;
	password_option.password = true;
	ftxui::Component inputPassword = ftxui::Input(&password, "Password", password_option);
	ftxui::Component inputPasswordAgain = ftxui::Input(&againPassword, "Password again", password_option);


	auto RegisterButton = ftxui::Button("Registruj se", [&] {});
	auto exit_button = ftxui::Button("IZLAZ", [&] { exit(0); });

	auto component = ftxui::Container::Vertical({ inputUsername, inputPassword, inputPasswordAgain, RegisterButton, exit_button});

	auto renderer = ftxui::Renderer(component, [&] {
		ftxui::Color input_color = light_gray;
		ftxui::Color password_color = light_gray;
	if (password == againPassword && password.size() > 0)
	{
		input_color = bright_green;
		password_color = bright_green;
		usernameFile = username;
		passwordFile = password;
	}
	
	return  vbox({ center(bold(ftxui::text(welcome_message)) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(welcomeMess_color)),
			separatorDouble(),
			center(hbox(center(inputUsername->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(input_color)))) | ftxui::borderRounded,
			center(hbox(center(inputPassword->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(password_color)))) | ftxui::borderRounded,
			center(hbox(hbox(center(inputPasswordAgain->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(password_color))))) | ftxui::borderRounded,
			center(hbox({ftxui::hbox({center(RegisterButton->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(bright_green),
			center(hbox(center(exit_button->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) })})) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });

	
	screen.Loop(renderer);
}
void gui::login_interface()
{
	std::string username;
	std::string password;
	std::string correct_username;
	std::string correct_password;

	std::string welcome_message{ "Softver za agenciju za prevoz putnika" };
	std::unordered_map<std::string, UserAccount> userDatabase;
	userDatabase = db::loadUsersFromFile();
	ftxui::Component name_input = ftxui::Input(&username, "Username");
	ftxui::InputOption password_option;
	password_option.password = true;
	ftxui::Component password_input = ftxui::Input(&password, "Password", password_option);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	auto quit_app = screen.ExitLoopClosure();
	
	ftxui::Color welcomeMess_color = blue;
	auto log_in_button = ftxui::Button("PRIJAVI SE", [&] {
		if (0) {
				welcome_message = "USPJESNA PRIJAVA";
				welcomeMess_color = bright_green;
			}
		});

	auto exit_button = ftxui::Button("IZLAZ", [&] { exit(0); });
	int a = 0;
	auto component = ftxui::Container::Vertical({ name_input, password_input, log_in_button, exit_button});
	auto renderer = ftxui::Renderer(component, [&] {
		ftxui::Color input_color = light_gray;
		ftxui::Color password_color = light_gray;

	if (password.size() >= 8) { // Provjera da li posotji username

		input_color = { bright_green };
		password_color = bright_green;
		// lozinku je dovoljno provjeravati samo u slucaju da je korisnicko ime tacno
		correct_password = password;
	}
	return ftxui::vbox({ center(bold(ftxui::text(welcome_message)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(welcomeMess_color)),
			separatorDouble(),
			ftxui::hbox({ftxui::vbox({center(ftxui::hbox(ftxui::text(""), name_input->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(input_color)))}) | ftxui::borderRounded,
			center(hbox(ftxui::text(""), password_input->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(password_color))) | ftxui::borderRounded}),
			center(hbox({ftxui::hbox({center(log_in_button->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(bright_green),
			center(hbox(center(exit_button->Render()) | size(WIDTH, EQUAL, 12) | ftxui::color(red))) })})) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	int depth = 0;
	auto on_agree = [&]() { depth = 0; };

	auto depth_1_container = Container::Horizontal({ Button("U REDU", [&] { on_agree(); }) });

	auto depth_1_renderer = Renderer(depth_1_container, [&] {
		return vbox({
				   text("Netacna lozinka"),
				   separator(),
				   center(hbox(depth_1_container->Render())) | color(red),
			}) |
			border;
		});	
	auto main_container = Container::Tab({ renderer, depth_1_renderer }, &depth);

	auto main_renderer = Renderer(main_container, [&] {
		Element document = renderer->Render();

	if (depth == 1) {
		document = dbox({
			document,
			depth_1_renderer->Render() | clear_under | center,
			});
	}
	return document;
		});

	screen.Loop(main_container);

}