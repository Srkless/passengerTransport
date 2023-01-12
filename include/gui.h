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

	void login_interface();
	void register_interface(int number);
	void change_password(std::string username);
	void administrator_interface(std::string username);

	void accountSettingsInterface(std::string username);
	/*void selling_items_interface(Database& db);
	void supervisor_interface(Database& db);
	void employee_overview(Database& db);
	void create_employee_interface(Database& db);
	void items_overview(Database& db);
	void report_interface(Database& db);
	void daily_report(Database& db, const std::vector<Item>& items);
	void weekly_report(Database& db, const std::vector<Item>& items);
	void monthly_report(Database& db, const std::vector<Item>& items);
	void yearly_report(Database& db, const std::vector<Item>& items);
	void arbitrary_report(Database& db, const std::vector<Item>& items);
	void create_item_interface(Database& db);*/
};