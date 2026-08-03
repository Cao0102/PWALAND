#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <expected>
#include <ranges>
#include <cassert>
#include <print>
#include <array>

#include "constants.hpp"
#include "utilities.hpp"
#include "Metadata.hpp"
#include "Achievements.hpp"
#include "Player.hpp"
#include "Alpaca.hpp"

class CommandSystem {
public:
    using Command = std::function<std::expected<void, std::string>(std::vector<std::string>& args)>;
private:
    std::unordered_map<std::string, Command> commands;
public:
    void add(std::string name, Command command) {
        commands.emplace(name, command);
    }

    Command* find (const std::string& name) {
        auto it = commands.find(name);
        if (it == commands.end()) return nullptr;
        else return &it->second;
    }

    std::expected<std::vector<std::string>, std::string> parse(std::string& cmd) {
        //ignores all spaces, supports quoted strings
        if (cmd.empty()) return std::unexpected("\'Mpty line PWA!\n");
        std::vector<std::string> a;
        std::string cur;
        bool iQuoted = false;
        a.reserve(10);
        for (char c : cmd) {
            if (iQuoted && c == '"') {
                iQuoted = false; 
                a.push_back(cur); 
                cur.clear();
                if (a.size() > 10) return std::unexpected("Hey! PWA DIZZY! TOO MANY ARGUMENTS\n");
                continue;
            }
            if (c == '"') {iQuoted = true; continue;}
            if (c != ' ' || iQuoted) {cur.push_back(c); continue;}
            if (!cur.empty()) {
                a.push_back(cur);
                cur.clear();
                if (a.size() > 10) return std::unexpected("Hey! PWA DIZZY! TOO MANY ARGUMENTS\n");
            }
        }
        if (!cur.empty()) a.push_back(cur);
        if (a.size() > 10) return std::unexpected("Hey! PWA DIZZY! TOO MANY ARGUMENTS\n");
        return a;
    }

    std::expected<void, std::string> run(std::string& cmd) {
        auto parseres = parse(cmd);
        if (!parseres) return std::unexpected(parseres.error());
        auto& args = parseres.value();
        auto check = find(args[0]);
        if (!check) return std::unexpected("No such command exists!");
        auto cmdrun = *check;
        auto res = cmdrun(args);
        if (!res) return std::unexpected(res.error());
        meta.logcmd("ALL");
        meta.logcmd(args[0]);
        return {};
    }
};

class herd {
    std::unordered_map<std::string, Alpaca> pwaherd;
public:
    Alpaca* findpwa (const std::string name) {
        auto it = pwaherd.find(name);

        if (it == pwaherd.end()) return nullptr;
        return &it->second;
    }

    Alpaca& addpwa (const std::string& name) {
        auto [it, _] = pwaherd.try_emplace(name, (name));
        return it->second;
    }

    int getsize() {return pwaherd.size();}

    void intro() {for (auto& [_, pwa] : pwaherd) pwa.intro();}

