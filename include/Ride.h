#pragma once
#include <string>
#include <chrono>
#include <vector>

class Ride
{
private:
	std::string m_RideID;
	std::string m_Driver;
	std::chrono::system_clock::time_point m_StartTime;
	std::chrono::system_clock::time_point m_EndTime;
	std::string m_StartLocation;
	std::vector<std::string> m_PathLocations;
	std::string m_EndLocation;
public:
	Ride();
	Ride(std::string rideID, std::string driver, std::chrono::system_clock::time_point startTime,
		std::chrono::system_clock::time_point endTime, std::string startLocation, std::vector<std::string> pathLocations, std::string endLocation);

	void setRideID(std::string RideID);
	void setDriver(std::string driver);
	void setStartTime(std::chrono::system_clock::time_point startTime);
	void setEndTime(std::chrono::system_clock::time_point endTime);
	void setStartLocation(std::string startLocation);
	void setEndLocation(std::string endLocation);
	void setPathLocationos(std::vector<std::string> pathLocations);

	std::string getRideID() const;
	std::string getDriver() const;
	std::chrono::system_clock::time_point getStartTime() const;
	std::chrono::system_clock::time_point getEndTime() const;
	std::string getStartLocation() const;
	std::vector<std::string> getPathLocations() const;
	std::string geEndLocation() const;
};