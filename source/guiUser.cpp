#include "gui.h"
#include <iomanip>

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
	ticket.setStartTime(ride.getStartTime());
	ticket.setEndTime(ride.getEndTime());
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
	int error = 2;
	int flag = 0;
	auto backButton = ftxui::Button("BACK", [&] {selectEndLocationInterface(user, ride, ticket); });
	auto buyButton = ftxui::Button("Purchase", [&] {flag = 1; error = ticket.buyTicket(user, ride); });
	auto component = ftxui::Container::Vertical({backButton, buyButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({ center(ftxui::text(bannerMessage1) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage2) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage3) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage4) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(), vbox({
				hbox(
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				center(hbox(buyButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))))  | hcenter | color(white) | borderHeavy
			   }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });

	auto agreeButton = [&]() { error = 0; gui::UserInterface(user); };
		auto availableTicketsContainer = Container::Horizontal({ Button("OK", [&] { agreeButton(); }) });
		auto availableTicketsRederer = Renderer(availableTicketsContainer, [&] {
			return vbox({
					   text("All tickets are sold"),
					   separator(),
					   center(hbox(availableTicketsContainer->Render())) | color(red),
				}) |
				border;
			});

		auto priceContainer = Container::Horizontal({ Button("OK", [&] { agreeButton(); }) });
		auto priceRederer = Renderer(priceContainer, [&] {
			return vbox({
					   text("You don't have enough money"),
					   separator(),
					   center(hbox(priceContainer->Render())) | color(red),
				}) |
				border;
			});

		auto successContainer = Container::Horizontal({ Button("OK", [&] { agreeButton(); }) });
		auto successRederer = Renderer(priceContainer, [&] {
			return vbox({
					   text("Successful purchase"),
					   separator(),
					   center(hbox(priceContainer->Render())) | color(bright_green),
				}) |
				border;
			});

		auto mainAvailableTicketsContainer = Container::Tab({ renderer, availableTicketsRederer }, &error);
		auto mainPriceContainer = Container::Tab({ renderer, priceRederer }, &error);
		auto mainsSucessContainer = Container::Tab({ renderer, successRederer }, &error);
		auto mainContainer = Container::Vertical({ mainAvailableTicketsContainer, mainPriceContainer, mainsSucessContainer });

		auto mainRenderer = Renderer(mainContainer, [&] {
			Element document = renderer->Render();

		if (error == 0) {
			document = dbox({
				document,
				availableTicketsRederer->Render() | clear_under | center,
				});
		}
		else if (error == -1) document = dbox({
				document,
				priceRederer->Render() | clear_under | center,
			});
		else if (error == 1)
		{
			document = dbox({
				document,
				successRederer->Render() | clear_under | center,
				});
		}

		return document;
			});

		
		screen.Loop(mainRenderer);
}

void viewTicketsInterface(UserAccount& user)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = "Your tickets: ";
	std::string tmp2;
	int numOfTickets;
	std::vector<std::string> stringArr;
	std::vector<std::string> tickets;
	std::unordered_map<std::string, int> ticketIDs;
	std::vector<BusTicket> ticketArr;
	std::filesystem::path path = std::filesystem::current_path();
	path += ("\\data\\tickets\\user_tickets\\" + user.getUsername() + ".txt");
	if (std::filesystem::exists(path))
	{
		std::ifstream iFile(path);
		while(!iFile.eof())
		{
			std::string tmp;
			std::string line;
			std::getline(iFile, line);
			std::stringstream sstream(line);
			std::getline(sstream, tmp, '#');
			std::getline(sstream, line, '#');
			ticketIDs[tmp] = std::stoi(line);
		}
		iFile.close();
		for (auto& it : ticketIDs)
		{
			for (size_t i = 0; i < it.second; i++)
			{
				std::string name = it.first + "_" + std::to_string(i + 1);
				ticketArr.push_back(BusTicket::readFromFile(user, name));
				stringArr.push_back(ticketArr.back().getStartLocation() + " - " + ticketArr.back().getEndLocation());
			}
		}
	}

	

	int selected = -1;
	auto view = Radiobox(&stringArr, &selected);

	ftxui::Color bannerMessageColor = white;
	auto backButton = ftxui::Button("BACK", [&] {gui::UserInterface(user); });
	auto viewButton = ftxui::Button("VIEW", [&] {if(selected != -1)viewTicketInterface(user, ticketArr[selected]); });
	auto component = ftxui::Container::Vertical({view, viewButton, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({
		center(ftxui::text(bannerMessage) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(), vbox({
				center(hbox(view->Render())),
				hbox({
				center(hbox(backButton->Render())),
				center(hbox(viewButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green)))}) | hcenter | color(white) | borderHeavy
			   }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}

void viewTicketInterface(UserAccount& user, BusTicket& ticket)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage1 = "Ride ID: " + ticket.getRideID() + " Price: ";
	std::ostringstream sstream; sstream << std::fixed << std::setprecision(2) << ticket.getPrice(); std::string priceString = sstream.str();
	bannerMessage1 += priceString;
	std::string bannerMessage2 = "Start Location: " + ticket.getStartLocation();
	std::string bannerMessage3 = "End Location: " + ticket.getEndLocation();
	std::string bannerMessage4 = "Start Time: " + ticket.getStartTime() + " Est.Arrival Time: " + ticket.getEndTime();
	std::string bannerMessage5 = (ticket.hasBaggage() ? "Has Additional Baggage" : "Does Not Have Additional Baggage");
	ftxui::Color bannerMessageColor = white;
	auto backButton = ftxui::Button("BACK", [&] {viewTicketsInterface(user); });
	auto component = ftxui::Container::Vertical({ backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({
		center(ftxui::text(bannerMessage1) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage2) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage3) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage4) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		center(ftxui::text(bannerMessage5) | vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(bannerMessageColor)),
		   separatorDouble(), vbox({
				hbox({
				center(hbox(backButton->Render() | size(WIDTH, LESS_THAN, 20) | ftxui::color(bright_green))),
				}) | hcenter | color(white) | borderHeavy
			   }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150); });
	screen.Loop(renderer);
}