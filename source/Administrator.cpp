#include "Administrator.h"
#include<fstream>
#include<iostream>
#include <cstdio>
#include"database.h"
#include <filesystem>
#include<algorithm>
#include"Ride.h"

std::vector<std::string> CodeBooks;

using namespace db;

void Administrator::generatingTravelOrder(const std::string& fileName, const Ride& ride) const noexcept(false)
{
    std::ofstream file;
    std::filesystem::path path1 = std::filesystem::current_path();
    path1 += "\\data\\rides";
    std::filesystem::create_directories(path1);
    path1 += fileName;
    file.open(path1, std::ios::app);

    std::ofstream fileArray;
    std::filesystem::path path2 = std::filesystem::current_path();
    path2 += "\\data\\rides";
    path2 += "\\AllTravelOrders.txt";
    fileArray.open(path2, std::ios::app);


    if (fileArray.good() && file.good())    // rideID, username, startTime, endTime, startLocation, pathLocations, endLocation
    {
        fileArray << fileName << std::endl;
        //file << report;
    }
    else
        throw std::runtime_error("File could not open!");
}

void Administrator::ViewUserAccount()
{
    std::cout << "User Accounts!" << std::endl;
    std::unordered_map<std::string, UserAccount> myMap = db::loadUsersFromFile();
    int i = 1;
    for (auto const& x : myMap)
    {
        std::cout <<i++<<". " << x.first << std::endl;
    };
   
}



bool Administrator::DeleteCodeBook()
{
    std::filesystem::path word1 = std::filesystem::current_path();
    word1 += "\\data\\codebooks";
    std::filesystem::create_directories(word1);
    word1 += "\\data.txt";

    std::filesystem::path word2 = std::filesystem::current_path();
    word2 += "\\data\\codebooks";
    std::filesystem::create_directories(word2);
    word2 += "\\location.txt";

    std::filesystem::path word3 = std::filesystem::current_path();
    word3 += "\\data\\codebooks";
    std::filesystem::create_directories(word3);
    word3 += "\\Bus.txt";

    std::filesystem::path word4 = std::filesystem::current_path();
    word4 += "\\data\\codebooks";
    std::filesystem::create_directories(word4);
    word4 += "\\Tour.txt";

    std::string word;
    std::ifstream Data(word1);
    int SerialNum;
    std::string DeleteFile, Check;

    std::vector<std::string> CodeBooks2;
    std::vector<std::string> CodeBooks3;
    std::vector<std::string> CodeBooks4;
    std::vector<std::string> CodeBooks5;

    while (Data >> word)
    {
        CodeBooks2.push_back(word);
    };
    Data.close();
    Data.open(word2);

    while (Data >> word)
    {
        CodeBooks3.push_back(word);
    };
    Data.close();

    Data.open(word3);
    while (Data >> word)
    {
        CodeBooks4.push_back(word);
    };

    Data.close();

    Data.open(word4);
    while (Data >> word)
    {
        CodeBooks5.push_back(word);
    };


    if (CodeBooks2.size() == 0)
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
        for (int i = 0; i < CodeBooks2.size(); i++)
        {
            std::cout << i + 1 << ". " << CodeBooks2[i] << std::endl;
        }

        do
        {
            std::cout << std::endl << " Enter the serial number of the code you are deleting" << std::endl;
            std::cin >> SerialNum;

            if (SerialNum > 0 && SerialNum <= CodeBooks2.size())
                break;

        } while (1);


        DeleteFile = CodeBooks2[SerialNum - 1];
        std::cout << "Enter yes if you are sure you want to delete?" << std::endl;
        std::cin >> Check;
        if (Check == "yes")
        {
            const char* a = DeleteFile.c_str();

            CodeBooks2.erase(CodeBooks2.begin() + (SerialNum - 1));//Brisanje iz glavnog fajla gdje su imena svih sifranika!

            auto n = std::find(CodeBooks3.begin(), CodeBooks3.end(), DeleteFile);
            if (n != CodeBooks3.end())
            {
                CodeBooks3.erase(n);
            };
            n = std::find(CodeBooks4.begin(), CodeBooks4.end(), DeleteFile);
            if (n != CodeBooks4.end())
            {
                CodeBooks4.erase(n);
            };
             n = std::find(CodeBooks4.begin(), CodeBooks4.end(), DeleteFile);
            if (n != CodeBooks4.end())
            {
                CodeBooks4.erase(n);
            };
             n = std::find(CodeBooks5.begin(), CodeBooks5.end(), DeleteFile);
            if (n != CodeBooks5.end())
            {
                CodeBooks5.erase(n);
            };
        

            for (int i = 0; i < CodeBooks2.size(); i++)
            {
                DATA << CodeBooks2[i] << std::endl;
            };
            DATA.close();

            DATA.open(word2);
            for (int i = 0; i < CodeBooks3.size(); i++)
            {
                DATA << CodeBooks3[i] << std::endl;
            };
            DATA.close();

            DATA.open(word3);
            for (int i = 0; i < CodeBooks4.size(); i++)
            {
                DATA << CodeBooks4[i] << std::endl;
            };
            DATA.close();

            DATA.open(word4);
            for (int i = 0; i < CodeBooks5.size(); i++)
            {
                DATA << CodeBooks5[i] << std::endl;
            };
            DATA.close();

            std::filesystem::path path = std::filesystem::current_path();
            path += "\\data\\codebooks\\";
            path += DeleteFile;
            path += ".txt";

            if (remove(path))
                return true;
            else
                return false;
        };
        std::cout << "Do you want to go back?" << std::endl;
        std::cout << "          yes or no    " << std::endl;
        std::cin >> Check;



        if (Check == "yes")
        {
            for (int i = 0; i < CodeBooks2.size(); i++)
            {
                DATA << CodeBooks2[i] << std::endl;
            };
            DATA.close();

            return false;
        }


    } while (1);

}

