#pragma once

#include <string>
#include <expected>


class Alpaca {
    int pwaid = 0;
    int pwatimes = 0;
    int level = 0;
    long long xp = 0;

    public:
    std::string name;
    Alpaca(const std::string& n);
    bool operator<(const Alpaca& other) const;
    void determine_levelup();
    void setid(int newid);
    void pwa(int times);
    std::expected<void, std::string> feed(int times, int herdsz);
    void play();
    void intro();
    void restorepwa (int lid, int lpwatimes, int llevel, long long lxp);
    void savepwa (std::ofstream& out);
};