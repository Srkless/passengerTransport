#include "gui.h"

void routeOverviewInterface(DriverAccount& driver)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = driver.getUsername() + "'s account";
	ftxui::Color bannerMessageColor = blue;

	std::unordered_map<std::string, Ride> rides;
	rides = db::loadDriverRides(driver.getUsername());

	std::vector<std::string> drivenRideIds;
	std::vector<std::string> undrivenRideIds;

	for (auto& ride : rides)
	{
		if(ride.second.getDrivenStatus())
			drivenRideIds.push_back(ride.first);
		else
			undrivenRideIds.push_back(ride.first);
	}

	int selectedAllDrivenRides = -1;
	int selectedAllUndriverRides = -1;

	auto allDrivenRides = Radiobox(&drivenRideIds, &selectedAllDrivenRides);
	auto allUndrivenRides = Radiobox(&undrivenRideIds, &selectedAllUndriverRides);
	auto driveButton = Button("DRIVE", [&] {driver.driveRoute(undrivenRideIds[selectedAllUndriverRides]); gui::DriverInterface(driver); });
	auto backButton = Button("BACK", [&] {gui::DriverInterface(driver); });
	auto component = ftxui::Container::Vertical({ allDrivenRides, allUndrivenRides, driveButton, backButton });

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
			separatorDouble(), vbox({
				hbox(text("(*) Driven rides") | ftxui::color(light_gray) | hcenter),
				center(hbox(text("   "), allDrivenRides->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				hbox(text("(*) Undriven rides") | ftxui::color(light_gray) | hcenter),
				center(hbox(text("   "), allUndrivenRides->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
				center(hbox(driveButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(orange) | hcenter)),
				center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green) | hcenter)) }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
		});
	screen.Loop(renderer);
}

void reportsOverviewInterface(DriverAccount& driver)
{
	exit(0);
	//auto screen = ftxui::ScreenInteractive::TerminalOutput();
	//std::string bannerMessage = driver.getUsername() + "'s account";
	//ftxui::Color bannerMessageColor = blue;

	//std::unordered_map<std::string, Report> reports;
	//reports = db::loadDriverReports(driver.getUsername());

	//std::vector<std::string> allReports;
	//std::vector<std::string> passangerProblems;
	//std::vector<std::string> busProblems;

	//for (auto& report : reports)
	//{
	//	allReports.push_back(report.second.getRideID());
	//}

	//int selected = -1;

	//auto allDriverReports = Radiobox(&allReports, &selected);
	////auto allUndrivenRides = Radiobox(&undrivenRideIds, &selectedAllUndriverRides);
	//auto backButton = Button("BACK", [&] {gui::DriverInterface(driver); });
	//auto component = ftxui::Container::Vertical({ allDriverReports, allUndrivenRides, driveButton, backButton });

	//auto renderer = ftxui::Renderer(component, [&] {
	//	return ftxui::vbox({ center(bold(ftxui::text(bannerMessage)) | vcenter | size(HEIGHT, EQUAL, 3) | ftxui::color(bannerMessageColor)),
	//		separatorDouble(), vbox({
	//			hbox(text("(*) Driven rides") | ftxui::color(light_gray) | hcenter),
	//			center(hbox(text("   "), allDrivenRides->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
	//			hbox(text("(*) Undriven rides") | ftxui::color(light_gray) | hcenter),
	//			center(hbox(text("   "), allUndrivenRides->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(light_gray) | hcenter)),
	//			center(hbox(driveButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(orange) | hcenter)),
	//			center(hbox(backButton->Render() | size(WIDTH, EQUAL, 20) | ftxui::color(bright_green) | hcenter)) }) }) | hcenter | color(white) | borderHeavy | size(WIDTH, EQUAL, 150);
	//	});
	//screen.Loop(renderer);
}

void writeReportInterface(DriverAccount& driver)
{
	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	std::string bannerMessage = driver.getUsername() + "'s account";
	ftxui::Color bannerMessageColor = blue;

	std::unordered_map<std::string, Report> reports;
	reports = db::loadDriverReports(driver.getUsername());

	std::vector<std::string> options = { "Reports", "Passanger problem", "Bus problem" };
	std::string ReportID;
	std::string text;
	ftxui::Component ReportIDInput = ftxui::Input(&ReportID, "ReportID");
	ftxui::Component textInput = ftxui::Input(&text, "Write problem");
	int selected = -1;

	auto optionsBox = Radiobox(&options, &selected);
	auto backButton = Button("       BACK", [&] {gui::DriverInterface(driver); });
	auto sendButton = Button("  SEND", [&] {
		if (selected == 0)
		{
			Report report(ReportID, driver.getUsername(), text);
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