    void recoverpwa(int nAlpacas, std::ifstream& in) {
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

    void savepwa(std::ofstream& out) {for (auto& [name, alpaca] : pwaherd) alpaca.savepwa(out);}
    void clear() {pwaherd.clear();}
};

class Gameplay {
    // This class contains the special gameplay elements that are not significant enough to have a seperate class
public:
    void adventure(long long Entry) {
        const int base_chance = 200; //for now
        int Coins_Gained = 0;
        std::print("\nAdventure time!\n===================================\n\n");
        struct Question {
            int chance;
            std::string Q;
            std::string A;
            std::string B;
            std::string C;
            std::string D;
            char Ans;
            std::string Correct;
            std::string Wrong;
        };
        // Alright, here we go
        std::vector<Question> Q_set = {
            {
                base_chance,
                "Night falls. It's time to sleep. You have 83 alpacas, and each inn can fit a maximum of 7 alpacas, and you must not sleep with the alpacas, "
                "how many inns do you need to accompany everyone?", "A. 10", "B. 11", "C. 12", "D. 13", 'D', "Everyone slept well =)", 
                "You bought the wrong amount of inns, now the alpacas want to go home."
            }, {
                base_chance,
                "You find yourself arguing with a shepherd, who believes that sheep are better than alpacas (unacceptable), which argument do "
                "you make?", "A. Sheep are too ugly.", "B. Nobody likes sheep.", "C. Alpacas produce premium fiber.", "D. Sheep hate humans.", 'C', 
                "You won the argument, your alpacas give you a happy nod.", "The shepherd laughs at you, the alpacas are embarrassed and disappointed"
            }, {
                base_chance,
                "A merchant walks by and says he is willing to sell you an extra alpaca for only 20 pwacoins (a fantastic deal), what will you ask "
                "about the alpaca?", "A. Its color", "B. Whether it pwas", "C. Its appetite", "D. Is it raining tacos?", 'C', 
                "The sale goes through. The merchant explains everything thoroughly", "The merchant looks confused by your priorities"
            }, {
                base_chance,
                "A man saw your massive alpaca herd and immediately asked you for advice on how to manage livestock, how do you respond?",
                "A. Skill issue, get better", "B. Get emotionally attached", "C. Say nothing, just laugh", "D. Walk away", 'B', 
                "The man thanks you for your advice", "He gets confused and walks away, wondering why you are so rude?"
            }, {
                base_chance,
                "Two of your alpacas are arguing over who pwaed more this year loudly and is disrupting everyone, what will you do?",
                "A. Show the pwaing leaderboard", "B. Just ignore it", "C. Tell them the discussion is pointless", "D. Argue with them", 'A', 
                "The conflict is eventually resolved, everyone moves on", "That did nothing, the argument spreads and everyone is arguing"
            }
        };
        for (int i : std::views::iota(1,6)) {
            int result = util::rng();
            auto& Q = Q_set[result/base_chance - (result == 1000 ? 1 : 0)];
            std::print("Question {}:\n{}\n\nWhich option will you choose [A/B/C/D]?\n{}\n{}\n{}\n{}\n\nANSWER > ", i, Q.Q, Q.A, Q.B, Q.C, Q.D);
            std::string answer;
            std::getline(std::cin, answer);
            if (answer.size() != 1 || answer[0] != 'A' && answer[0] != 'B' && answer[0] != 'C' && answer[0] != 'D') {
                std::print("Pwa? That is not an answer! The alpacas are disappointed, you cannot even answer properly!\nYou lost the adventure!");
                return;
            }
            if (answer[0] != Q.Ans) {
                std::print("{}\nYou lost the adventure!", Q.Wrong);
                return;
            }
            Coins_Gained += 20 + 10*i + util::rng()/100;
            std::print("{}\nYour prize pool is now {} pwacoins! Do you want to continue for better prizes, or end the game now [Y/N]?\nCONTINUE?> ", Q.Correct, Coins_Gained);
            std::string response;
            std::getline(std::cin, response);
            while (response != "Y" && response != "N") {std::print("Please enter 'Y' or 'N'!\nCONTINUE?> "); std::getline(std::cin, response);}
            if (response == "N") {
                std::print("Ending the adventure!\nAdding your prizes to your coins!\n");
                player.coinup(Coins_Gained);
                std::print("You now have {} coins!\n See you next time!", player.getBalance());
                return;
            }
            std::print("Proceeding to the next question!\n");
        }
        Coins_Gained += Entry*3; 
        player.coinup(Coins_Gained);
        std::print("You really reached the end! Yay!!!\nYou got the grand jackpot of {}! Your total prize is {} pwacoins!\nBye!", Coins_Gained, player.getBalance());
    }
};
Gameplay games;

class Onboarding {
public:
    void welcome(herd& pwaherd) {
        std::print(R"(
PWA WELCOME TO PWALAND!
----------------------------------
Newcommer pwa-eh? Pwa! You must be soooo pwa confused
Someone ought to show you how things are done here
You start off having one ALPACA. But soon will be many!
How would you name your first alpaca? (no spaces!)
> )");
        std::string name;
        std::cin >> name;
        (pwaherd.addpwa(name)).setid(1);
        util::clearo();
        std::print(R"(

All done!
Now...
These alpacas needed to be fed, played with and always say pwa!
You will do this with commands!
Enter "HLP" for help!
)");
    }

