#pragma once

#include <string>
#include <fstream>
#include <unordered_map>
#include <ranges>
#include <print>

namespace categories {
    inline constexpr auto cmd = "Commands";
    inline constexpr auto pwa = "Alpaca";
    inline constexpr auto spc = "Special";
}

class Gamedata {
    std::unordered_map <std::string, std::unordered_map<std::string, long long>> metadata;
public:
    long long& log(const std::string& category, const std::string& entry);
    long long see(const std::string& category, const std::string& entry) const;
    void printout (std::ofstream& out) const;
    void readin(std::ifstream& in);
    void list();
};
class GameAbstract {
    Gamedata mtd;
public:
    void logpwa(const int x);
    void loglvl(const int x);
    void logfail();
    void logcmd(const std::string& cmd);
    void loglastdaily(int date);
    void saveto(std::ofstream& out);
    void loadin(std::ifstream& in);
    void listout();
    long long getlvl() const;
    long long getpwa() const;
    long long getcmd(const std::string& cmd) const;
    long long getfail() const;
    long long getlastdaily() const;
};
extern GameAbstract meta;