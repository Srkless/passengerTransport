#include "gui.h"

void routeOverviewInterface(DriverAccount& driver)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = driver.getUsername() + "'s account";
	ftxui::Color bannerMessageColor = blue;

	std::unordered_map<std::string, UserAccount> userDatabase;
	userDatabase = db::loadUsersFromFile();

	std::unordered_map<std::string, Ride> rides;
	int ridesSize = 0;
	rides = db::loadDriverRides(driver.getUsername());
	

	std::vector<std::string> drivenRideIds;
	std::vector<std::string> undrivenRideIds;

	for (auto& ride : rides)
	{
		if (ride.second.getDriver() == driver.getUsername())
			ridesSize++;
		if(ride.second.getDriver() == driver.getUsername() && ride.second.getDrivenStatus())
			drivenRideIds.push_back(ride.first);
		else
			undrivenRideIds.push_back(ride.first);
	}

	int selectedAllDrivenRides = -1;
	int selectedAllUndriverRides = -1;
	
	if (ridesSize == drivenRideIds.size())
	{
		for (int i = 0; i < drivenRideIds.size(); i++)
		{
			rides[drivenRideIds[i]].changeDrivenStatus(false);
			db::rewriteExistingRide(rides[drivenRideIds[i]]);
		}
	}
	//db::writeUsersToFile(userDatabase);
	auto allDrivenRides = Radiobox(&drivenRideIds, &selectedAllDrivenRides);
	auto allUndrivenRides = Radiobox(&undrivenRideIds, &selectedAllUndriverRides);
	auto driveButton = Button("DRIVE", [&] {driver.driveRoute(undrivenRideIds[selectedAllUndriverRides]); driver.decreaseReport(); gui::DriverInterface(driver); });
	auto backButton = Button("       BACK", [&] {gui::DriverInterface(driver); });
	auto component = ftxui::Container::Vertical({ allDrivenRides, allUndrivenRides, driveButton, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				hbox(text("(*) Driven rides") | ftxui::color(light_gray) | hcenter),
				center(hbox(text("   "), allDrivenRides->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				hbox(text("(*) Undriven rides") | ftxui::color(light_gray) | hcenter),
				center(hbox(text("   "), allUndrivenRides->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				(undrivenRideIds.size() != 0)?center(hbox(driveButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(orange) | hcenter)) : center(hbox()),
				center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green) | hcenter)) }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});
	
	screen.Loop(renderer);
}

void reportsOverviewInterface(DriverAccount& driver)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = driver.getUsername() + "'s account";
	ftxui::Color bannerMessageColor = blue;

	std::unordered_map<std::string, Report> reports;
	reports = db::loadDriverReports(driver.getUsername());

	std::unordered_map<std::string, ProblemReport> problems;
	problems = db::loadDriverProblemReports(driver.getUsername());

	std::vector<std::string> allReports;
	std::vector<std::string> passangerProblems;
	std::vector<std::string> busProblems;

	allReports.push_back("Reports");
	passangerProblems.push_back("Passenger Problems");
	busProblems.push_back("Autobus Problems");

	for (auto& report : reports)
	{
		allReports.push_back(report.second.getRideID());
	}

	for (auto& problem : problems)
	{
		if(problem.second.getTypeOfProblem() == "passenger")
			passangerProblems.push_back(problem.second.getRideID());
		else
			busProblems.push_back(problem.second.getRideID());
	}

	int reportSelect = -1;
	int passProbSelect = -1;
	int busProbSelect = -1;

	auto allReportsBox = Radiobox(&allReports, &reportSelect);
	auto allPassangerProblemsBox = Radiobox(&passangerProblems, &passProbSelect);
	auto allBusProblemsBox = Radiobox(&busProblems, &busProbSelect);

	auto backButton = Button("       BACK", [&] {gui::DriverInterface(driver); });
	auto component = ftxui::Container::Vertical({ allReportsBox, allPassangerProblemsBox, allBusProblemsBox, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				center(hbox(text(""), allReportsBox->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				(reportSelect > 0)?center(hbox(text(allReports[reportSelect]))) | borderHeavy | size(WIDTH, EQUAL, 150) : center(hbox()),
				center(hbox(text(""), allPassangerProblemsBox->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				(passProbSelect > 0)?center(hbox(text(passangerProblems[passProbSelect]))) | borderHeavy | size(WIDTH, EQUAL, 150) : center(hbox()),
				center(hbox(text(""), allBusProblemsBox->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				(busProbSelect > 0)?center(hbox(text(busProblems[busProbSelect]))) | borderHeavy | size(WIDTH, EQUAL, 150) : center(hbox()),
				center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green) | hcenter)) }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});
	screen.Loop(renderer);
}

void writeReportInterface(DriverAccount& driver)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = driver.getUsername() + "'s account";
	ftxui::Color bannerMessageColor = blue;

	if (driver.getReports() == 0)
	{
		std::unordered_map<std::string, UserAccount> userDatabase;
		userDatabase = db::loadUsersFromFile();
		std::vector<std::string> admins = Utility::returnAdmins();
		if (driver.checkRouteAndReport())
		{
			if (userDatabase[driver.getUsername()].getNotificationAlert() == 0)
				userDatabase[driver.getUsername()].changeNotificationAlert();
			for (auto& admin : admins)
			{
				if (userDatabase[admin].getNotificationAlert() == 0)
					userDatabase[admin].changeNotificationAlert();
			}
			db::writeUsersToFile(userDatabase);
		}
	}
	std::vector<std::string> options = { "Reports", "Passanger problem", "Bus problem" };
	std::string ReportID;
	std::string text;
	ftxui::Component ReportIDInput = ftxui::Input(&ReportID, "ReportID");
	ftxui::Component textInput = ftxui::Input(&text, "Write problem");
	int selected = -1;

	auto optionsBox = Radiobox(&options, &selected);
	auto backButton = Button("       BACK", [&] {gui::DriverInterface(driver); });
	auto sendButton = Button("  SEND", [&] {
		if(ReportID != "" && text != "")
		{
			if (selected == 0)
			{
				Report report(ReportID, driver.getUsername(), text);
				driver.increaseReport();
				driver.writeReport(ReportID, report);
			}
			else if (selected == 1)
			{
				ProblemReport passProblem(ReportID, driver.getUsername(), text, "passenger");
				driver.writeProblemReport(ReportID, passProblem);
			}
			else
			{
				ProblemReport passProblem(ReportID, driver.getUsername(), text, "bus");
				driver.writeProblemReport(ReportID, passProblem);
			}
		}
		gui::DriverInterface(driver);
		});
	auto component = ftxui::Container::Vertical({ optionsBox, backButton,ReportIDInput, textInput, sendButton });
	
	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				hbox(optionsBox->Render() | ftxui::color(light_gray) | hcenter),
				center(hbox({ReportIDInput->Render() | size(WIDTH, EQUAL, 10) | ftxui::color(light_gray) | ftxui::borderRounded,
				textInput->Render() | size(WIDTH, EQUAL, 30) | ftxui::color(light_gray) | ftxui::borderRounded,
				center(hbox(sendButton->Render() | size(WIDTH, EQUAL, 10) | ftxui::color(light_gray) | hcenter))})),
				center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green) | hcenter)) }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});
	screen.Loop(renderer);
}