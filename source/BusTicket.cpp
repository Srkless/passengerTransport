#include "BusTicket.h"

BusTicket::BusTicket(std::string rideID, size_t seatNumber, std::string startLocation, std::string endLocation, double price, bool hasBaggage)
	: m_rideID(rideID), m_seatNumber(seatNumber), m_startLocation(startLocation), m_endLocation(endLocation)
{}

void BusTicket::generatePrice()
{
	Schedule schedule;
	schedule = db::readScheduleFromFile();
	Ride currentRide = schedule.getRide(m_rideID);

	std::vector<std::string> route;
	std::vector<std::string> currentRideLocations = currentRide.getPathLocations();
	if (m_startLocation == currentRide.getStartLocation())
	{
		route.push_back(currentRide.getStartLocation());
	}
	else
	{
		size_t i = 0;
		while (currentRideLocations[i] != m_startLocation)
		{
			i++;
		}
		route.push_back(currentRideLocations[i]);
		while (currentRideLocations[i] != m_endLocation && i<currentRideLocations.size())
		{
			route.push_back(currentRideLocations[i]);
			i++;
		}
		if (currentRideLocations[i] == m_endLocation)
		{
			route.push_back(m_endLocation);
		}
		else
		{
			route.push_back(currentRide.geEndLocation());
		}
	}

	m_price = (5 * route.size() + (2*m_hasBaggage));
}

void BusTicket::addBaggage()
{
	m_hasBaggage = true;
}

size_t BusTicket::getAvailableTickets()
{
	std::unordered_map;
}
	
