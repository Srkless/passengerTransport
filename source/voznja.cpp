#include "voznja.h"

Voznja::Voznja()
	: m_Driver(""), m_StartTime(std::chrono::system_clock::now()), m_EndTime(std::chrono::system_clock::now()), m_StartLocation(""), m_PathLocations{}, m_EndLocation(""){}

Voznja::Voznja(std::string driver, std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point endTime, std::string startLocation, std::vector<std::string> pathLocations, std::string endLocation)
	: m_Driver(driver), m_StartTime(startTime), m_EndTime(endTime), m_StartLocation(startLocation), m_PathLocations(pathLocations), m_EndLocation(endLocation){}

void Voznja::setDriver(std::string driver)
{
	m_Driver = driver;
}

void Voznja::setStartTime(std::chrono::system_clock::time_point startTime)
{
	m_StartTime = startTime;
}

void Voznja::setEndTime(std::chrono::system_clock::time_point endTime)
{
	m_EndTime = endTime;
}

void Voznja::setStartLocation(std::string startLocation)
{
	m_StartLocation = startLocation;
}

void Voznja::setEndLocation(std::string endLocation)
{
	m_EndLocation = endLocation;
}

void Voznja::setPathLocationos(std::vector<std::string> pathLocations)
{
	m_PathLocations = pathLocations;
}

std::string Voznja::getDriver()
{
	return m_Driver;
}

std::chrono::system_clock::time_point Voznja::getStartTime()
{
	return m_StartTime;
}

std::chrono::system_clock::time_point Voznja::getEndTime()
{
	return m_EndTime;
}

std::string Voznja::getStartLocation()
{
	return m_StartLocation;
}

std::vector<std::string> Voznja::getPathLocations()
{
	return m_PathLocations;
}

std::string Voznja::geEndLocation()
{
	return m_EndLocation;
}


