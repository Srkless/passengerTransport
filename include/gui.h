#pragma once
#include "database.h"
#include "UserAccount.h"
#include "utility.h"
#include <ftxui/dom/table.hpp>
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/util/ref.hpp"
#include "DriverAccount.h"
#include "BusTicket.h"

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
namespace gui {

	void loginInterface();
	void registerInterface(std::string accountUsername, int number, bool flagFirst = false);
	void changePassword(std::string username);

	void administrator_interface(UserAccount& administrator);
	void accountSettingsInterface(UserAccount& administrator);
	void createCodeBooksInterface(UserAccount& administrator);
	void EnterLocation(UserAccount& administrator, std::string name, std::filesystem::path& path1, std::filesystem::path& path2);
	void createCodeBus(UserAccount& administrator);
	void createCodeTour(UserAccount& administrator);
	void createCodeLocation(UserAccount& administrator);
	void ScheduleSettings(UserAccount& administrator);
	void generateTravelWarrant(UserAccount& administrator);
	void reportsSettings(UserAccount& administrator);
	void DeleteCodeBooks(UserAccount& administrator);
	void ModifyCodeBooks(UserAccount& administrator);
	void LocationModify(UserAccount& administrator);
	void BusModify(UserAccount& administrator);
	void TourModify(UserAccount& administrator);
	void addRideInterface(UserAccount& administrator);
	void deleteRideInterface(UserAccount& administrator);

	void DriverInterface(DriverAccount& driver);
	void UserInterface(UserAccount& user);
	void CreateSafetycopy(UserAccount& administrator);
};

void routeOverviewInterface(DriverAccount& driver);
void reportsOverviewInterface(DriverAccount& driver);
void writeReportInterface(DriverAccount& driver);
void viewAllRoutsInterface(UserAccount& user);
void selectStartLocationInterface(UserAccount& user, Ride& ride);
void selectEndLocationInterface(UserAccount& user, Ride& ride, BusTicket& ticket);
void selectBaggageInterface(UserAccount& user, Ride& ride, BusTicket& ticket);
void buyTicketInterface(UserAccount& user, Ride& ride, BusTicket& ticket);
void viewTicketsInterface(UserAccount& user);
void viewRide(UserAccount& user, Ride& ride);
void viewTicketInterface(UserAccount& user, BusTicket& ticket);