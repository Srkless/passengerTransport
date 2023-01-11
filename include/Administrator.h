#pragma once
#include<string>
#include<iostream>
#include<vector>

bool is_equal(std::string name);
bool modify(std::string,std::ofstream& file);

class Administrator
{

	int admin = 1;//Srkles pormjenjiva koja ce vidjeti dal je admim adim ili vec postoji prepravi ako imas bolje nesto ovo samo oank stavio ja
public:

	bool CreateCodeBook();
	bool DeleteCodeBook();
	bool ModificationCodeBoks();

};

