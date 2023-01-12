#pragma once
#include <string>
#include <unordered_map>
#include "Ride.h"

class Schedule
{
private:
	std::unordered_map<std::string, Ride> m_Schedule;
public:
	// reads Ride by ID from file and puts it into the schedule
	void addRideToSchedule(const std::string& rideID, const Ride& ride);
	void removeRideFromSchedule(const std::string& rideID);
	Ride& getRide(std::string rideID);
	friend std::ostream& operator<<(std::ostream& os, const Schedule& schedule);
	friend std::istream& operator>>(std::istream& is, Schedule& schedule);
};