bool Administrator::ModificationCodeBoks()
{
    std::vector<std::string> CodeBooks2;
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
        CodeBooks2.push_back(word);
    };

    if (CodeBooks2.size() == 0)
    {
        std::cout << "There is no codebooks" << std::endl;
        return false;
    };
    Data.close();



    std::string type;
    do
    {
        std::cout << "Printout of all codeBooks!" << std::endl;
        for (int i = 0; i < CodeBooks2.size(); i++)
        {
            std::cout << i + 1 << ". " << CodeBooks2[i] << std::endl;
        }

        do
        {
            std::cout << std::endl << " Enter the serial number of the code you want to change!" << std::endl;
            std::cin >> SerialNum;

            if (SerialNum > 0 && SerialNum <= CodeBooks2.size())
                break;

        } while (1);

        ModifyFile = CodeBooks2[SerialNum - 1];


        std::cout << "  Do you want to add something to the code or change it completely ? " << std::endl;
        std::cout << "        modify or change  " << std::endl;
        std::cin >> Check;
        std::cin.ignore(100, '\n');
        if (Check == "modify")
        {
            std::ofstream DATA;
            DATA.open(ModifyFile, std::ios::app);

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
    data.open(word1, std::ios::app);
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


    std::filesystem::path path = std::filesystem::current_path();
    path += "\\data\\codebooks\\";
    path += name;
    path += ".txt";
    std::ofstream file;
    file.open(path);

    std::cin.ignore(100, '\n');

    if (file)
    {
        if (type == "Location")
        {

            std::filesystem::path location3 = std::filesystem::current_path();
            location3 += "\\data\\codebooks";
            std::filesystem::create_directories(location3);
            location3 += "\\location.txt";

            std::ofstream location33;
            location33.open(location3,std::ios::app);

            std::string location;
            std::string country;
            std::cout << "Enter the location where bus can go!" << std::endl;
            std::cout << "Enter END for end!" << std::endl;
            do
            {
                std::cout << "Enter the city!" << std::endl;

                getline(std::cin, location);
                if (location == "END")
                    break;
                std::cout << "Enter the country!" << std::endl;

                getline(std::cin, country);
                file << location << "#" << country << std::endl;
            } while (1);
            file.close();

            if (is_equal(name))
            {
                std::cout << "Such a codebook already exists" << std::endl;
                remove(path);
                return false;
            }
            std::cout << "You have finished entering the codebook!" << std::endl;
            data << name << std::endl;
            location33 << name<<std::endl;
            data.close();
            location33.close();
        }
        // Marka, model, godina proizvodnje, registracija
        else if (type == "Bus")
        {
            std::filesystem::path location3 = std::filesystem::current_path();
            location3 += "\\data\\codebooks";
            std::filesystem::create_directories(location3);
            location3 += "\\Bus.txt";

            std::ofstream location33;
            location33.open(location3, std::ios::app);

            std::string busBrand;
            std::string model;
            std::string yearProduction;
            std::string Registration;
            int numberOfSeats;

            std::cout << "Enter bus brand " << std::endl;

            getline(std::cin, busBrand);
            file << busBrand;

            std::cout << "Enter bus model " << std::endl;

            getline(std::cin, model);
            file << "#" << model;

            std::cout << "Enter bus the year of production" << std::endl;

            getline(std::cin, yearProduction);
            file << "#" << yearProduction;

            std::cout << "Enter registration" << std::endl;

            getline(std::cin, Registration);
            file << "#" << Registration;
            file.close();
            if (is_equal(name))
            {
                std::cout << "Such a codebook already exists" << std::endl;
                const char* a = name.c_str();
                remove(path);
                return false;
            }
            data << name << std::endl;
            location33 << name << std::endl;
            data.close();
            location33.close();
            std::cout << "You have finished entering the codebook!" << std::endl;
        }
        else
        {

            std::filesystem::path location3 = std::filesystem::current_path();
            location3 += "\\data\\codebooks";
            std::filesystem::create_directories(location3);
            location3 += "\\Tour.txt";

            std::ofstream location33;
            location33.open(location3, std::ios::app);

            std::string location;
            std::string location1;
            std::string location2;

            //file << type << std::endl;
            std::cout << "Enter start location" << std::endl;

            getline(std::cin, location1);
            // file << location1 << std::endl;

            std::cout << "Enter stop location" << std::endl;

            getline(std::cin, location2);
            // file << location2 << std::endl;

            std::cout << "Enter stops between the start and end locations!" << std::endl;
            std::cout << "Enter END for end!" << std::endl;
            file << location1;

            do
            {

                getline(std::cin, location);

                if (location == "END")
                    break;
                file << "#" << location;

            } while (1);

            file << "#" << location2;
            file.close();

            if (is_equal(name))
            {
                std::cout << "Such a codebook already exists" << std::endl;
                const char* a = name.c_str();
                remove(path);
                return false;
            }

            data << name << std::endl;
            location33 << name << std::endl;
            data.close();
            location33.close();
            std::cout << "You have finished entering the codebook!" << std::endl;
        }
    }
    else
    {
        std::cout << "Unable to open file";
        return false;
    };

    data.open(word1, std::ios::app);

    for (int i = 0; i < CodeBooks.size(); i++)
    {
        file << CodeBooks[i] << std::endl;
    };

    data.close();
    return true;

}


bool is_equal(std::string name)
{
    std::filesystem::path path = std::filesystem::current_path();
    path += "\\data\\codebooks\\";
    path += name;
    path += ".txt";
    int i = 0;
    std::vector<std::string> strings;
    std::vector<std::string> strings2;
    std::string word, word2;
    std::ifstream file, file2;

    if (CodeBooks.size() == 0)
        return false;

    while (i < CodeBooks.size())
    {
        int check2 = 0;

        std::filesystem::path path1 = std::filesystem::current_path();
        path1 += "\\data\\codebooks\\";
        path1 += CodeBooks[i++];
        path1 += ".txt";
        file2.open(path1);

        while (getline(file2, word))
        {
            strings.push_back(word);
        }
        file2.close();


        file.open(path);

        while (getline(file, word2))
        {
            strings2.push_back(word2);
        }

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

bool modify(std::string type, std::ofstream& file)
{
    /* file << std::endl;*/
    std::cin.ignore(100, '\n');
    if (type == "Location")
    {
        std::string location;
        std::string country;
        std::cout << "Enter the location where bus can go!" << std::endl;
        std::cout << "Enter END for end!" << std::endl;
        do
        {
            std::cout << "Enter the city!" << std::endl;

            getline(std::cin, location);
            if (location == "END")
                break;
            std::cout << "Enter the country!" << std::endl;

            getline(std::cin, country);
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

        getline(std::cin, busBrand);
        file << busBrand;

        std::cout << "Enter bus model " << std::endl;

        getline(std::cin, model);
        file << "#" << model;

        std::cout << "Enter bus the year of production" << std::endl;

        getline(std::cin, yearProduction);
        file << "#" << yearProduction;

        std::cout << "Enter registration" << std::endl;

        getline(std::cin, Registration);
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

        getline(std::cin, location1);
        //   file << location1 << std::endl;

        std::cout << "Enter stop location" << std::endl;

        getline(std::cin, location2);
        // file << location2 << std::endl;

        std::cout << "Enter stops between the start and end locations!" << std::endl;
        std::cout << "Enter END for end!" << std::endl;
        file << location1;

        do
        {
            std::cin.ignore();
            getline(std::cin, location);
            if (location == "END")
                break;
            file << "#" << location;

        } while (1);

        file << "#" << location2;
        std::cout << "You have finished entering the codebook!" << std::endl;
    }
    return true;
}
