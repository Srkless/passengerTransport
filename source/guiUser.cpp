#include "gui.h"

void viewAllRoutsInterface(UserAccount& user)
{
	std::filesystem::path pathSchedule = std::filesystem::current_path();
	pathSchedule += "\\data\\rides";
	std::filesystem::create_directories(pathSchedule);
	pathSchedule += "\\schedule.txt";

	std::string files;
	std::ifstream Data(pathSchedule);

	std::vector<std::string> scheduleRides;

	std::unordered_map<std::string, Ride> allRides = db::loadRidesFromFile();
	std::unordered_map<std::string, Ride> scheduleRidesMap;
	while (Data >> files)
	{
		std::string ride = allRides[files].getStartLocation() + " - " + allRides[files].geEndLocation() + " :  " + allRides[files].getStartTime();
		scheduleRides.push_back(ride);
		scheduleRidesMap[ride] = allRides[files];
	};

	Data.close();
	int selected = -1;

	auto menu = Radiobox(&scheduleRides, &selected);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Available routes: ";
	ftxui::Color bannerMessageColor = blue;
	auto backButton = ftxui::Button("BACK", [&] {gui::UserInterface(user); });
	auto viewButton = ftxui::Button("View Details", [&] {(selected != -1) ? viewRide(user, scheduleRidesMap[scheduleRides[selected]]) : viewAllRoutsInterface(user); });
	auto component = ftxui::Container::Vertical({ menu, backButton, viewButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(), vbox({
				center(hbox(menu->Render())), hbox({
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				center(hbox(viewButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))}) | hcenter | color(white) | borderHeavy
			   }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}

void viewRide(UserAccount& user, Ride& ride)
{

	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage2 = "Start Location: " + ride.getStartLocation();
	std::string bannerMessage3 = "Incidental locations: ";

	for (size_t i = 0; i < ride.getPathLocations().size(); i++)
	{
		bannerMessage3 += (" " + ride.getPathLocations()[i]);
	}

	std::string bannerMessage4 = "End Location: " + ride.geEndLocation();
	std::string bannerMessage5 = "Start Time: " + ride.getStartTime() + " Est.Arrival Time: " + ride.getEndTime();
	ftxui::Color bannerMessageColor = white;
	auto backButton = ftxui::Button("BACK", [&] {viewAllRoutsInterface(user); });
	auto buyButton = ftxui::Button("Purchase", [&] {selectStartLocationInterface(user, ride); });
	auto component = ftxui::Container::Vertical({ backButton, buyButton});

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({
		center(ftxui::text(bannerMessage2) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage3) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage4) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage5) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(), vbox({
				hbox({
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				center(hbox(buyButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))}) | hcenter | color(white) | borderHeavy
			   }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}

void selectStartLocationInterface(UserAccount& user, Ride& ride)
{
	BusTicket ticket;
	std::string ID = ride.getRideID();
	ticket.setRideID(ID);
	std::vector<std::string> locations;
	locations.push_back(ride.getStartLocation());
	for (size_t i = 0; i < ride.getPathLocations().size(); i++)
	{
		locations.push_back(ride.getPathLocations()[i]);
	}

	int selected = -1;
	auto menu = Radiobox(&locations, &selected);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Available Start Locations: ";
	ftxui::Color bannerMessageColor = blue;
	auto backButton = ftxui::Button("BACK", [&] {viewRide(user, ride); });
	auto viewButton = ftxui::Button("Accept", [&] {if(selected != -1) ticket.setStartLocation(locations[selected]); (selected != -1) ? selectEndLocationInterface(user, ride, ticket) : viewRide(user, ride); });
	auto component = ftxui::Container::Vertical({ menu, backButton, viewButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(), vbox({
				center(hbox(menu->Render())), hbox(
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				center(hbox(viewButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))) | hcenter | color(white) | borderHeavy
			   }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}

void selectEndLocationInterface(UserAccount& user, Ride& ride, BusTicket& ticket)
{
	std::vector<std::string> locations;
	if(ticket.getStartLocation() == ride.getStartLocation())
	{
		for (size_t i = 0; i < ride.getPathLocations().size(); i++)
		{
			locations.push_back(ride.getPathLocations()[i]);
		}
	}
	else
	{
		size_t i = 0;
		for (i; ride.getPathLocations()[i] != ticket.getStartLocation(); i++);
		for (i++;i < ride.getPathLocations().size(); i++)
		{
			locations.push_back(ride.getPathLocations()[i]);
		}
	}

	locations.push_back(ride.geEndLocation());

	int selected = -1;
	auto menu = Radiobox(&locations, &selected);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Available End locations: ";
	ftxui::Color bannerMessageColor = blue;
	auto backButton = ftxui::Button("BACK", [&] {selectStartLocationInterface(user, ride); });
	auto viewButton = ftxui::Button("Accept", [&] {if (selected != -1) ticket.setEndLocation(locations[selected]); (selected != -1) ? selectBaggageInterface(user, ride, ticket) : selectStartLocationInterface(user, ride); });
	auto component = ftxui::Container::Vertical({ menu, backButton, viewButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(), vbox({
				center(hbox(menu->Render())), hbox(
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				center(hbox(viewButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))) | hcenter | color(white) | borderHeavy
			   }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}

void selectBaggageInterface(UserAccount& user, Ride& ride, BusTicket& ticket)
{
	int selected = -1;
	std::vector<std::string> options;
	options.push_back("Yes. I want additional baggage.    + 2.00€");
	options.push_back("No.");
	auto menu = Radiobox(&options, &selected);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Do you want to add additional baggage to your ticket?";
	ftxui::Color bannerMessageColor = blue;
	auto backButton = ftxui::Button("BACK", [&] {selectEndLocationInterface(user, ride, ticket); });
	auto viewButton = ftxui::Button("Accept", [&] {if (selected == 0) ticket.addBaggage(); (selected != -1) ? buyTicketInterface(user, ride, ticket) : selectEndLocationInterface(user, ride, ticket); });
	auto component = ftxui::Container::Vertical({ menu, backButton, viewButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(), vbox({
				center(hbox(menu->Render())), hbox(
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				center(hbox(viewButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))) | hcenter | color(white) | borderHeavy
			   }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}

void buyTicketInterface(UserAccount& user, Ride& ride, BusTicket& ticket)
{
	double ticketPrice = ticket.generatePrice(ride);
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage1 = "Start Location: " + ticket.getStartLocation();
	std::string bannerMessage2 = "End Location: " + ticket.getEndLocation();
	std::ostringstream out;
	out << std::fixed << std::setprecision(2) << ticketPrice;
	std::string tmpString = out.str();
	std::string bannerMessage3 = "Ticket Price: " + tmpString;
	out.str("");
	out.clear();
	out << std::fixed << std::setprecision(2) << user.getBalance();
	tmpString = out.str();
	std::string bannerMessage4 = "Your Balance: " + tmpString;
	ftxui::Color bannerMessageColor = white;
	auto backButton = ftxui::Button("BACK", [&] {selectEndLocationInterface(user, ride, ticket); });
	auto buyButton = ftxui::Button("Purchase", [&] { });
	auto component = ftxui::Container::Vertical({backButton, buyButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage1) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage2) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage3) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage4) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(), vbox({
				hbox(
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				center(hbox(buyButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))) | hcenter | color(white) | borderHeavy
			   }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}

void viewTicketsInterface(UserAccount& user)
{
	exit(0);
}
