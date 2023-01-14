#include "Schedule.h"
#include "database.h"

void Schedule::addRideToSchedule(const std::string& rideID, const Ride& ride)
{
	m_Schedule[rideID] = ride;
	db::writeScheduleToFile(*this);
}

void Schedule::removeRideFromSchedule(const std::string& rideID)
{
	m_Schedule.erase(rideID);
	db::writeScheduleToFile(*this);
}

Ride& Schedule::getRide(std::string rideID)
{
	return m_Schedule[rideID];
}

std::ostream& operator<<(std::ostream& os, const Schedule& schedule)
{
	for (const auto& item : schedule.m_Schedule)
	{
		os << item.second;
	};

	return os;
}
