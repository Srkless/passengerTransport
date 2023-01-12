#include "Bus.h"

bus::bus(std::string a, std::string b, std::string c, std::string d, int e): brand(a), model(b), yearOfProduction(c), registration(d), numberOfSeats(e)
{
}

void bus::setBrand(std::string brand)
{
	this->brand = brand;
}

void bus::setModel(std::string model)
{
	this->model = model;
}

void bus::setYearOfProduction(std::string year)
{
	this->yearOfProduction = year;
}

void bus::setRegistration(std::string registration)
{
	this->registration = registration;
}

void bus::setNumberOfSeats(int number)
{
	this->numberOfSeats = number;
}

std::string bus::getBrand()
{
	return this->brand;
}

std::string bus::getModel()
{
	return this->model;
}

std::string bus::getYearOfProduction()
{
	return this->yearOfProduction;
}

std::string bus::getRegistraion()
{
	return this->registration;
}

int bus::getNumberOfSeats()
{
	return this->numberOfSeats;
}