    void welcome_back(herd& pwaherd) {
        std::print ("*Sleeping pwa mumbles* \nPWA! You came back? YAY PWA! You want to continue last save though? [Y/N]\n> ");
        std::string response;
        std::cin >> response;
        while (response != "N" && response != "Y") {
            std::print("Please enter Y or N\n> ");
            std::cin >> response;
        }
        if (response == "Y") {
            std::print("Pwa! The fun starts again!\n\nFor recap, you have {} alpacas!\nYou remember them?\nHere is all of them!\n\n", pwaherd.getsize());
            pwaherd.intro();
            std::print("\nLet's go! We start pwa! Enter HLP for help!\n");
            util::clearo();
            return;
        }
        util::clearo();
        std::print("Wait... you-- want to delete our world?\nBut... we had so much fun!\n\n");
        pwaherd.intro();
        std::print(R"(
Look... those are the alpacas

Remember them? 

You... had have so much fun... 

...

Why...

Why would you want...

To do such cruel things

But... I suppose I can't stop you...

Do... do you still want to say goodbye?

    Proceed
    Do Not
> )");
        std::getline(std::cin, response);
        while (response != "Proceed" && response != "Do Not") {
            std::print("\tProceed\n\tDo Not\n> ");
            std::getline(std::cin, response);
        }
        if (response == "Proceed") {
            std::print("Suddenly, the world starts fading, and fading, ...\n");
            util::delay(500);
            std::filesystem::remove("save1.txt");
            pwaherd.clear();
            player.clear();
            std::print("Until... Gone was the world you built...\n");
            util::delay(5000);
            std::print("\n\nAnd indeed... We guess...\n");
            util::delay(1000);
            std::print("Nothing, happened, nothing ever happened\n");
            util::delay(1000);
            std::print("A world GONE\n");
            util::delay(600);
            std::print("Just, like that...\n");
            util::delay(2000);
            std::print("No... We won't blame you...\n");
            util::delay(600);
            std::print("Just...\n");
            util::delay(1000);
            std::print("Next time...\n");
            util::delay(2000);
            std::print("Remember them, will you? The memories...\n");
            util::delay(3000);
            std::print("Let's begin again...\n\n");
            util::delay(1000);
            welcome(pwaherd);
        }
        else {std::print("...\nYou caught yourself in the middle of the thought to delete a save, nevermind, enter \"HLP for cmd list\"");}
    }
};
Onboarding welcomer;

class SaveManager {
public:
    std::expected<void, std::string> load(herd& pwaherd) {
        std::ifstream load("save1.txt");

        if (load.is_open()) {
            int SaveVInFile; int lastDaily;
            load >> SaveVInFile;
            if (SaveVInFile != SAVE_VERSION) return std::unexpected("Save file version mismatch");
            load >> lastDaily;
            meta.loglastdaily(lastDaily);
            int nAlpacas;
            long long coinmount;
            load >> coinmount >> nAlpacas;
            player.recoverBal(coinmount);
            pwaherd.recoverpwa(nAlpacas, load);
            meta.loadin(load);
            std::print("Pwa data recovered!\n");
        }
        else return std::unexpected("Pwa... System error... Failed to load save file");

        load.close();
        return {};
    }

    void save(herd& pwaherd) {
        std::ofstream save("save1.txt");
        if (save) {
            save << SAVE_VERSION << '\n';
            save << meta.getlastdaily() << '\n';
            save << player.getBalance() << '\n';
            save << pwaherd.getsize() << '\n';
            pwaherd.savepwa(save);
            meta.saveto(save);
            save.close();
            return;
        }
        else std::print("Oops, we cannot open the safe file, this is an internal error, we are sorry pwa...\n");
        save.close();
    }

    void savecheck(herd& pwaherd) {
        if (util::save_exist()) {
            auto load_result = load(pwaherd);
            if (load_result) {
                std::print("PWA LOAD SUCESSFUL!\n");
                Achievements.save_sync();
                welcomer.welcome_back(pwaherd);
            }
            else {std::print("Error: {}\nPerhaps this is an internal error, for now we can restart the game\n", load_result.error()); welcomer.welcome(pwaherd);}
        }
        else welcomer.welcome(pwaherd);
    }
};
SaveManager Save;

class Config {
    CommandSystem setup_commands(herd& pwaherd) {
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
    void run(CommandSystem& cmdsys, herd& Alpacaherd) {
        cmdsys = setup_commands(Alpacaherd);
        Achievements.setup();
        Save.savecheck(Alpacaherd);
    }
};
Config setup;

class PWALAND {
    herd pwaherd;
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
