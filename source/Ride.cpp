#include "Ride.h"

Ride::Ride()
	: m_Driver(""), m_StartTime(std::string()), m_EndTime(std::string()), m_StartLocation(""), m_PathLocations{}, m_EndLocation("") {}

Ride::Ride(std::string rideID, std::string driver, std::string busReg, std::string startTime, std::string endTime, std::string startLocation, std::vector<std::string> pathLocations, std::string endLocation)
	: m_RideID(rideID), m_Driver(driver), m_BusRegistration(busReg), m_StartTime(startTime), m_EndTime(endTime), m_StartLocation(startLocation), m_PathLocations(pathLocations), m_EndLocation(endLocation) {}

void Ride::setRideID(std::string RideID)
{
	m_RideID = RideID;
}

void Ride::setDriver(std::string driver)
{
	m_Driver = driver;
}

void Ride::setBusReg(std::string busReg)
{
	m_BusRegistration = busReg;
}

void Ride::setStartTime(std::string startTime)
{
	m_StartTime = startTime;
}

void Ride::setEndTime(std::string endTime)
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

void Ride::changeDrivenStatus()
{
	m_drivenStatus = true;
}

std::string Ride::getRideID() const
{
	return m_RideID;
}

std::string Ride::getDriver() const
{
	return m_Driver;
}

std::string Ride::getStartTime() const
{
	return m_StartTime;
}

std::string Ride::getEndTime() const
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

bool Ride::getDrivenStatus() const
{
	return m_drivenStatus;
}

std::istream& operator>>(std::istream& is, Ride& ride)
{
	std::string line;
	std::getline(is, line);
	std::stringstream sstream(line);
	std::vector<std::string> items;
	std::vector<std::string> pathLocationItems;
	std::string item;
	size_t count = 0;
	while (std::getline(sstream, item, '#') && count < 5)
	{
		items.push_back(item);
		count++;
	}
	items.push_back(item);
	while (std::getline(sstream, item, '!'))
	{
		pathLocationItems.push_back(item);
	}

	while (std::getline(sstream, item, '#'))
	{
		items.push_back(item);
	}

	ride.m_RideID = items[0];
	ride.m_Driver = items[1];
	ride.m_BusRegistration = items[2];
	ride.m_StartTime = items[3];
	ride.m_EndTime = items[4];
	ride.m_StartLocation = items[5];
	ride.m_EndLocation = items[6];
	pathLocationItems.resize(pathLocationItems.size() - 1);
	ride.m_PathLocations = pathLocationItems;
	ride.m_drivenStatus = std::stoi(items[7]);

	return is;
}

std::ostream& operator<<(std::ostream& os, const Ride& ride)
{
	os << ride.m_RideID << "#" << ride.m_Driver << "#" << ride.m_BusRegistration << "#" << ride.m_StartTime << "#" << ride.m_EndTime << "#" << ride.m_StartLocation << "#" << ride.m_drivenStatus << "#";
	for (size_t i = 0; i < ride.m_PathLocations.size(); i++)
	{
		os << ride.m_PathLocations[i] << "!";
	}
	os << ride.m_EndLocation << "#";
	return os;
}

