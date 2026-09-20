#include <string>
#include <print>
#include <format>
#include <iostream>

#include "CommandSys.hpp"
#include "AlpacaHerd.hpp"
#include "Initialization.hpp"
#include "SaveManager.hpp"
#include "Error_types.hpp"

class PWALAND {
    Herd pwaherd;
    CommandSystem cmdsys;

    void day_ends() {
        long long pwacoins = pwaherd.getsize() * 3;
        auto dialogue = util::w_rand<std::string, 330, 330, 330, 10>({
            std::format("The day ends... your alpacas had given you {} pwacoins!", pwacoins),
            std::format("Dusk and dawn, may the next day be peaceful, you have been awarded {} pwacoins!", pwacoins), 
            std::format("Sky had faded, day had ended. You are awarded with {} pwacoins!", pwacoins), 
            std::format("Secret lies upon, will you dare? You are awarded {} pwacoins...", pwacoins)
        });
        std::print("{}\n", dialogue);
        player.coinup(pwacoins);
    }

public:
    void start() {
        setup.run(cmdsys, pwaherd);
        std::string cmdline;
        int time_of_day = 0;
        std::print("\nUSER_COMMAND > ");
        while(std::getline(std::cin, cmdline)) {
            CommandSystem::State res = cmdsys.run(cmdline);
            if (res == CommandSystem::State::Ending) break;
            else if (res == CommandSystem::State::Success) {
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
