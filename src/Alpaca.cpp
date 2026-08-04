#include <cmath>
#include <ranges>
#include <cassert>
#include <print>
#include <array>
#include <fstream>

#include "Alpaca.hpp"
#include "Player.hpp"
#include "Metadata.hpp"
#include "Utilities.hpp"

Alpaca::Alpaca(const std::string& n) : name(n) {}

bool Alpaca::operator<(const Alpaca& other) const {
    return name < other.name;
}
void Alpaca::determine_levelup() {
    long long xp_required = (level * (long long) std::log2(level + 5)) * 2;
    std::print("Pwa {} is at level {}, [{}/{}]!\n", name, level, xp, xp_required);
    while (xp >= xp_required) {
        level++;
        meta.loglvl(1);
        xp -= xp_required;
        xp_required = (level * (long long) std::log2(level + 5)) * 2;
        std::print("YEAH! Pwa {} had leveled up to level {}, [{}/{}]!\n", name, level, xp, xp_required);
        pwatimes++;
    }
}

void Alpaca::setid(int newid) {
    std::print("Pwa hi! Pwa's id is {}!\n", newid);
    pwaid = newid;
    pwatimes += 2;
    meta.logpwa(2);
}

void Alpaca::pwa(int times) {
    std::print("{} pwaing!\n", name);
    pwatimes += times;
    meta.logpwa(times);
    for (int i = 0; i < times; i++) std::print("pwa ");
    std::print("\n");
}

std::expected<void, std::string> Alpaca::feed(int times, int herdsz) {
    long long cost;
    cost = meta.getlvl() + herdsz*5 - std::min(meta.getpwa()/50000, (long long) herdsz*3);
    auto res = player.coindown(cost);
    if (!res) return std::unexpected (res.error());
    std::print("Pwa, really? You are giving me {} food? THANK YOU PWA!\n\n", times);
    while (times--) {
        pwatimes += 2;
        meta.logpwa(2);
        xp += 5;
    }
    determine_levelup();
    return {};
}

void Alpaca::play() {
    std::print("Playing with {} PWA PWA!\n", name);
    struct ticket {
        int chance;
        int pwa;
        int exp;
    };
    int destiny = util::rng();
    constexpr static std::array<ticket, 6> wheel {{ {150, 20, 20}, {200, 15, 20}, {250, 20, 15}, {300, 10, 10}, {90, 25, 25}, {10, 100, 100} }};
    int sum = 0;
    auto reward_place = std::ranges::find_if(wheel, [&sum, destiny](const ticket& slot) {sum += slot.chance; return sum > destiny;});
    assert(reward_place != wheel.end());

    auto reward = *reward_place;
    int pwaadded = reward.pwa;
    int xpadded = reward.exp;
    std::print("Pwa is very happy! You got {} xp and {} pwas!\n", xpadded, pwaadded);
    pwatimes += pwaadded;
    xp += xpadded;
    meta.logpwa(pwaadded);
    determine_levelup();
    return;
}

void Alpaca::intro() {
    struct ticket {
        int chance;
        int pwa;
        std::string contents;
    };

    int destiny = util::rng();

    const std::array<ticket,8> wheel = {{
        { 240, 6, std::format("PWA! Pwa's name is {}, pwa's id is {}, pwa is level {} + {} xp, and pwa had pwa-ed {} times", name, pwaid, level, xp, pwatimes)}, 
        { 240, 9, std::format("pwa pwa pwa... pwa is {}, with id {} and pwa pwa level {} + {} xp, pwa pwa pwa-ed {} times", name, pwaid, level, xp, pwatimes)}, 
        { 240, 9, std::format("PWA! PWA! PWA! NAME {}! ID {}! LEVEL {}! XP {}! PWATIMES {}! PWA! PWA! PWA! PWA! PWA!", name, pwaid, level, xp, pwatimes)},
        { 240, 6, std::format("PWA PWa Pwa pwa... name is {}, ID is {}, level is {} + {} xp, and pwa pwa-ed {} times", name, pwaid, level, xp, pwatimes)}, 
        { 10, 20, std::format("PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA {}", name)}, 
        { 10, 10, std::format("Pwa... {} here! Pwa??? Pwa pwa pwa pwa no tell >:(, pwa ha ha ha ha pwa pwa pwa *eat grass*", name)}, 
        { 10, 8, std::format("And it's AL - PA - CA TIME for pwa to PWA PWA PWA pwa pwa. Pwa name is {} pwa", name)}, 
        { 10, 10, std::format("PWA HA HA HA HA! PWA WILL PWA EVERYTHING INTO PWAS AND PWAS PWA PWA PWA. Know PWA! Pwa is {}", name)}
    }};

    int sum = 0;
    auto star_result = std::ranges::find_if(wheel, [&sum, &destiny](const auto& c) {sum += c.chance; return sum > destiny;});
    assert(star_result != wheel.end());
    auto result = *star_result;
    pwatimes += result.pwa;
    meta.logpwa(result.pwa);
    std::print("{}\n",result.contents);
    return;
}
void Alpaca::restorepwa (int lid, int lpwatimes, int llevel, long long lxp) {
    pwaid = lid;
    pwatimes = lpwatimes;
    level = llevel;
    xp = lxp;
}

void Alpaca::savepwa (std::ofstream& out) {std::print(out, "{} {} {} {} {}\n", name, pwaid, pwatimes, level, xp);}