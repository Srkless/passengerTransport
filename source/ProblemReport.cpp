#include "ProblemReport.h"
#include <fstream>
#include <string>


ProblemReport::ProblemReport(std::string reportID, std::string reportAuthor, std::string content, std::string typeOfProblem) : typeOfProblem(typeOfProblem), Report(reportID, reportAuthor, content) {};

void ProblemReport::setTypeOfProblem(std::string problemType) noexcept(true)
{
	typeOfProblem = problemType;
}

std::string ProblemReport::getTypeOfProblem() const noexcept(true)
{
	return typeOfProblem;
}

std::ostream& operator<<(std::ostream& os, const ProblemReport& report)
{
	os << report.getRideID() << "#" << report.getAuthor() << "#"  << report.getTypeOfProblem() << "#" << report.getContent() << "#";

	return os;
}

std::istream& operator>>(std::istream& is, ProblemReport& report)
{
	std::string line;

	std::getline(is, line, '#');
	report.setReportID(line);

	std::getline(is, line, '#');
	report.setReportAuthor(line);

	std::getline(is, line, '#');
	report.setTypeOfProblem(line);

	std::getline(is, line, '#');
	report.setContent(line);

	return is;
}
