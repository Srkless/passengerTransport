#include "Administrator.h"
#include<fstream>
#include<iostream>
#include <cstdio>
#include <iostream>
#include <functional>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

std::vector<std::string> CodeBooks;

bool Administrator::DeleteCodeBook()
{
    std::filesystem::path word1 = std::filesystem::current_path();
    word1 += "\\data\\codebooks";
    std::filesystem::create_directories(word1);
    word1 += "\\data.txt";
    std::string word;
    std::ifstream Data(word1);
    int SerialNum;
    std::string DeleteFile,Check;

    while (Data >> word)
    {
        CodeBooks.push_back(word);
    };

    if (CodeBooks.size() == 0)
    {
        std::cout << "There is no codebooks" << std::endl;
        return false;
    };

    Data.close();
    std::ofstream DATA;
    DATA.open(word1);
    do
    {
        std::cout << "Printout of all codeBooks!" << std::endl;
        for (int i = 0; i < CodeBooks.size(); i++)
        {
            std::cout << i + 1 << ". " << CodeBooks[i] << std::endl;
        }
        std::cout << std::endl << " Enter the serial number of the code you are deleting" << std::endl;
        std::cin >> SerialNum;
        DeleteFile = CodeBooks[SerialNum - 1];
        std::cout << "Enter yes if you are sure you want to delete?" << std::endl;
        std::cin >> Check;
        if (Check == "yes")
        {
            const char* a = DeleteFile.c_str();

            CodeBooks.erase(CodeBooks.begin() + (SerialNum - 1));//Brisanje iz glavnog fajla gdje su imena svih sifranika!
            for (int i = 0; i < CodeBooks.size(); i++)
            {
                DATA << CodeBooks[i]<<std::endl;
            };

            if (remove(a))
                return true;
            else
                return false;
        };
        std::cout << "Do you want to go back?" << std::endl;
        std::cout << "          yes or no    " << std::endl;
        std::cin >> Check;
        if(Check=="yes")
        {
            return false;
        }
    } while (1);

}

bool Administrator::ModificationCodeBoks()
{
    std::filesystem::path word1 = std::filesystem::current_path();
    word1 += "\\data\\codebooks";
    std::filesystem::create_directories(word1);
    word1 += "\\data.txt";
    std::string word;
    std::ifstream Data(word1);
    int SerialNum;
    std::string ModifyFile, Check;

    while (Data >> word)
    {
        CodeBooks.push_back(word);
    };

    if (CodeBooks.size() == 0)
    {
        std::cout << "There is no codebooks" << std::endl;
        return false;
    };
    Data.close();
    std::string type;
    do
    {
        std::cout << "Printout of all codeBooks!" << std::endl;
        for (int i = 0; i < CodeBooks.size(); i++)
        {
            std::cout << i + 1 << ". " << CodeBooks[i] << std::endl;
        }
        std::cout << std::endl << " Enter the serial number of the code you want to change!" << std::endl;
        std::cin >> SerialNum;
        ModifyFile = CodeBooks[SerialNum - 1];

        std::cout << "  Do you want to add something to the code or change it completely ? " << std::endl;
        std::cout << "        modify or change  " << std::endl;
        std::cin >> Check;

        if (Check == "modify")
        {
            std::ofstream DATA;
            DATA.open(ModifyFile,std::ios::app);

            do
            {
                std::cout << "Enter the type of CodeBook!" << std::endl << "1.Location!" << std::endl << "2.Bus" << std::endl << "3.Tour" << std::endl;
                std::cin >> type;
                if (type == "Location" || type == "Bus" || type == "Tour")
                {
                    break;
                }
                else
                    std::cout << "Doesn't exist this type od CodeBook!" << std::endl;

               
            } while (1);
             modify(type, DATA);
             DATA.close();

        }
        else if (Check == "change")
        {
            std::ofstream DATA;
            DATA.open(ModifyFile);
            do
            {
                std::cout << "Enter the type of CodeBook!" << std::endl << "1.Location!" << std::endl << "2.Bus" << std::endl << "3.Tour" << std::endl;
                std::cin >> type;
                if (type == "Location" || type == "Bus" || type == "Tour")
                {
                    break;
                }
                else
                    std::cout << "Doesn't exist this type od CodeBook!" << std::endl;
               
            } while (1);
            modify(type, DATA);
            DATA.close();
        }

        std::cout << "Enter 'End' if you're done!" << std::endl;
        std::cin >> Check;
        if (Check == "End")
            break;

    } while (1);
    return true;
}





