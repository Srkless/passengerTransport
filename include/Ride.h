#pragma once
#include <string>
#include <filesystem>
#include <sstream>
#include <vector>

class Ride
{
private:
	std::string m_RideID;
	std::string m_Driver;
	std::string m_BusRegistration;
	std::string m_StartTime;
	std::string m_EndTime;
	std::string m_StartLocation;
	std::vector<std::string> m_PathLocations;
	std::string m_EndLocation;
	bool m_drivenStatus = false;
public:
	Ride();
	Ride(std::string rideID, std::string driver, std::string busReg, std::string startTime,
		std::string endTime, std::string startLocation, std::vector<std::string> pathLocations, std::string endLocation);
	Ride(std::string rideID, std::string driver, std::string busReg, std::string startTime,
		std::string endTime, std::string startLocation, std::string endLocation);
	void setRideID(std::string RideID);
	void setDriver(std::string driver);
	void setBusReg(std::string busReg);
	void setStartTime(std::string startTime);
	void setEndTime(std::string endTime);
	void setStartLocation(std::string startLocation);
	void setEndLocation(std::string endLocation);
	void setPathLocationos(std::vector<std::string> pathLocations);
	void changeDrivenStatus(bool value);

	std::string getRideID() const;
	std::string getDriver() const;
	std::string getBusReg() const;
	std::string getStartTime() const;
	std::string getEndTime() const;
	std::string getStartLocation() const;
	std::vector<std::string> getPathLocations() const;
	std::string geEndLocation() const;
	bool getDrivenStatus() const;

	friend std::ostream& operator<<(std::ostream& os, const Ride& ride);
	friend std::istream& operator>>(std::istream& is, Ride& ride);
};