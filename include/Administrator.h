#pragma once
#include<string>
#include<iostream>
#include<vector>

bool is_equal(std::string name);
bool modify(std::string,std::ofstream& file);

class Administrator
{

	int admin = 1;
public:

	bool CreateCodeBook();
	bool DeleteCodeBook();
	bool ModificationCodeBoks();

};

