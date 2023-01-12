#pragma once
#include <string>
#include "Report.h"

class ProblemReport : public Report
{
private:
	std::string typeOfProblem;
public:
	ProblemReport(std::string reportID, std::string reportAuthor, std::string content, std::string typeOfProblem);

	void setTypeOfProblem(std::string problemType) noexcept(true);

	std::string getTypeOfProblem() const noexcept(true);
};