#pragma once
#include <string>
#include <unordered_map>
#include "Ride.h"

class Schedule
{
private:
	std::vector<std::string> m_Schedule;
public:
	// reads Ride by ID from file and puts it into the schedule
	void addRideToSchedule(const std::string& rideID);
	void removeRideFromSchedule(const std::string& rideID);
	std::string& getRide(std::string rideID);
	friend std::ostream& operator<<(std::ostream& os, const Schedule& schedule);
};