bool Administrator::CreateCodeBook()
{
    std::filesystem::path word1 = std::filesystem::current_path();
    word1 += "\\data\\codebooks";
    std::filesystem::create_directories(word1);
    word1 += "\\data.txt";
    std::string word;
    std::ifstream Data(word1);
    
    while (Data >> word)
    {
        CodeBooks.push_back(word);
    };
    Data.close();
    std::string name;
    std::ofstream data;
    std::string type;
    data.open(word1,std::ios::app);
    do
    {
        std::cout << "Enter the type of CodeBook!" << std::endl << "1.Location!" << std::endl << "2.Bus" << std::endl << "3.Tour" << std::endl;
        std::cin >> type;
        if (type == "Location" || type == "Bus" || type == "Tour")
        {
            break;
        }
        else
            std::cout << "Doesn't exist this type od CodeBook!" << std::endl;
    } while (1);
    int t = 0;
    do
    {
        std::cout << "Enter the codeook name!" << std::endl;
        std::cin >> name;
        t = 0;
        for (int i = 0; i < CodeBooks.size(); i++)
        {
            if (name == CodeBooks[i])
            {
                std::cout << "This codeBooks already exists!" << std::endl;
                t++;
            }
        }
        if (t == 0)
            break;

    } while (1);

   

    std::ofstream file;
    file.open(name);
    
    if (file)
    {
        if(type=="Location")
        {
            std::string location;
            std::string country;
            std::cout << "Enter the location where bus can go!" << std::endl;
            std::cout << "Enter END for end!" << std::endl;
            do
            {
                std::cout <<"Enter the city!" << std::endl;
                std::cin >> location;
                if (location == "END")
                    break;
                std::cout << "Enter the country!" << std::endl;
                std::cin >> country;
                file <<location << "#" << country << std::endl;
            } while (1);
            file.close();

            if(is_equal(name))
            {
                std::cout << "Such a codebook already exists";
                const char* a = name.c_str();
                remove(a);
                return false;
            }
            std::cout << "You have finished entering the codebook!" << std::endl;
            data << name << std::endl;
            data.close();
        }
       // Marka, model, godina proizvodnje, registracija
        else if (type == "Bus")
        {
            std::string busBrand;
            std::string model;
            std::string yearProduction;
            std::string Registration;

            std::cout << "Enter bus brand " << std::endl;
            std::cin >> busBrand;
            file << busBrand;

            std::cout << "Enter bus model " << std::endl;
            std::cin >> model;
            file <<"#" << model;

            std::cout << "Enter bus the year of production" << std::endl;
            std::cin >>yearProduction;
            file << "#" <<yearProduction;

            std::cout << "Enter registration" << std::endl;
            std::cin >> Registration;
            file << "#" << Registration;
            file.close();
            if (is_equal(name))
            {
                std::cout << "Such a codebook already exists" << std::endl;
                const char* a = name.c_str();
                remove(a);
                return false;
            }
            data << name << std::endl;
            data.close();
            std::cout << "You have finished entering the codebook!" << std::endl;
        }
        else 
        {
            std::string location;
            std::string location1;
            std::string location2;

            //file << type << std::endl;
            std::cout << "Enter start location" << std::endl;
            std::cin >> location1;
            file << location1 << std::endl;

            std::cout << "Enter stop location" << std::endl;
            std::cin >> location2;
            file << location2 << std::endl;

            std::cout << "Enter stops between the start and end locations!" << std::endl;
            std::cout << "Enter END for end!" << std::endl;
            file << location1;

            do
            {
                std::cin >> location;
                if (location == "END")
                    break;
                file << "#" << location;

            } while (1);

            file <<"#"<< location2;
            file.close();

            if (is_equal(name))
            {
                std::cout << "Such a codebook already exists" << std::endl;
                const char* a = name.c_str();
                remove(a);
                return false;
            }

            data << name << std::endl;
            data.close();
            std::cout << "You have finished entering the codebook!" << std::endl;
        }
    }
    else
    {
        std::cout << "Unable to open file";
        return false;
    }

    return true;

}

