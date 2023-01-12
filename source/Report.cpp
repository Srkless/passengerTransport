#include "Report.h"
#include <fstream>
#include <string>

Report::Report(std::string reportID, std::string reportAuthor, std::string content) : rideID(reportID), author(reportAuthor), content(content) {}

void Report::setReportID(std::string reportID) noexcept(true)
{
	rideID = reportID;
}

void Report::setReportAuthor(std::string reportAuthor) noexcept(true)
{
	author = reportAuthor;
}

void Report::setContent(std::string reportContent) noexcept(true)
{
	content = reportContent;
}

std::string Report::getRideID() const noexcept(true)
{
	return rideID;
}

std::string Report::getAuthor() const noexcept(true)
{
	return author;
}

std::string Report::getContent() const noexcept(true)
{
	return content;
}

std::ostream& operator<<(std::ostream& os, const Report& report)
{
	os << report.getRideID() << "#" << report.getAuthor() << "#" << report.getContent() << "#";

	return os;
}

std::istream& operator>>(std::istream& is, Report& report)
{
	std::string line;

	std::getline(is, line, '#');
	report.setReportID(line);

	std::getline(is, line, '#');
	report.setReportAuthor(line);

	std::getline(is, line, '#');
	report.setContent(line);

	return is;
}
