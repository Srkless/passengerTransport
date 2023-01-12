#include "Schedule.h"

void Schedule::addRideToSchedule(const std::string& rideID, const Ride& ride)
{
	m_Schedule[rideID] = ride;
}

void Schedule::removeRideFromSchedule(const std::string& rideID)
{
	m_Schedule.erase(rideID);
}

std::ostream& operator<<(std::ostream& os, const Schedule& schedule)
{
	for (const auto& item : schedule.m_Schedule)
	{
		os << item.second;
	};

	return os;
}

std::istream& operator>>(std::istream& is, Schedule& schedule)
{
	while (is.good())
	{
		Ride tmp;
		is >> tmp;
		schedule.m_Schedule[tmp.getRideID()] = tmp;
	}

	return is;
}
