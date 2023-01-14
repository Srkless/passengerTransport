#include "BusTicket.h"

BusTicket::BusTicket()
	: m_rideID(""), m_startLocation(""), m_endLocation("")
{
}

BusTicket::BusTicket(std::string rideID, std::string startLocation, std::string endLocation, bool hasBaggageb)
	: m_rideID(rideID), m_startLocation(startLocation), m_endLocation(endLocation)
{}

void BusTicket::setRideID(std::string& ID)
{
	m_rideID = ID;
}

void BusTicket::setStartLocation(std::string& startLocation)
{
	m_startLocation = startLocation;
}

void BusTicket::setEndLocation(std::string& endLocation)
{
	m_endLocation = endLocation;
}

std::string BusTicket::getStartLocation()
{
	return m_startLocation;
}

std::string BusTicket::getEndLocation()
{
	return m_endLocation;
}

std::string BusTicket::getRideID()
{
	return m_rideID;
}

bool BusTicket::hasBaggage()
{
	return m_hasBaggage;
}

double BusTicket::generatePrice(Ride& currentRide)
{
	std::vector<std::string> route;
	std::vector<std::string> currentRideLocations = currentRide.getPathLocations();
	size_t i = 0;
	if (m_startLocation == currentRide.getStartLocation())
	{
		route.push_back(currentRide.getStartLocation());
	}
	else
	{
		while (currentRideLocations[i] != m_startLocation)
		{
			i++;
		}
		route.push_back(currentRideLocations[i++]);
	}
	
	while (i < currentRideLocations.size() && currentRideLocations[i] != m_endLocation)
	{
		route.push_back(currentRideLocations[i]);
		i++;
	}
	if(i > 0)
	{
		if (currentRideLocations[i - 1] == m_endLocation)
		{
			route.push_back(m_endLocation);
		}
		else
		{
			route.push_back(currentRide.geEndLocation());
		}
	}
	else if (i == 0 && currentRideLocations.size() == 0)
	{
		route.push_back(m_endLocation);
	}
	return (15 * route.size() + (2 * m_hasBaggage));
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
			iFile >> (numOfSeats);
			return numOfSeats;
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
	return 0;
}

bool BusTicket::buyTicket(UserAccount& usr, Ride& ride)
{
	double price = generatePrice(ride);
	int availableTickets = getAvailableTickets();
	if (availableTickets > 0 && price <= usr.getBalance())
	{
		std::filesystem::path path = std::filesystem::current_path();
		path += "\\data\\tickets\\" + m_rideID + ".txt";
		std::ofstream oFile(path);
		oFile << availableTickets - 1;
		oFile.close();

		// change user's ticket count
		usr.setBalance(usr.getBalance() - price);
		path = std::filesystem::current_path();
		path += "\\data\\tickets\\user_tickets\\";
		std::filesystem::create_directories(path);
		path += usr.getUsername() + ".txt";
		if (std::filesystem::exists(path))
		{
			std::unordered_map<std::string, int> tickets;
			std::string rideName;
			int ticketNum;
			std::ifstream ticketFile(path);
			std::string line;
			while (!ticketFile.eof())
			{
				std::getline(ticketFile, line);
				std::stringstream sstream(line);
				
				std::getline(sstream, rideName, '#');
				std::getline(sstream, line, '#');
				ticketNum = std::stoi(line);
				tickets[rideName] = ticketNum;
				if (rideName == m_rideID)
				{
					tickets[rideName]++;
				}
			}
			ticketFile.close();
			std::ofstream ticketOFile(path);
			for (auto& ticket : tickets)
			{
				if (ticket.second)
				{
					ticketOFile.seekp(0, std::ios::end);
					if (ticketOFile.tellp() == 0)
						ticketOFile << ticket.first << "#" << ticket.second;
					else
						ticketOFile << std::endl << ticket.first << "#" << ticket.second;
				}
			}
		}
		else
		{
			std::ofstream ticketOFile(path);
			ticketOFile << m_rideID << "#" << 1;
		}
		return true;
	}
	else
	{
		return false;
	}
}
	
