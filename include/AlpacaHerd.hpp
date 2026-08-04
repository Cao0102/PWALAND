#pragma once //almost forgot

#include <unordered_map>
#include <fstream>
#include <string>

#include "Alpaca.hpp"

class Herd {
    std::unordered_map<std::string, Alpaca> pwaherd;
public:
    Alpaca* findpwa (const std::string name);
    Alpaca& addpwa (const std::string& name);
    int getsize();
    void intro();
    void recoverpwa(int nAlpacas, std::ifstream& in);
    void savepwa(std::ofstream& out);
    void clear();
};