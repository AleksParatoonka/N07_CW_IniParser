// N07_CW_IniParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <windows.h>
#include <algorithm>
#include <map>
#include <set>
#include <vector>

class WrongInput : public std::exception
{
public:
    const char* what() const override { return "The specified section does not exist"; }
};

class WrongFile : public std::exception
{
public:
    const char* what() const override { return "There is problem with parsing"; }
};

class WrongSection : public std::exception
{
public:
    const char* what() const override { return "The specified section does not exist"; }
};

class WrongVariable : public std::exception
{
public:
    const char* what() const override { return "There is no such variable in the specified section"; }
};

class ini_parser {
public:
    std::map<std::string, std::map<std::string, std::string>> parsed_file;
    std::string filename, input_variables, input_section;

    ini_parser(std::string filename) {
        this->filename = filename;
        this->parsing_file();
    }    

    template<class T>
    T get_value(std::string input) {
        static_assert(sizeof(T) == -1, "not implemented type for get_value");
    }

    void parsing_file() {
        std::ifstream fin(filename);
        if (!fin.is_open())
            {
                
                std::cout << "Error opening file!\n";
            }
        std::string s;
        int lines_count = 0;
        std::string current_section;
        bool reading_variables = false;

        while (!fin.eof()) {
            std::getline(fin, s);
            s = s.substr(0, s.find(";", 0));
            s.erase(0, s.find_first_not_of(" \t\n\r\f\v"));
            s.erase(s.find_last_not_of(" \t\n\r\f\v") + 1); 
            std::size_t pos1 = s.find(";");
            
            if (pos1 != std::string::npos) {
                s.erase(pos1);
            }

            if (s.size() == 0) {

                lines_count++;
            }
            else if (s[0] == '[' ) { 
                if ( s[s.size() - 1] != ']') {
                    std::cout << "Incorrect syntax --- line " << lines_count << std::endl;
                    lines_count++;
                }
                else { 
                    s = s.substr(1, s.size() - 2);
                    parsed_file.insert(std::pair<std::string, std::map<std::string, std::string>>(s, {}));
                    current_section = s;
                    lines_count++;
                }
            }
            else {
                if (current_section != "") {
                    if (s.find("=") == std::string::npos) {
                        std::cout << "Incorrect syntax --- line " << lines_count << std::endl;
                    }
                    else {                      
                        std::string variable, value;

                        variable = s.substr(0, s.find("=", 0));
                        value = s.substr(s.find("=") + 1);

                        if (value.size() != 0) {
                            parsed_file.at(current_section)[variable] = value;
                        }
                    }   
                    lines_count++;
                }
                else {
                    std::cout << "Incorrect syntax --- line " << lines_count << std::endl;
                    lines_count++;
                }   
            }
        }
        fin.close();
        
        std::cout << " Parsed file " << std::endl;
        
        for (const auto& elem : parsed_file) {
            std::cout << elem.first << ": " << std::endl;
            for (const auto& elem2 : elem.second) {
                std::cout << "    ---- " << elem2.first << " : " << elem2.second << std::endl;
            }
        }
    }
    ~ini_parser() {}

private:

    std::string get_value_string(std::string input) {
        if(parsed_file.empty() == true) throw WrongFile();
        std::string res;
        std::size_t pos = input.find(".");
        if (pos == std::string::npos) throw WrongInput();
        input_section = input.substr(0, pos);
        if (parsed_file.count(input_section) == 0) throw WrongSection();
        input_variables = input.substr(pos + 1);
        if (parsed_file[input_section].count(input_variables) == 0) throw WrongVariable();
        res = parsed_file.at(input_section).at(input_variables);

        return res;
    }
};
template<>
std::string ini_parser::get_value(std::string input) {
    return get_value_string(input);
}

template<>
int ini_parser::get_value(std::string input) {
    int res = 0;
    std::string tmp_res = get_value_string(input);
    size_t sz;
    try {
        res = std::stoi(tmp_res, &sz);
    }
    catch (...) {
        std::cout << "Value isn't number" << std::endl;
    }

    return res;
}

template<>
float ini_parser::get_value(std::string input) {
    float res = 0;
    std::string tmp_res = get_value_string(input);
    size_t sz;
    
    try {
        res = std::stof(tmp_res, &sz);
    }
    catch (...) {
        std::cout << "Value isn't number" << std::endl;
    }
    return res;
}

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);


    ini_parser parser("filename.ini");

    try {
        auto value = parser.get_value<std::string>("Section1.var3");
        //auto value = parser.get_value<int>("Section1.var7");
        //auto value = parser.get_value<float>("Section1.var1");
        std::cout << "\n value is " << value << std::endl;
    }
    catch (const WrongInput& ex) {
        std::cout << ex.what() << std::endl;
    }
    catch (const WrongSection& ex) {
        std::cout << ex.what() << std::endl;
    }
    catch (const WrongVariable& ex) {
        std::cout << ex.what() << std::endl;
    }    
    catch (const WrongFile& ex) {
        std::cout << ex.what() << std::endl;
    }
}