//REsiti da provjerava sve!
bool is_equal( std::string name)
{
    int i = 0;
    std::vector<std::string> strings;
    std::vector<std::string> strings2;
    std::string word,word2;
    std::ifstream file,file2;
    
    if (CodeBooks.size() == 0)
        return false;
 
    while (i < CodeBooks.size())
    {
        int check2 = 0;
        file2.open(CodeBooks[i++]);

        while (getline(file2, word))
        {
            strings.push_back(word);
        }
        file2.close();

 
        file.open(name);
        while (getline(file, word2))
        strings2.push_back(word2);
       
        file.close();
        

        
      
        int check = 0;
        for (int j = 0; j < strings.size(); j++)
        {
          
            word = strings2[j];

            if (strings.size() != strings2.size())
                break;


            for (int k = 0; k < strings.size(); k++)
            {
                if (word == strings[k])
                    check++;
            }
            if (check != 0)
                check2++;
 
            check = 0;
        }
      
        if (check2 == strings2.size())
        {
            return true;
        };

        check2 = 0;

    };
    
    return false;
}

bool modify(std::string type,std::ofstream& file)
{
    file << std::endl;
    if (type == "Location")
    {
        std::string location;
        std::string country;
        std::cout << "Enter the location where bus can go!" << std::endl;
        std::cout << "Enter END for end!" << std::endl;
        do
        {
            std::cout << "Enter the city!" << std::endl;
            std::cin >> location;
            if (location == "END")
                break;
            std::cout << "Enter the country!" << std::endl;
            std::cin >> country;
            file << location << "#" << country << std::endl;

        } while (1);
       

        std::cout << "You have finished entering the codebook!" << std::endl;
        
    }
    // Marka, model, godina proizvodnje, registracija
    else if (type == "Bus")
    {
        std::string busBrand;
        std::string model;
        std::string yearProduction;
        std::string Registration;

        std::cout << "Enter bus brand " << std::endl;
        std::cin >> busBrand;
        file << busBrand;

        std::cout << "Enter bus model " << std::endl;
        std::cin >> model;
        file << "#" << model;

        std::cout << "Enter bus the year of production" << std::endl;
        std::cin >> yearProduction;
        file << "#" << yearProduction;

        std::cout << "Enter registration" << std::endl;
        std::cin >> Registration;
        file << "#" << Registration;
       
        std::cout << "You have finished entering the codebook!" << std::endl;
    }
    else
    {
        std::string location;
        std::string location1;
        std::string location2;

        //file << type << std::endl;
        std::cout << "Enter start location" << std::endl;
        std::cin >> location1;
        file << location1 << std::endl;

        std::cout << "Enter stop location" << std::endl;
        std::cin >> location2;
        file << location2 << std::endl;

        std::cout << "Enter stops between the start and end locations!" << std::endl;
        std::cout << "Enter END for end!" << std::endl;
        file << location1;

        do
        {
            std::cin >> location;
            if (location == "END")
                break;
            file << "#" << location;

        } while (1);

        file << "#" << location2;
        std::cout << "You have finished entering the codebook!" << std::endl;
    }
    return true;
}
