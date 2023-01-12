#include <iostream>
#include <filesystem>
#include <fstream>

class bus
{
private:
	std::string brand;
	std::string model;
	std::string yearOfProduction;
	std::string registration;
	int numberOfSeats;
public:
	bus();
	bus(std::string a, std::string b, std::string c, std::string d, int e);
	void setBrand(std::string brand);
	void setModel(std::string model);
	void setYearOfProduction(std::string year);
	void setRegistration(std::string registration);
	void setNumberOfSeats(int number);
	std::string getBrand();
	std::string getModel();
	std::string getYearOfProduction();
	std::string getRegistraion();
	int getNumberOfSeats();
	friend std::ostream& operator<<(std::ostream& os, const bus& a);
	friend std::istream& operator>>(std::istream& is, bus& a);
};