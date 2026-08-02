#include <algorithm>
#include <utility>
#include <cassert>
#include <ranges>
#include <print>

#include "Achievements.hpp"

Achievement::Achievement(std::string n, std::string d, std::vector<std::function<bool()>> c) : name(std::move(n)), description(std::move(d)), condition(std::move(c)) {};
achievement_state::Status Achievement::evaluate() {
    if (completed) return achievement_state::Status::Already;
    if (std::ranges::all_of(condition, [](auto& c) {return c();})) {completed = true; return achievement_state::Status::JustDone;}
    return achievement_state::Status::NotDone;
}
void Achievement::hide() {secret = true;}
void Achievement::info() {
    if (secret == true) return;
    std::print("\nAchievement: {}\nDescription {}\nYou have {} this achievement!\n", name, description, (completed ? "completed" : "not completed"));
}
void Achievement::complete() {std::print("\nYou have completed {}! Check AIF \"{}\" for more info!\n", name, name);}

void AchievementsManager::add(std::string name, std::string description,std::vector<std::function<bool()>> conditions){
    achievement_list.try_emplace(name, std::move(name), std::move(description), std::move(conditions));
}
void AchievementsManager::hide(const std::string& name) {
    auto hide = achievement_list.find(name);
    assert(hide != achievement_list.end());
    hide->second.hide();
}
std::vector<std::function<bool()>> AchievementsManager::requirements::pwa(int amount) {return {[amount] {return meta.getpwa() >= amount;}};}
std::vector<std::function<bool()>> AchievementsManager::requirements::fal(int amount) {return {[amount] {return meta.getfail() >= amount;}};}
std::vector<std::function<bool()>> AchievementsManager::requirements::lvl(int amount) {return {[amount] {return meta.getlvl() >= amount;}};}
std::vector<std::function<bool()>> AchievementsManager::requirements::cmd(const std::string& name, int amount) {
    return {[name, amount] {return meta.getcmd(name) >= amount;}};
}
AchievementsManager::requirements req;
void AchievementsManager::setup() {
    ///All achievements go here
    add( "First pwa", "Pwa for the first time", req.pwa(1));
    add( "More pwas", "Pwa more than 10 times", req.pwa(10));
    add( "Beginner pwa", "Pwa more than 100 times", req.pwa(100));
    add( "Intermediate pwa", "Pwa more than 1000 times", req.pwa(1'000));
    add( "Great pwa!", "Pwa more than 10000 times", req.pwa(10'000));
    add( "Master pwa!","Pwa more than 100000 times", req.pwa(100'000));
    add( "Brilliant pwa!", "Pwa more than 1 000 000 times", req.pwa(1'000'000));
    add( "Legendary pwa!", "Pwa more than 10 000 000 times", req.pwa(10'000'000));
    add( "Epic pwa!", "Pwa more than 100 000 000 times", req.pwa(100'000'000));
    add( "Mythic pwa!", "Pwa more than 200 000 000 times", req.pwa(200'000'000));
    add( "Solar pwa!", "Pwa more than 300 000 000 times", req.pwa(300'000'000));
    add( "Interstellar pwa!", "Pwa more than 400 000 000 times", req.pwa(400'000'000));
    add( "Galactic pwa!", "Pwa more than 500 000 000 times", req.pwa(500'000'000));
    add( "Intergalactic pwa!", "Pwa more than 600 000 000 times", req.pwa(600'000'000));
    add( "Void pwa!", "Pwa more than 700 000 000 times", req.pwa(700'000'000));
    add( "Universal pwa!", "Pwa more than 800 000 000 times", req.pwa(800'000'000));
    add( "Multiversal pwa!", "Pwa more than 900 000 000 times", req.pwa(900'000'000));
    add( "GOD OF PWAS", "Pwa more than 1 000 000 000 times. PWA TRANSCENDENCE", req.pwa(1'000'000'000));
    add( "Clueless beginner", "Use the HLP command", req.cmd("HLP", 1));
    add( "Forgetful man", "Use the HLP command more than 10 times", req.cmd("HLP", 10));
    add( "Confused dude", "Use the FAQ command", req.cmd("FAQ", 1));
    add( "So many weird things!", "Use the FAQ command more than 10 times", req.cmd("FAQ", 10));
    add( "Ooh? New dashboard?", "Use the MTD command", req.cmd("MTD", 1));
    add( "Obsessive checker", "Use the MTD command more than 10 times", req.cmd("MTD", 10));
    add( "Awww too little money", "Use the BAL command", req.cmd("BAL", 1));
    add( "Financially responsible", "Use the BAL command more than 10 times", req.cmd("BAL", 10));
    add( "Nhom nhom nhom", "Use the FED command", req.cmd("FED", 1));
    add( "Pwaful and well fed", "Use the FED command more than 10 times", req.cmd("FED", 10));
    add( "Pwa pwa pwa pwa pwa", "Use the PWA command", req.cmd("PWA", 1));
    add( "Pwa machine gun!", "Use the PWA command more than 10 times", req.cmd("PWA", 10));
    add( "Catch!", "Use the PLY command",req.cmd("PLY", 1));
    add( "Vet but alpaca and friendly!", "Use the PLY command more than 10 times", req.cmd("PLY", 10));
    add( "How's my alpaca?", "Use the INF command", req.cmd("INF", 1));
    add( "Know too much", "Use the INF command more than 10 times", req.cmd("INF", 10));
    add( "New pwa into town!", "Use the ADD command", req.cmd("ADD", 1));
    add( "A literal barn!", "Use the ADD command more than 10 times", req.cmd("ADD", 10));
    add( "Line... up!", "Use the LNP command", req.cmd("LNP", 1));
    add( "Into formation!", "Use the LNP command more than 10 times", req.cmd("LNP", 10));
    add( "Prizes, YAY!", "Use the DLY command", req.cmd("DLY", 1));
    add( "Daily logger", "Use the DLY command 10 times", req.cmd("DLY", 10));
    add( "Pwa-bye!", "Use the END command", req.cmd("END", 1));
    add( "Quitter!", "Use the END command more than 10 times", req.cmd("END", 10));
    add( "First command ever!", "Enter your first command", req.cmd("ALL", 1));
    add( "You got the hang of it!", "Do 10 commands", req.cmd("ALL", 10));
    add( "Master commander", "Do 100 commands", req.cmd("ALL", 100));
    add( "Definitely a spammer", "Do 1000 command", req.cmd("ALL", 1'000));
    add( "A hidden conversation", "Discover DEV command", req.cmd("DEV", 1));
    hide("A hidden conversation");
    add( "Oops... try again!", "Fail a command for the first time", req.fal(1));
    add( "Trial and error", "Fail a command ten times", req.fal(10));
    add( "Uncareful typo", "Fail a command 100 times", req.fal(100));
    add( "The epic fail", "Fail a command 1000 times", req.fal(1000));
    add( "LEVEL_UP", "Level up alpacas more than once", req.lvl(1));
    add( "Super booster!", "Level up alpacas more than 10 times", req.lvl(10));
    add( "Muscular pwarrr", "Level up alpacas more than 100 times", req.lvl(100));
    add( "PWA of a fighter!", "Level up alpacas more than 1000 times", req.lvl(1'000));
    add( "Tanky pwa, or pwa is a tank?", "Level up alpacas more than 10000 times", req.lvl(10'000));
}
std::expected<void, std::string> AchievementsManager::show (std::string& name) {
    std::print("Searching for [{}]!\n", name);
    auto it = achievement_list.find(name);
    if (it == achievement_list.end()) return std::unexpected("No such achievements\n");
    it->second.info();
    return {};
}
void AchievementsManager::list_out() {for (auto& [_, achievement] : achievement_list) achievement.info();}
void AchievementsManager::check() {
    for (auto& [_, ach] : achievement_list) {
        achievement_state::Status result = ach.evaluate();
        if (result == achievement_state::Status::JustDone) ach.complete();
    }
}
void AchievementsManager::save_sync() {for (auto& [_, achievement] : achievement_list) auto _ = achievement.evaluate();}
AchievementsManager Achievements;