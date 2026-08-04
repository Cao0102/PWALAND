#include "AlpacaHerd.hpp"

Alpaca* Herd::findpwa (const std::string name) {
    auto it = pwaherd.find(name);

    if (it == pwaherd.end()) return nullptr;
    return &it->second;
}

Alpaca& Herd::addpwa (const std::string& name) {
    auto [it, _] = pwaherd.try_emplace(name, (name));
    return it->second;
}

int Herd::getsize() {return pwaherd.size();}

void Herd::intro() {for (auto& [_, pwa] : pwaherd) pwa.intro();}

void Herd::recoverpwa(int nAlpacas, std::ifstream& in) {
    for (int i = 0; i < nAlpacas; i++) {
        std::string lname;
        int lid;
        int lpwatimes;
        int llevel;
        long long lxp;
        in >> lname >> lid >> lpwatimes >> llevel >> lxp;
        addpwa(lname).restorepwa(lid, lpwatimes, llevel, lxp);
    }
}

void Herd::savepwa(std::ofstream& out) {for (auto& [name, alpaca] : pwaherd) alpaca.savepwa(out);}
void Herd::clear() {pwaherd.clear();}