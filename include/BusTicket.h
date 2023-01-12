#pragma once
#include <vector>
#include "database.h"
#include "Ride.h"
#include "Schedule.h"
#include <unordered_map>
#include <string>
#include "UserAccount.h"


class BusTicket
{
private:
	std::string m_rideID;
	size_t m_seatNumber;
	std::string m_startLocation;
	std::string m_endLocation;
	bool m_hasBaggage = false;
public:
	BusTicket(std::string rideID, size_t seatNumber, std::string startLocation, std::string endLocation, bool hasBaggage = false);
	
	// TODO we have to check if there is room on the bus

	double generatePrice();
	void addBaggage();
	int getAvailableTickets();
	bool buyTicket(UserAccount& usr);
};