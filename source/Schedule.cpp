#include "Schedule.h"
#include "database.h"
#include <unordered_map>

void Schedule::addRideToSchedule(const std::string& rideID)
{
	m_Schedule.push_back(rideID);
}

void Schedule::removeRideFromSchedule(const std::string& rideID)
{
	m_Schedule.erase(std::remove(m_Schedule.begin(), m_Schedule.end(), rideID));
	db::writeScheduleToFile(*this);
}

std::string& Schedule::getRide(std::string rideID)
{
	auto it = std::find(m_Schedule.begin(), m_Schedule.end(), rideID);
	return m_Schedule[std::distance(m_Schedule.begin(), it)];
}

std::ostream& operator<<(std::ostream& os, const Schedule& schedule)
{
	if(schedule.m_Schedule.size() > 0)
	{
		os << schedule.m_Schedule[0];
		for (size_t i = 1; i < schedule.m_Schedule.size(); i++)
		{
			os << std::endl << schedule.m_Schedule[i];
		}
	}
	else
	{
		os << "";
	}

	return os;
}
