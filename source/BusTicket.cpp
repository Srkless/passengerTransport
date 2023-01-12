#include "BusTicket.h"

BusTicket::BusTicket(std::string rideID, size_t seatNumber, std::string startLocation, std::string endLocation, double price, bool hasBaggageb)
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

int BusTicket::getAvailableTickets()
{
	std::unordered_map<std::string, Ride> rides;
	rides = db::loadRidesFromFile();
	std::unordered_map<std::string, bus> buses;
	buses = db::readBusFromFile();

	std::ofstream oFile;
	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data\\tickets";
	std::filesystem::create_directories(path);
	path += "\\ticketdata.txt";
	oFile.open(path, std::ios::app);

	std::ofstream oFile2;
	path = std::filesystem::current_path();
	path += "\\data\\tickets\\";
	path += m_rideID;
	path += ".txt";

	oFile.seekp(0, std::ios::end);
	if (oFile.tellp() == 0)
	{
		oFile << m_rideID << ".txt";
		oFile2.open(path);
		oFile2 << buses[rides[m_rideID].getBusReg()].getNumberOfSeats();
		oFile2.close();
		return buses[rides[m_rideID].getBusReg()].getNumberOfSeats();
	}
	else
	{
		std::string cmpName = m_rideID + ".txt";
		if (std::filesystem::exists(path))
		{
			int numOfSeats;
			std::ifstream iFile(path);
			iFile >> numOfSeats;
		}
		else
		{
			oFile2.open(path);
			oFile2 << buses[rides[m_rideID].getBusReg()].getNumberOfSeats();
			oFile.close();
			return buses[rides[m_rideID].getBusReg()].getNumberOfSeats();
		}
	}
	oFile.close();
}

bool BusTicket::buyTicket()
{
	int availableTickets = getAvailableTickets();
	if (availableTickets > 0)
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\tickets\\" + m_rideID + ".txt";
		std::ofstream oFile(path);
		oFile << availableTickets - 1;
		return true;
	}
	else
	{
		return false;
	}
}
	
