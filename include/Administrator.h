#pragma once
#include<string>
#include<iostream>
#include<vector>


bool is_equal(std::string name);
bool modify(std::string,std::ofstream& file);

class Administrator
{

public:

	void ViewUserAccount();
	bool SuspendingUserAccount();
	bool CreateCodeBook();
	bool DeleteCodeBook();
	bool ModificationCodeBoks();

};

