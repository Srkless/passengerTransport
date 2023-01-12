#pragma once
#include <string>

class Report
{
protected:
	std::string rideID;
	std::string author;
	std::string content;
public:
	Report() {};
	Report(std::string reportID, std::string reportAuthor, std::string content);

	void setReportID(std::string reportID) noexcept(true);
	void setReportAuthor(std::string reportAuthor) noexcept(true);
	void setContent(std::string reportContent) noexcept(true);

	std::string getRideID() const noexcept(true);
	std::string getAuthor() const noexcept(true);
	std::string getContent() const noexcept(true);

	friend std::ostream& operator<<(std::ostream& os, const Report& report);
	friend std::istream& operator>>(std::istream& is, Report& report);
};