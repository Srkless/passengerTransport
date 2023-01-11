#include "Ride.h"

Ride::Ride()
	: m_Driver(""), m_StartTime(std::chrono::system_clock::now()), m_EndTime(std::chrono::system_clock::now()), m_StartLocation(""), m_PathLocations{}, m_EndLocation("") {}

Ride::Ride(std::string rideID, std::string driver, std::chrono::system_clock::time_point startTime, std::chrono::system_clock::time_point endTime, std::string startLocation, std::vector<std::string> pathLocations, std::string endLocation)
	: m_RideID(rideID), m_Driver(driver), m_StartTime(startTime), m_EndTime(endTime), m_StartLocation(startLocation), m_PathLocations(pathLocations), m_EndLocation(endLocation) {}

void Ride::setRideID(std::string RideID)
{
	m_RideID = RideID;
}

void Ride::setDriver(std::string driver)
{
	m_Driver = driver;
}

void Ride::setStartTime(std::chrono::system_clock::time_point startTime)
{
	m_StartTime = startTime;
}

void Ride::setEndTime(std::chrono::system_clock::time_point endTime)
{
	m_EndTime = endTime;
}

void Ride::setStartLocation(std::string startLocation)
{
	m_StartLocation = startLocation;
}

void Ride::setEndLocation(std::string endLocation)
{
	m_EndLocation = endLocation;
}

void Ride::setPathLocationos(std::vector<std::string> pathLocations)
{
	m_PathLocations = pathLocations;
}

std::string Ride::getRideID() const
{
	return m_RideID;
}

std::string Ride::getDriver() const
{
	return m_Driver;
}

std::chrono::system_clock::time_point Ride::getStartTime() const
{
	return m_StartTime;
}

std::chrono::system_clock::time_point Ride::getEndTime() const
{
	return m_EndTime;
}

std::string Ride::getStartLocation() const
{
	return m_StartLocation;
}

std::vector<std::string> Ride::getPathLocations() const
{
	return m_PathLocations;
}

std::string Ride::geEndLocation() const
{
	return m_EndLocation;
}

