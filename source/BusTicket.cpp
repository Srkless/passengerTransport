#include "BusTicket.h"

BusTicket::BusTicket()
	: m_rideID(""), m_startLocation(""), m_endLocation("")
{
}

BusTicket::BusTicket(std::string rideID, std::string startLocation, std::string endLocation, std::string startTime, std::string endTime, bool hasBaggage)
	: m_rideID(rideID), m_startLocation(startLocation), m_endLocation(endLocation), m_startTime(startTime), m_endTime(endTime), m_hasBaggage(hasBaggage)
{}

void BusTicket::setRideID(const std::string& ID)
{
	m_rideID = ID;
}

void BusTicket::setStartLocation(const std::string& startLocation)
{
	m_startLocation = startLocation;
}

void BusTicket::setEndLocation(const std::string& endLocation)
{
	m_endLocation = endLocation;
}

void BusTicket::setStartTime(const std::string& time)
{
	m_startTime = time;
}

void BusTicket::setEndTime(const std::string& time)
{
	m_endTime = time;
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

std::string BusTicket::getStartTime()
{
	return m_startTime;
}

std::string BusTicket::getEndTime()
{
	return m_endTime;
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
	return (7 * route.size() + (2 * m_hasBaggage));
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
			bool flag = false;
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
					flag = true;
				}
			}
			ticketFile.close();
			if (!flag)
			{
				tickets[m_rideID] = 1;
			}
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
			ticketOFile.close();
		}
		else
		{
			std::ofstream ticketOFile(path);
			ticketOFile << m_rideID << "#" << 1;
			ticketOFile.close();
		}
		std::unordered_map<std::string, UserAccount> map = db::loadUsersFromFile();
		map[usr.getUsername()].setBalance(usr.getBalance() - price);
		db::writeUsersToFile(map);
		writeToFile(usr, ride);
		return true;
	}
	else
	{
		return false;
	}
}

void BusTicket::writeToFile(UserAccount& usr, Ride& ride)
{
	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data\\tickets\\user_tickets\\";
	std::filesystem::create_directories(path);
	path += (usr.getUsername() + ".txt");
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
		}
		ticketFile.close();

		path = std::filesystem::current_path();
		path += ("\\data\\tickets\\user_tickets\\" + usr.getUsername() + "\\");
		std::filesystem::create_directories(path);
		path += (m_rideID + "_" + std::to_string(tickets[m_rideID]) + ".txt");
		std::ofstream outFile(path);
		outFile << m_rideID << "#" << m_startLocation << "#" << m_endLocation << "#" << m_hasBaggage << "#" << ride.getStartTime() << "#" << ride.getEndTime();
		outFile.close();
	}
}

BusTicket BusTicket::readFromFile(UserAccount& usr, std::string& name)
{
	std::ifstream iFile;
	std::filesystem::path path = std::filesystem::current_path();
	path += "\\data\\tickets\\user_tickets\\" + usr.getUsername() + "\\" + name + ".txt";
	iFile.open(path);
	std::string line;
	std::getline(iFile, line);
	std::stringstream sstream(line);
	std::vector<std::string> arr;
	size_t count = 0;
	while (std::getline(sstream, line, '#') && count < 6)
	{
		arr.push_back(line);
		count++;
	}
	BusTicket tmp(arr[0], arr[1], arr[2], arr[4], arr[5], std::stoi(arr[3]));
	return tmp;
}
