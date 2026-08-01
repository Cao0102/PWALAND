#include "Metadata.hpp"

long long& Gamedata::log(const std::string& category, const std::string& entry) {return metadata[category][entry];}
long long Gamedata::see(const std::string& category, const std::string& entry) const {
    auto it = metadata.find(category);
    if (it == metadata.end()) return 0;
    auto it2 = it->second.find(entry);
    if (it2 == it->second.end()) return 0;
    return it2->second;
}
void Gamedata::printout (std::ofstream& out) const {
    std::print(out, "{}\n", metadata.size());
    for (auto& [cate_name, category] : metadata) {
        std::print(out, "{} {}\n", cate_name, category.size());
        for (auto& [entry_name,amount] : category) std::print(out, "{} {}\n", entry_name, amount);
    }
}
void Gamedata::readin(std::ifstream& in) {
    int Category_count;
    std::string Category;
    int Entry_count;
    std::string Entry;
    long long Entry_Val;
    in >> Category_count;
    for (auto _ : std::views::iota(0, Category_count)) {
        in >> Category >> Entry_count;
        for (auto _ : std::views::iota(0, Entry_count)) {
            in >> Entry >> Entry_Val;
            metadata[Category][Entry] = Entry_Val;
        }
    }
}
void Gamedata::list() {
    std::print("\n\nPWA HERE IS YOUR METADATA\n===================================\n");
    std::print("\n1. Commands usage\n");
    for (auto& [name, amount] : metadata[categories::cmd]) std::print("{}: {}\n", name, amount);
    std::print("\n2. Alpaca stats\n");
    for (auto& [name, amount] : metadata[categories::pwa]) std::print("Your alpacas had {}-ed {} times!\n", name, amount);
    std::print("\n3. Exotic stats\n");
    for (auto& [name, amount] : metadata[categories::spc]) std::print("{}: {}\n", name, amount);
}

void GameAbstract::logpwa(const int x) { mtd.log(categories::pwa, "pwa") += x; }
void GameAbstract::loglvl(const int x) { mtd.log(categories::pwa, "lvl") += x; }
void GameAbstract::logfail() { mtd.log(categories::spc, "Wrong Commands")++; }
void GameAbstract::logcmd(const std::string& cmd) { mtd.log(categories::cmd, cmd)++; }
void GameAbstract::loglastdaily(int date) { mtd.log(categories::spc, "Last daily received") = date; }
void GameAbstract::saveto(std::ofstream& out) { mtd.printout(out); }
void GameAbstract::loadin(std::ifstream& in) { mtd.readin(in); }
void GameAbstract::listout() { mtd.list(); }
long long GameAbstract::getlvl() const { return mtd.see(categories::pwa, "lvl"); }
long long GameAbstract::getpwa() const { return mtd.see(categories::pwa, "pwa"); }
long long GameAbstract::getcmd(const std::string& cmd) const { return mtd.see(categories::cmd, cmd); }
long long GameAbstract::getfail() const { return mtd.see(categories::spc, "Wrong Commands"); }
long long GameAbstract::getlastdaily() const { return mtd.see(categories::spc, "Last daily received"); }

GameAbstract meta;