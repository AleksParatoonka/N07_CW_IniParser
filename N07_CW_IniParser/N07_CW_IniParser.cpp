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
private:
    std::string filename;

public:
    std::map<std::string, std::map<std::string, std::string>> parsed_file;
    

    ini_parser(std::string filename) {
        this->filename = filename;
        this->parsing_file();
    }

    /*void parsed_file_section_insert(std::string input) {
        parsed_file.insert(std::pair<std::string, std::map<std::string, std::string>>(input, {}));
    }
    
    void parsed_file_variable_insert(std::string section, std::string s) {
        std::string variable, value;
        
        variable = s.substr(0, s.find("=", 0));
        value = s.substr(s.find("=") + 1);
        
        if (value.size() != 0) {
            parsed_file.at(section)[variable] = value;
        }
        
    }*/

    
    template<class T>
    T get_value(std::string input) {
        T res;
        std::size_t pos = input.find(".");
        std::string input_section = input.substr(0, pos);
        std::string input_variables = input.substr(pos+1);
        //std::cout << "---res---" << std::endl;
        try {
            res = parsed_file.at(input_section).at(input_variables);
        }        
        catch (...) {
            std::cout << "Error in input string "  << std::endl;
        }
        return res;
    }

    template<>
    int get_value(std::string input) {
        int res=0;
        std::string tmp_res;
        size_t sz;
        std::size_t pos = input.find(".");
        std::string input_section = input.substr(0, pos);
        std::string input_variables = input.substr(pos+1);
        std::cout << "---res---" << std::endl;        
        std::cout << input_section << "---res---"<< input_variables << std::endl;
        try {
            tmp_res = parsed_file.at(input_section).at(input_variables);
        }
        catch (...) {
            std::cout << "Error in input string " << std::endl;         
        }
        try {
            res = std::stoi(tmp_res, &sz);
        }
        catch (...) {
            std::cout << "Value isn't number" << std::endl;
        }
        return res;
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
            //std::cout << s << std::endl;

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
                    //parsed_file_section_insert(s);
                    //parsed_file_variable_insert(current_section, "");
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
                        //parsed_file_variable_insert(current_section,  s);
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
    ~ini_parser() {
        //fin.close();
    }
};

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    ini_parser parser("filename.ini");
    auto value = parser.get_value<std::string>("Section1.var3");
    //int value = parser.get_value<int>("Section2.var1");
    std::cout << "\n value is " << value << std::endl;
}