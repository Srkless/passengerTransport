#include "ProblemReport.h"

ProblemReport::ProblemReport(std::string reportID, std::string reportAuthor, std::string content, std::string typeOfProblem) : typeOfProblem(typeOfProblem), Report(reportID, reportAuthor, content) {};

void ProblemReport::setTypeOfProblem(std::string problemType) noexcept(true)
{
	typeOfProblem = problemType;
}

std::string ProblemReport::getTypeOfProblem() const noexcept(true)
{
	return typeOfProblem;
}
