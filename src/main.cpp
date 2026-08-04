#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <expected>
#include <ranges>
#include <cassert>
#include <print>
#include <array>
#include <filesystem>

#include "Constants.hpp"
#include "Utilities.hpp"
#include "Metadata.hpp"
#include "Achievements.hpp"
#include "Player.hpp"
#include "CommandSys.hpp"
#include "AlpacaHerd.hpp"
#include "Games.hpp"
#include "Onboarding.hpp"
#include "SaveManager.hpp"

class Config {
    CommandSystem setup_commands(Herd& pwaherd) {
        CommandSystem cmdsys;
        cmdsys.add("HLP", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            std::print(R"(

COMMANDS GUIDE
----------------------------------

1. General commands
HLP - The help command, show command details
FAQ - Common questions and errors
MTD - Shows some data about your gameplay.
END - Stop the program and exit, progress is indeed saved

2. Player information
BAL - Show your pwacoin balance!
ACH - Show all achievements
AIF "<name>" - View information on a specific achievement
DLY - Get the daily reward!

3. Alpaca interactions
INF <name> - Show information on your alpaca
FED <name> <amount> - Feed the alpacas, may they pwa more and be well fed
PWA <name> <amount> - Let them PWA!
PLY <name> - Play with your alpaca!
ADD <name> - Grow your herd! Add another alpaca!
LNP - Show your alpaca formation!

4. Games
ADV - Play an all-or-nothing game answering series of questions for a big prize

More coming soon! =)
)");
            return {};
        });

        cmdsys.add("MTD", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            meta.listout();
            return {};
        });


        cmdsys.add("FAQ", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            std::print (R"( 

Frequently Asked Questions
==================================

Q: Oh no my save / load is not working, is there any way to recover this?
A: Right now, unfortunately not. 
This could happen because the save file was deleted or because of an internal program error. =(
    If this were load, try running the file again. 
    If this is save, then there is sadly no way to recover your progress.
One way to avoid this is to save regularly and do not make changes to the project folder. 
Always use the END command to save before closing the terminal.
Progress will only be saved when using this command.

Q: I entered the right arguments, why does it say no such alpaca / command exists?
A: There is a good chance you are mixing lowercase and uppercase.
    If you are entering a command, all letters uppercase
    If you are entering an alpaca, case sensitive and be exact
    This is another reason to enter a memorable and easy to type alpaca name

Q: Is there any help entering achievement names? It's hard to enter!
A: No, at least not yet, for now:
    Always surround the names with quotes ("), this avoids parsing problems
    Beware of case sensitivity
    The program will show "Searching for: [Your_Input]" to help
More recently, parser was updated to ignore leading and trailing spaces to help safer parsing

)");
            return {};
        });

        cmdsys.add("FED", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 3) return std::unexpected(util::Argnum_err(2, args.size()-1));
            std::string& targetname = args[1];
            Alpaca* pwatarg = pwaherd.findpwa(targetname);
            if (!pwatarg) return std::unexpected(util::Nopwa_err());

            auto numres = util::parse_num(args[2]);
            if (!numres) return std::unexpected(numres.error());
            int amount = numres.value();

            auto working = pwatarg->feed(amount, pwaherd.getsize());
            if (!working) return std::unexpected(working.error());
            return {};
        });

        cmdsys.add("PWA", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 3) return std::unexpected(util::Argnum_err(2, args.size()-1));
            std::string targetname = args[1];
            Alpaca* pwatarg = pwaherd.findpwa(targetname);
            if (!pwatarg) return std::unexpected(util::Nopwa_err());

            auto numres = util::parse_num(args[2]);
            if (!numres) return std::unexpected(numres.error());
            int times = numres.value();
            pwatarg->pwa(times);
            return {};
        });

        cmdsys.add("PLY", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 2) return std::unexpected(util::Argnum_err(1, args.size()-1));
            std::string targetname = args[1];
            Alpaca* pwatarg = pwaherd.findpwa(targetname);
            if (!pwatarg) return std::unexpected(util::Nopwa_err());
            auto res = player.coindown(10);
            if (!res) return std::unexpected(res.error());
            pwatarg->play();
            return {};
        });

        cmdsys.add("INF", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 2) return std::unexpected(util::Argnum_err(1, args.size()-1));
            std::string pwaname = args[1];
            auto it = pwaherd.findpwa(pwaname);
            if (!it) return std::unexpected(util::Nopwa_err());
            it->intro();
            return {};
        });

        cmdsys.add("ADD", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 2) return std::unexpected(util::Argnum_err(1, args.size()-1));
            std::string pwaname = args[1];
            auto it = pwaherd.findpwa(pwaname);
            if (it) return std::unexpected("That alpaca already exists!");
            long long cost = 25 + 15 * (pwaherd.getsize()-1);
            auto res = player.coindown(cost);
            if (!res) return std::unexpected(res.error());
            std::print("Adding alpaca {} into your herd!\n\n", pwaname);
            pwaherd.addpwa(pwaname).setid(pwaherd.getsize());
            std::print("\nThe herd continues to grow\nYou now have {} alpacas!\n", pwaherd.getsize());
            return {};
        });

        cmdsys.add("BAL", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            long long balance = player.getBalance();
            std::print("Your balance is {} pwacoins\n", balance);
            return {};
        });

        cmdsys.add("LNP", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            std::print("March! March! Pwa... Introduce!\nPwacount: {}!\n\n", pwaherd.getsize());
            pwaherd.intro();
            return {};
        });

        cmdsys.add("ACH", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            std::print("\nPWA ACHIEVEMENTS!\n=======================================\n\n");
            Achievements.list_out();
            return {};
        });

        cmdsys.add("AIF", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
            //this one is a pain in hell
            if (args.size() != 2) return std::unexpected(util::Argnum_err(1, args.size()-1));
            std::string target = args[1];
            auto res = Achievements.show(target);
            if (!res) return std::unexpected(res.error());
            return {};
        });

        cmdsys.add("ADV", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            long long Entryfee = 100;
            int times = meta.getcmd("ADV");
            while(times--) Entryfee = Entryfee*21/20;
            auto result = player.coindown(Entryfee);
            if (!result) return std::unexpected(result.error());
            games.adventure(Entryfee);
            return {};
        });

        cmdsys.add("DLY", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            int lastDaily = meta.getlastdaily();
            if (util::get_date() - lastDaily < 1) return std::unexpected("Awww you already take your daily rewards today...");
            struct ticket {
                int chance;
                int pwacoins;
            };
            constexpr static std::array<ticket, 4> chance = {{{100, 220}, {200, 200}, {300, 150}, {400, 80}}};
            int result = util::rng();
            int sum = 0;
            auto reward_place = std::ranges::find_if(chance, [&sum, result](const ticket& slot) {sum += slot.chance; return sum > result;});
            assert(reward_place != chance.end());
            int reward = reward_place->pwacoins;
            std::print("You got... {} PWACOINS! Come back tomorrow for more prices!\n", reward);
            player.coinup(reward);
            meta.loglastdaily(util::get_date());
            return {};
        });

        cmdsys.add("DEV", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
            /// THIS COMMAND IS HIDDEN AND DELIBERATELY UNDOCUMENTED
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            std::print(R"(
Oh... You are here? Interesting... Let's break away from the normal game for a moment and talk shall we?
How did you find this?
    [Someone told me/By accident/I datamined it]
> )");
            std::string response;
            std::getline(std::cin, response);
            if (response == "Someone told me") std::print("Fair enough\n");
            else if (response == "By accident") std::print("Accident? That's sus...\n");
            else if (response == "I datamined it") std::print("...\nOf course....\nNot... suprising at all...\n");
            else std::print("Despite everything, you didn't follow the very clear written instructions.... \nYou are not my QA\n");
            std::print(R"(
Whatever that was... Should it matter?
After all... this is my game to discover
A first project I am dedicated in...
So...
Do you like it? [Y/N]
> )");
            while(std::getline(std::cin, response)) {
                if (response == "Y") {std::print("Thank you, may you keep playing as happy\n"); break;}
                else if (response == "N") std::print("Wrong answer, try again\n");
                else std::print("As I said, you are not my QA >:( stop sending unfun responses\n> ");
            }
            std::print("I guess... Bye!\n");
            return {};
        });

        cmdsys.add("END", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() != 1) return std::unexpected(util::Argnum_err(0, args.size()-1));
            std::print("Pwa, goodbye that fast?\n");
            return std::unexpected("Ending");
        });

        cmdsys.add("ADMIN", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
            if (args.size() == 2 && args[1] == "Save clear") {
                std::print("\n[ADMIN] Clearing save\nRecommend pwa to restart the game\n");
                std::filesystem::remove("save1.txt");
            }
            else {
                std::print(R"(
Pwa... No such admin command exists
Here are the existing commands!

ADMIN "Save clear"
With double quotes!
Clear the save without having to go through the normal pathway
)");
            }
            return {};
        });

        return cmdsys;
    }
public:
    void run(CommandSystem& cmdsys, Herd& Alpacaherd) {
        cmdsys = setup_commands(Alpacaherd);
        Achievements.setup();
        Save.savecheck(Alpacaherd);
    }
};
Config setup;

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
    void mainpart() {
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
    game.mainpart();
}
