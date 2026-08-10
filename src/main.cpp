#include <string>
#include <ranges>
#include <cassert>
#include <print>
#include <array>
#include <format>
#include <iostream>

#include "CommandSys.hpp"
#include "AlpacaHerd.hpp"
#include "Initialization.hpp"
#include "SaveManager.hpp"

class PWALAND {
    Herd pwaherd;
    CommandSystem cmdsys;

    void day_ends() {
        long long pwacoins = pwaherd.getsize() * 3;
        struct Ticket {
            int weight;
            std::string dat;
        };
        std::array<Ticket,4> tickets = {{
            { 330, std::format("The day ends... your alpacas had given you {} pwacoins!", pwacoins)},
            { 330, std::format("Dusk and dawn, may the next day be peaceful, you have been awarded {} pwacoins!", pwacoins)}, 
            { 330, std::format("Sky had faded, day had ended. You are awarded with {} pwacoins!", pwacoins)}, 
            { 10, std::format("Secret lies upon, will you dare? You are awarded {} pwacoins...", pwacoins)}
        }};
        int holy_judgement = util::rng();

        int sum = 0;
        auto spincake = std::ranges::find_if(tickets, [&sum, holy_judgement](const auto& c) {sum += c.weight; return sum > holy_judgement;});
        std::string data = spincake->dat;
        std::print("{}\n", data);
        player.coinup(pwacoins);
    }

public:
    void start() {
        setup.run(cmdsys, pwaherd);
        std::string cmdline;
        int time_of_day = 0;
        std::print("\nUSER_COMMAND > ");
        while(std::getline(std::cin, cmdline)) {
            auto res = cmdsys.run(cmdline);
            if (!res) {
                std::string error_msg = res.error();
                if (error_msg == "Ending") break;
                meta.logfail();
                std::print("{}\n",error_msg);
            }
            else {
                time_of_day++;
                time_of_day %= 5;
                if (time_of_day == 0) day_ends();
            }
            Achievements.check();
            std::print("\n\nUSER_COMMAND > ");
        }

        std::print("Here is your herd info up until now\n\n");
        pwaherd.intro();
        std::print("Saving your data\n");
        Save.save(pwaherd);
        std::print("Pwa-bye! See ya again\n");
    }
};

int main() {
    PWALAND game;
    game.start();
}
