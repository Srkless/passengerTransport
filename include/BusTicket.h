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
	std::string m_startTime;
	std::string m_endTime;
	double m_price;
public:
	BusTicket();
	BusTicket(std::string rideID, std::string startLocation, std::string endLocation, std::string startTime, std::string endTime, bool hasBaggage = false, double price = 0);
	
	void setRideID(const std::string&);
	void setStartLocation(const std::string&);
	void setEndLocation(const std::string&);
	void setStartTime(const std::string&);
	void setEndTime(const std::string&);

	std::string getStartLocation();
	std::string getEndLocation();
	std::string getRideID();
	std::string getStartTime();
	std::string getEndTime();
	bool hasBaggage();
	double getPrice();

	double generatePrice(Ride& ride);
	void addBaggage();
	int getAvailableTickets();
	int buyTicket(UserAccount& usr, Ride& ride);
	void writeToFile(UserAccount& usr, Ride& ride);
	static BusTicket readFromFile(UserAccount& usr, std::string& name);
};