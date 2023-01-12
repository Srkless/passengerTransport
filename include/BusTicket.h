#pragma once
#include <vector>
#include "database.h"
#include "Ride.h"
#include "Schedule.h"
#include <unordered_map>
#include <string>


class BusTicket
{
private:
	std::string m_rideID;
	size_t m_seatNumber;
	std::string m_startLocation;
	std::string m_endLocation;
	bool m_hasBaggage = false;
	double m_price;
public:
	BusTicket(std::string rideID, size_t seatNumber, std::string startLocation, std::string endLocation, double price = 0, bool hasBaggage = false);
	
	void generatePrice();
	void addBaggage();
};