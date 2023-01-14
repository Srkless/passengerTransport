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
	std::string m_startLocation;
	std::string m_endLocation;
	bool m_hasBaggage = false;
public:
	BusTicket();
	BusTicket(std::string rideID, std::string startLocation, std::string endLocation, bool hasBaggage = false);
	
	void setRideID(std::string&);
	void setStartLocation(std::string&);
	void setEndLocation(std::string&);

	std::string getStartLocation();
	std::string getEndLocation();
	std::string getRideID();
	bool hasBaggage();

	double generatePrice(Ride& ride);
	void addBaggage();
	int getAvailableTickets();
	bool buyTicket(UserAccount& usr, Ride& ride);
};