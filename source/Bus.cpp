#include "Bus.h"

bus::bus()
	: brand(""), model(""), yearOfProduction(""), registration(""), numberOfSeats(0) {}

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

std::ostream& operator<<(std::ostream& os, const bus& a)
{
	os << a.brand << "#" << a.model << "#" << a.yearOfProduction << "#" << a.registration << "#" << a.numberOfSeats;
	return os;
}

std::istream& operator>>(std::istream& is, bus& a)
{
	std::string line;
	std::getline(is, line);
	std::stringstream sstream(line);
	std::vector<std::string> items;
	std::string item;
	size_t count = 0;
	while (std::getline(sstream, item, '#') && count < 5)
	{
		items.push_back(item);
		count++;
	}

	a.brand = items[0];
	a.model = items[1];
	a.yearOfProduction = items[2];
	a.registration = items[3];
	a.numberOfSeats = std::stoi(items[4]);

	return is;
}
