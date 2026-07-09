#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <limits>
#include <functional>
#include <unordered_map>
#include <expected>
#include <random>
#include <format>
#include <ranges>
#include <cassert>
#include <charconv>

namespace sc {
    constexpr auto cmd = "Commands";
    constexpr auto pwa = "Alpaca";
    constexpr auto spc = "Special";
}

class dataclass {
    std::unordered_map <std::string, std::map<std::string, long long>> metadata;

public:
    long long& use(const std::string& cat, const std::string& ent) {
        return metadata[cat][ent];
    }
    long long get(const std::string& cat, const std::string& ent) const {
        auto it = metadata.find(cat);
        if (it == metadata.end()) return 0;
        auto it2 = it->second.find(ent);
        if (it2 == it->second.end()) return 0;
        return it2->second;
    }
    void exp(std::ofstream& out) {
        out << metadata.size() << '\n';
        for (auto& [name,cat] : metadata) {
            out << name << ' ' << cat.size() << '\n';
            for (auto& [name,amount] : cat) {
                out << name << ' ' << amount << '\n';
            }
        }
    }
    void imp(std::ifstream& in) {
        int nCats;
        std::string Cat;
        int nEnt;
        std::string Ent;
        long long EntVal;
        in >> nCats;
        for (auto _ : std::views::iota(0, nCats)) {
            in >> Cat >> nEnt;
            for (auto _ : std::views::iota(0, nEnt)) {
                in >> Ent >> EntVal;
                metadata[Cat][Ent] = EntVal;
            }
        }
    }
    void say() {
        std::cout << '\n' << "PWA! HERE IS YOUR METADATA!" << '\n'
        << '\n' << "1. Commands metadata" << '\n'
        << "You used ... times" << '\n';
        for (auto& [name, amount] : metadata[sc::cmd]) std::cout << name << ": " << amount << '\n';
        std::cout << '\n' << "2. Alpaca Statitics" << '\n';
        for (auto& [name, amount] : metadata[sc::pwa])
            std::cout << "Your alpacas together had achieved " << amount << ' ' << name << '!' << '\n';
        std::cout << '\n' << "3. Fun and special stuff" << '\n'
        << "You failed your commands " << metadata[sc::spc]["fail"] << " times... wow" << '\n';
    }
};
class metause {
    dataclass mtd;
public:
    void logpwa(int x) { mtd.use(sc::pwa, "pwa") += x; }
    void loglvl(int x) { mtd.use(sc::pwa, "lvl") += x; }
    void logfail() { mtd.use(sc::spc, "fail")++; }
    void logcmd(std::string cmd) { mtd.use(sc::cmd, cmd)++; }
    void saveto(std::ofstream& out) { mtd.exp(out); }
    void loadin(std::ifstream& in) { mtd.imp(in); }
    void listout() { mtd.say(); }
    long long getlvl() { return mtd.get(sc::pwa, "lvl"); }
    long long getpwa() { return mtd.get(sc::pwa, "pwa"); }
    long long getcmd(std::string cmd) { return mtd.get(sc::cmd, cmd); }
    long long getfail() { return mtd.get(sc::spc, "fail"); }
};
metause meta;

namespace util {
    void clearo() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    bool if_save_exist() {
        return std::filesystem::exists("save1.txt");
    }
    int rng() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> rand(1, 100);
        return rand(gen);
    }
    long long calc_fed_cost(int herdsz) {
        return meta.getlvl() + herdsz*5 - std::min(meta.getpwa()/50000, (long long) herdsz*3);
    }
    std::expected<int, std::string> str_to_num(std::string str) {
        int value;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
        if (ec == std::errc::invalid_argument)
            return std::unexpected("Pwa? Isn't this supposed to be... a numbber?\n");
        if (ec == std::errc::result_out_of_range || value >= 10000) 
            return std::unexpected("Pwa... big number...\n");
        if (value < 0)
            return std::unexpected("But pwa no learn negative numbers!\n");
        if (value == 0)
            return std::unexpected("Pwa why would you do something 0 times?\n");
        if (ptr != str.data() + str.size())
            return std::unexpected("Tricky owner sneak trailing characters!\n");
        return value;
    }
}

class Achievements {
    std::string name;
    std::string des;
    std::vector<std::function<bool()>> cond;
    bool completed = false;
    bool secret = false;

public:
    Achievements(
        std::string n,
        std::string d,
        std::vector<std::function<bool()>> c
    ) : name(std::move(n)),
        des(std::move(d)),
        cond(std::move(c)) {};

    int check() {
        if (completed) return 2;
        for (auto& c : cond) if (!(c())) return 0;
        completed = true;
        return 1;

        // 2 means this achievement was ALREADY COMPLETED
        // 1 means this achievement is JUST COMPLETED
        // 0 means this achievement is NOT COMPLETED
    }
    void set_secret() {secret = true;}
    void present() {
        if (secret == true) return;
        std::cout << '\n' << "Achievement: " << name << '\n'
        << "Description: " << des << '\n'
        << "You have " << (completed ? "completed " : "not completed ") << "this achievement!" << '\n';
    }
    void complete() {
        std::cout << "You achieved... " << name << '!' << '\n'
        << "Look at achievement info for more information!" << '\n';
    }
    void internal_markdone() {
        completed = true;
    }
};
class achievements_mana {
    std::unordered_map<std::string, Achievements> achi;
public:
    void add(
        std::string name,
        std::string des,
        std::vector<std::function<bool()>> cond
    )
    {
        achi.try_emplace(
            name,
            std::move(name),
            std::move(des),
            std::move(cond)
        );
    }
    void hide(const std::string& name) {
        auto hide = achi.find(name);
        assert(hide != achi.end());
        hide->second.set_secret();
    }
    class reqs {
        //tbd
    };
    void setup() {
        ///All achievements go here
        add( "First pwa", "Pwa for the first time", { [] {return meta.getpwa() >= 1;} });
        add( "More pwas", "Pwa more than 10 times", { [] {return meta.getpwa() >= 10;} });
        add( "Beginner pwa", "Pwa more than 100 times", { [] {return meta.getpwa() >= 100;} });
        add( "Intermediate pwa", "Pwa more than 1000 times", { [] {return meta.getpwa() >= 1000;} });
        add( "Great pwa!", "Pwa more than 10000 times", { [] {return meta.getpwa() >= 10000;} });
        add( "Master pwa!","Pwa more than 100000 times",{ [] {return meta.getpwa() >= 100000;} });
        add( "Brilliant pwa!", "Pwa more than 1 000 000 times", { [] {return meta.getpwa() >= 1e6;} });
        add( "Legendary pwa!", "Pwa more than 10 000 000 times", { [] {return meta.getpwa() >= 10000000;} });
        add( "Epic pwa!", "Pwa more than 100 000 000 times", { [] {return meta.getpwa() >= 100000000;} });
        add( "Mythic pwa!", "Pwa more than 200 000 000 times",{ [] {return meta.getpwa() >= 200000000;} });
        add( "Solar pwa!", "Pwa more than 300 000 000 times", { [] {return meta.getpwa() >= 300000000;} });
        add( "Interstellar pwa!", "Pwa more than 400 000 000 times", { [] {return meta.getpwa() >= 400000000;} });
        add( "Galactic pwa!", "Pwa more than 500 000 000 times", { [] {return meta.getpwa() >= 500000000;} });
        add( "Intergalactic pwa!", "Pwa more than 600 000 000 times", { [] {return meta.getpwa() >= 600000000;} });
        add( "Void pwa!", "Pwa more than 700 000 000 times", { [] {return meta.getpwa() >= 700000000;} });
        add( "Universal pwa!", "Pwa more than 800 000 000 times", { [] {return meta.getpwa() >= 800000000;} });
        add( "Multiversal pwa!", "Pwa more than 900 000 000 times", { [] {return meta.getpwa() >= 900000000;} });
        add( "GOD OF PWAS", "Pwa more than 1 000 000 000 times. PWA TRANSCENDENCE", { [] {return meta.getpwa() >= 1000000000;} });
        add( "Clueless beginner", "Use the HLP command", { [] {return meta.getcmd("HLP") >= 1;} });
        add( "Forgetful man", "Use the HLP command more than 10 times", { [] {return meta.getcmd("HLP") >= 10;} });
        add( "Confused dude", "Use the FAQ command", { [] {return meta.getcmd("FAQ") >= 1;} });
        add( "So many weird things!", "Use the FAQ command more than 10 times", { [] {return meta.getcmd("FAQ") >= 10;} });
        add( "Curious person", "Use the JGL command", { [] {return meta.getcmd("JGL") >= 1;} });
        add( "Update inspector", "Use the JGL command more than 10 times", { [] {return meta.getcmd("JGL") >= 10;} });
        add( "Ooh? New dashboard?", "Use the MTD command", { [] {return meta.getcmd("MTD") >= 1;} });
        add( "Obsessive checker", "Use the MTD command more than 10 times", { [] {return meta.getcmd("MTD") >= 10;} });
        add( "Awww too little money", "Use the BAL command", { [] {return meta.getcmd("BAL") >= 1;} });
        add( "Financially responsible", "Use the BAL command more than 10 times", { [] {return meta.getcmd("BAL") >= 10;} });
        add( "Nhom nhom nhom", "Use the FED command", { [] {return meta.getcmd("FED") >= 1;} });
        add( "Pwaful and well fed", "Use the FED command more than 10 times", { [] {return meta.getcmd("FED") >= 10;} });
        add( "Pwa pwa pwa pwa pwa", "Use the PWA command", { [] {return meta.getcmd("PWA") >= 1;} });
        add( "Pwa machine gun!", "Use the PWA command more than 10 times", { [] {return meta.getcmd("PWA") >= 10;} });
        add( "Catch!", "Use the PLY command", { [] {return meta.getcmd("PLY") >= 1;} });
        add( "Vet but alpaca and friendly!", "Use the PLY command more than 10 times", { [] {return meta.getcmd("PLY") >= 10;} });
        add( "How's my alpaca?", "Use the INF command", { [] {return meta.getcmd("INF") >= 1;} });
        add( "Know too much", "Use the INF command more than 10 times", { [] {return meta.getcmd("INF") >= 10;} });
        add( "New pwa into town!", "Use the ADD command", { [] {return meta.getcmd("ADD") >= 1;} });
        add( "A literal barn!", "Use the ADD command more than 10 times", { [] {return meta.getcmd("ADD") >= 10;} });
        add( "Line... up!", "Use the LNP command", { [] {return meta.getcmd("LNP") >= 1;} });
        add( "Into formation!", "Use the LNP command more than 10 times", { [] {return meta.getcmd("LNP") >= 10;} });
        add( "Pwa-bye!", "Use the END command", { [] {return meta.getcmd("END") >= 1;} });
        add( "Quitter!", "Use the END command more than 10 times",{ [] {return meta.getcmd("END") >= 10;} });
        add( "First command ever!", "Enter your first command", { [] {return meta.getcmd("ALL") >= 1;} });
        add( "You got the hang of it!", "Do 10 commands", { [] {return meta.getcmd("ALL") >= 10;} });
        add( "Master commander", "Do 100 commands", { [] {return meta.getcmd("ALL") >= 100;} });
        add( "Definitely a spammer", "Do 1000 command", { [] {return meta.getcmd("ALL") >= 1000;} });
        add( "A hidden conversation", "Discover DEV command", { [] {return meta.getcmd("DEV") >= 1;} });
        hide("A hidden conversation");
        add( "Oops... try again!", "Fail a command for the first time", { [] {return meta.getfail() >= 1;} });
        add( "Trial and error", "Fail a command ten times", { [] {return meta.getfail() >= 10;} });
        add( "Uncareful typo", "Fail a command 100 times", { [] {return meta.getfail() >= 100;} });
        add( "The epic fail", "Fail a command 1000 times", { [] {return meta.getfail() >= 1000;} });
        add( "LEVEL_UP", "Level up alpacas more than once", { [] {return meta.getlvl() >= 1;} });
        add( "Super booster!", "Level up alpacas more than 10 times", { [] {return meta.getlvl() >= 10;} });
        add( "Muscular pwarrr", "Level up alpacas more than 100 times", { [] {return meta.getlvl() >= 100;} });
        add( "PWA of a fighter!", "Level up alpacas more than 1000 times", { [] {return meta.getlvl() >= 1000;} });
        add( "Tanky pwa, or pwa is a tank?", "Level up alpacas more than 10000 times", { [] {return meta.getlvl() >= 10000;} });
    }
    std::expected<void, std::string> show (std::string& name) {
        std::cout << "Searching for: [" << name << "]\n";
        auto it = achi.find(name);
        if (it == achi.end()) return std::unexpected("No such achievements\n");
        it->second.present();
        return {};
    }
    void list_out() {
        for (auto& [_, ach] : achi) {
            ach.present();
        }
    }
    void check() {
        for (auto& [_, ach] : achi) {
            int res = ach.check();
            if (res == 1) ach.complete();
        }
    }
    void save_sync() {
        for (auto& [_, ach] : achi) {
            int res = ach.check();
            if (res == 1) ach.internal_markdone();
        }
    }
};

achievements_mana achieve_list;

class playerinfo {
    long long money = 0;

public:
    static playerinfo& instance() {
        static playerinfo p;
        return p;
    }
    void coinup(long long amount) {
        money += amount;
    }

    std::expected<void,std::string> coindown (long long amount) {
        if (money >= amount) {
            money -= amount;
            return {};
        }
        else return std::unexpected(std::format("Not enough money pwa... You need {} more\n", amount-money));
    }

    long long getBalance() {
        return money;
    }

    void recoverBal(long long amount) {
        money = amount;
    }
};

class Alpaca {
    int pwaid = 0;
    int pwatimes = 0;
    int level = 0;
    long long xp = 0;

    public:
    std::string name;
    Alpaca(const std::string& n) : name(n) {}

    bool operator<(const Alpaca& other) const {
        return name < other.name;
    }
    void determine_levelup() {
        long long xp_required = (level * (long long) std::log2(level + 5)) * 2;
        std::cout << std::format("Pwa {} is at level {}, [{}/{}]!\n", name, level, xp, xp_required);
        while (xp >= xp_required) {
            level++;
            meta.loglvl(1);
            xp -= xp_required;
            xp_required = (level * (long long) std::log2(level + 5)) * 2;
            std::cout << std::format("YEAH! Pwa {} had leveled up to level {}, [{}/{}]!\n", name, level, xp, xp_required);
            pwatimes++;
        }
    }

    void setid(int newid) {
        std::cout << "PWA! I am setting my pwaid to "
        << newid << '\n';
        pwaid = newid;
        pwatimes += 2;
    }

    void pwa(int times) {
        std::cout << name << " pwaing! ";
        pwatimes += times;
        meta.logpwa(times);
        for (int i = 0; i < times; i++) {
            std::cout << "pwa ";
        }
        std::cout << '\n';
    }

    std::expected<void, std::string> feed(int herdsz) {
        long long cost;
        cost = util::calc_fed_cost(herdsz);
        auto res = playerinfo::instance().coindown(cost);
        if (!res) return std::unexpected (res.error());
        std::cout << "pwa " << name
        << " thank you nhom nhom nhom "
        << "yum pwa" << '\n';
        pwatimes += 2;
        meta.logpwa(2);
        xp += 5;
        determine_levelup();
        return {};
    }

    void play() {
        std::cout << "Playing with " << name << " PWA PWA!" << '\n';
        struct chance {
            int perc;
            int pwam;
            int expm;
        };
        int destiny = util::rng();
        std::vector<chance> shot { {15, 20, 20}, {20, 15, 20}, {25, 20, 15}, {30, 10, 10}, {9, 25, 25}, {1, 100, 100} };
        int sum = 0;
        for (auto& nows_your_chance : shot) {
            sum += nows_your_chance.perc;
            if (sum > destiny) {
                std::cout << name << " is very happy pwa! You got " << nows_your_chance.pwam << " pwas and "
                << nows_your_chance.expm << " xp!" << '\n';
                pwatimes += nows_your_chance.pwam;
                xp += nows_your_chance.expm;
                meta.logpwa(nows_your_chance.pwam);
                determine_levelup();
                return;
            }
        }
    }

    void intro() {
        struct ticket {
            int chance;
            int pwamount;
            std::string contents;
        };

        int destiny = util::rng();

        std::vector<ticket> BIG_SHOT = {
            {
                24, 6,
                std::format("PWA! Pwa's name is {}, pwa's id is {}, pwa is level {} + {} xp, and pwa had pwa-ed {} times"
                            , name, pwaid, level, xp, pwatimes)
            }, {
                24, 9,
                std::format("pwa pwa pwa... pwa is {}, with id {} and pwa pwa level {} + {} xp, pwa pwa pwa-ed {} times"
                            , name, pwaid, level, xp, pwatimes)
            }, {
                24, 9,
                std::format("PWA! PWA! PWA! NAME {}! ID {}! LEVEL {}! XP {}! PWATIMES {}! PWA! PWA! PWA! PWA! PWA!"
                            , name, pwaid, level, xp, pwatimes)
            }, {
                24, 6,
                std::format("PWA PWa Pwa pwa... name is {}, ID is {}, level is {} + {} xp, and pwa pwa-ed {} times"
                            , name, pwaid, level, xp, pwatimes)
            }, {
                1, 20,
                std::format("PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA PWA {}"
                            , name)
            }, {
                1, 10,
                std::format("Pwa... {} here! Pwa??? Pwa pwa pwa pwa no tell >:(, pwa ha ha ha ha pwa pwa pwa *eat grass*"
                            , name)
            }, {
                1, 8,
                std::format("And it's AL - PA - CA TIME for pwa to PWA PWA PWA pwa pwa. Pwa name is {} pwa"
                            , name)
            }, {
                1, 10,
                std::format("PWA HA HA HA HA! PWA WILL PWA EVERYTHING INTO PWAS AND PWAS PWA PWA PWA. Know PWA! Pwa is {}"
                            , name)
            }
        };

        int sum = 0;
        for (auto& SHOT : BIG_SHOT) {
            sum += SHOT.chance;
            if (sum > destiny) {
                pwatimes += SHOT.pwamount;
                meta.logpwa(SHOT.pwamount);
                std::cout << SHOT.contents << '\n';
                return;
            }
        }
    }
    void restorepwa (
                    int lid,
                    int lpwatimes, int llevel,
                    long long lxp
                    )
    {
        pwaid = lid;
        pwatimes = lpwatimes;
        level = llevel;
        xp = lxp;
    }

    void savepwa (std::ofstream& out) {
        out << name << ' ' << pwaid << ' ' << pwatimes << ' '
        << level << ' ' << xp << '\n';
    }
};

class command_system {
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
            if (iQuoted) {
                if (c == '"') {
                    iQuoted = false; 
                    a.push_back(cur); 
                    cur.clear();
                    if (a.size() > 10) return std::unexpected("Hey! PWA DIZZY! TOO MANY ARGUMENTS\n");
                    continue;
                }
                cur.push_back(c);
                continue;
            }
            if (c == '"') {iQuoted = true; continue;}
            if (c == ' ' && !cur.empty()) {
                a.push_back(cur);
                cur.clear();
                if (a.size() > 10) return std::unexpected("Hey! PWA DIZZY! TOO MANY ARGUMENTS\n");
            }
            else if (c != ' ') cur.push_back(c);
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
        if (!check) return std::unexpected("No such command exist!\n");
        auto cmdrun = *check;
        auto res = cmdrun(args);
        if (!res) return std::unexpected(res.error());
        meta.logcmd("ALL");
        meta.logcmd(args[0]);
        return {};
    }
};

class herd {
    std::map<std::string, Alpaca> pwaherd;
public:
    Alpaca* findpwa (const std::string name) {
        auto it = pwaherd.find(name);

        if (it == pwaherd.end()) return nullptr;
        return &it->second;
    }

    void addpwa (const std::string name) {
        pwaherd.try_emplace(name, (name));
    }

    int getsize() {
        return pwaherd.size();
    }

    void intro() {
        for (auto& [_, pwa] : pwaherd) pwa.intro();
    }

    void recoverpwa(int nAlpacas, std::ifstream& in) {
        for (int i = 0; i < nAlpacas; i++) {
            std::string lname;
            int lid;
            int lpwatimes;
            int llevel;
            long long lxp;
            in >> lname >> lid >> lpwatimes
            >> llevel >> lxp;
            addpwa(lname);
            auto pwatarg = findpwa(lname);
            (*pwatarg).restorepwa(
                lid,
                lpwatimes, llevel,
                lxp
            );
        }
    }

    void savepwa(std::ofstream& out) {
        for (auto& [name, alpaca] : pwaherd) {
            alpaca.savepwa(out);
        }
    }

    void clear() {
        pwaherd.clear();
    }
};

command_system cmdres(herd& pwaherd) {
    command_system cmdsys;
    cmdsys.add("HLP", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 argumments, got {}\n", args.size()-1));
        std::cout << '\n' << "COMMANDS GUIDE" << '\n'
        << "----------------------------------"
        << '\n' << "1. General commands" << '\n'
        << "HLP - The help command, show commands details" << '\n'
        << "FAQ - Common questions and errors" << '\n'
        << "JGL - Show the changelog for the recent updates." << '\n'
        << "MTD - Shows some data about your gameplay." << '\n'
        << "END - Stop the program and exit, progress is indeed saved" << '\n'
        << '\n' << "2. Player information" << '\n'
        << "BAL - Show your pwacoin balance!" << '\n'
        << "ACH - Show all achievements" << '\n'
        << "AIF \"<name>\" - View information on a specific achievement" << '\n'
        << '\n' << "3. Alpaca interactions" << '\n'
        << "INF <name> - Show information on your alpaca" << '\n'
        << "FED <name> <amount> - Feed the alpacas, may they pwa more and be well fed" << '\n'
        << "PWA <name> <amount> - Let them PWA!" << '\n'
        << "PLY <name> - Play with your alpaca!" << '\n'
        << "ADD <name> - Grow your herd! Add another alpaca!" << '\n'
        << "LNP - Show your alpaca formation!" << '\n'
        << '\n' << "More features will be added soon, this will be updated" << '\n';

        return {};
    });

    cmdsys.add("MTD", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        meta.listout();
        return {};
    });


    cmdsys.add("FAQ", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        std::cout << '\n' << "FAQ" << '\n'
        << "Q: Why is my command not fully interpreted, what does invalid amount type mean?" << '\n'
        << "A: This means either that your command is missing or giving more than the required "
        << "arguments, please notice that amount has to be a numeric value above 0 and that names "
        << "are without spaces. The error means that your command structure is not as the program "
        << "intended." << '\n' << '\n'
        << "Q: Oh no my save / load is not working, is there any way to recover this?" << '\n'
        << "A: Right now, unfortunately not. This could be that the file is deleted for it is a "
        << "program error =(. If this were load, try running the file again. If this is save, then "
        << "there is sadly no way to recover your progress, you could restart the program to recover "
        << "from your last save, but your most recent progress is lost. One way to avoid this is to "
        << "save regularly and do not make changes to the project folder. And most importantly, use "
        << "the END command to close instead of closing the terminal itself, progress WILL NOT be "
        << "updated if you close the terminal" << '\n' << '\n'
        << "Q: My FED command is miscounted! Suppose i do FED _ 5 where i only have enough for 3, it "
        << "will not count that command entirely!" << '\n'
        << "A: We are aware of this problem, the commands in MTD only count sucessful commands whereas "
        << "a partially sucessful command will not count, while we fix this, if you want to farm the "
        << "stats, be wary and calculate how much money you can actually afford." <<  '\n' << '\n'
        << "Q: I entered the right arguments, why does it says no such alpaca / command exist?" << '\n'
        << "A: There is a good chance you are mixing lowercase and uppercase. You have to enter your "
        << "alpaca name exactly the way you enter them at the ADD command or in the beginning, case "
        << "sensitive. As for all commands you need to enter all letters uppercase. =)" << '\n' << '\n'
        << "Q: Why is AIF so annoying?" << '\n'
        << "A: Yes, I am entirely aware of this one, and is working towards fixing it in the future. "
        << "For now, for the best, perhaps use ACH or copy the exact name from the achievement "
        << "complete message (without the '!'). Hope it helps!" << '\n';

        return {};
    });

    cmdsys.add("JGL", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        struct v_log {
            int large_v;
            int small_v;
            std::string date;
            std::string contents;
        };
        static std::vector<v_log> changelogs = {
            {
                5, 0, "Jn21, 2026",
                "FIVE. DOT. OH. Added achievements, you can use the ACH command to view all of them. Also: use AIF "
                "to view each specific achievement (though that is fragile). Anyways final fulfillment to: the grand"
                "... implementation 5!"
            }
        };

        for (auto& log : changelogs) {
            std::cout << std::format("\n{}: Version {}.{}\n{}", log.date, log.large_v, log.small_v, log.contents);
        }

        return {};
    });

    cmdsys.add("FED", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 3) return std::unexpected(std::format("Expected 2 arguments, got {}\n", args.size()-1));
        std::string& targetname = args[1];
        Alpaca* pwatarg = pwaherd.findpwa(targetname);
        if (!pwatarg) return std::unexpected("No such alpaca pwa\n");

        auto numres = util::str_to_num(args[2]);
        if (!numres) return std::unexpected(numres.error());
        int amount = numres.value();

        for (int i = 0; i < amount; i++) {
            auto working = pwatarg->feed(pwaherd.getsize());
            if (!working) return std::unexpected(working.error());
        }
        return {};
    });

    cmdsys.add("PWA", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 3) return std::unexpected(std::format("Expected 2 arguments, got {}\n", args.size()-1));
        std::string targetname = args[1];
        Alpaca* pwatarg = pwaherd.findpwa(targetname);
        if (!pwatarg) return std::unexpected("No such alpaca pwa!\n");

        auto numres = util::str_to_num(args[2]);
        if (!numres) return std::unexpected(numres.error());
        int times = numres.value();
        pwatarg->pwa(times);
        return {};
    });

    cmdsys.add("PLY", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 2) return std::unexpected(std::format("Expected 1 arguments, got {}\n", args.size()-1));
        std::string targetname = args[1];
        Alpaca* pwatarg = pwaherd.findpwa(targetname);
        if (!pwatarg) return std::unexpected("No such alpaca pwa!\n");
        auto res = playerinfo::instance().coindown(10);
        if (!res) return std::unexpected(res.error());
        pwatarg->play();
        return {};
    });

    cmdsys.add("INF", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 2) return std::unexpected(std::format("Expected 1 arguments, got {}\n", args.size()-1));
        std::string pwaname = args[1];
        auto it = pwaherd.findpwa(pwaname);
        if (!it) return std::unexpected("No such alpaca!\n");
        it->intro();
        return {};
    });

    cmdsys.add("ADD", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 2) return std::unexpected(std::format("Expected 1 arguments, got {}\n", args.size()-1));
        std::string pwaname = args[1];
        auto it = pwaherd.findpwa(pwaname);
        if (it) return std::unexpected("That alpaca already exist!\n");

        long long cost = 25 + 15 * (pwaherd.getsize()-1);

        auto res = playerinfo::instance().coindown(cost);

        if (!res) return std::unexpected(res.error());

        std::cout << "Adding alpaca " << pwaname << " into your herd!" << '\n';
        pwaherd.addpwa(pwaname);
        auto it2 = pwaherd.findpwa(pwaname);
        it2->setid(pwaherd.getsize());
        std::cout << "The herd grows..." << '\n';
        std::cout << "You now have " << pwaherd.getsize() << " alpacas!" << '\n';
        return {};

    });

    cmdsys.add("BAL", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        long long balance = playerinfo::instance().getBalance();
        std::cout << "Your balance is " << balance << " pwacoins!" << '\n';
        return {};
    });

    cmdsys.add("LNP", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        std::cout << "PWA HERE IS YOUR LINEUP!" << '\n'
        << "You have " << pwaherd.getsize() << " alpacas!" << '\n'
        << "And may they introduce themselves to ya!" << '\n' << '\n';

        pwaherd.intro();

        return {};
    });

    cmdsys.add("ACH", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        std::cout << "PWA ACHIEVEMENTS!" << '\n';
        achieve_list.list_out();
        return {};
    });

    cmdsys.add("AIF", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        //this one is a pain in hell
        if (args.size() != 2) return std::unexpected(std::format("Expected 1 arguments, got {}\n", args.size()-1));
        std::string target = args[1];
        auto res = achieve_list.show(target);
        if (!res) return std::unexpected(res.error());
        return {};
    });

    cmdsys.add("DEV", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        /// THIS COMMAND IS HIDDEN AND DELIBERATELY UNDOCUMENTED
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        std::cout << "Huh? you are... here? Is that that you found it by accident, "
        << "or someone told you? [Y/N]" << '\n' << "> ";
        std::string response;
        std::cin >> response;
        if (response == "Y") std::cout << "Oh, lucky you" << '\n';
        else if (response == "N") std::cout << "I see..." << '\n';
        else std::cout << "Weird but ok" << '\n';
        std::cout << "Whether that is... Does it matter?" << '\n'
        << "All that's to be that you are here..." << '\n'
        << "Make this project for fun and all, im proud still" <<  '\n'
        << "But hey! If you appreciate it, continue!" << '\n'
        << "Bye! Again soon!" << '\n';

        return {};
    });

    cmdsys.add("END", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        std::cout << "Ending pwa =((((" << '\n';
        return std::unexpected("Ending");
    });

    return cmdsys;
}

class PWALAND {
    herd pwaherd;
    command_system cmdsys;

    void load_data() {
        std::ifstream load("save1.txt");

        if (load.is_open()) {
            int nAlpacas;
            long long coinmount;
            load >> coinmount >> nAlpacas;
            playerinfo::instance().recoverBal(coinmount);
            pwaherd.recoverpwa(nAlpacas, load);
            meta.loadin(load);
            std::cout << "Loaded data successfully" << '\n';
        }
        else std::cout << "Failure to load!";

        load.close();
    }

    void welcome() {
        std::cout << "PWA WELCOME TO PWALAND! " << '\n'
        << "----------------------------------" << '\n'
        << "Newcommer pwa-eh? Pwa! You must be soooo pwa confused" << '\n'
        << "Someone ought to show you how things are done here" << '\n'
        << "You have ALPACA, first is one, but more the better!" << '\n'
        << "How would you name your first alpaca?" << '\n';
        std::cout << "> ";
        std::string name;
        std::cin >> name;
        pwaherd.addpwa(name);
        auto pwaplace = pwaherd.findpwa(name);
        pwaplace->setid(1);
        util::clearo();
        std::cout << '\n';
        std::cout << '\n' << "All done!" << '\n'
        << "You will use commands to take care of him!"
        << '\n' << "It's important you know which"
        << '\n' << "Enter HLP for help!" << '\n'
        << '\n' << "> ";
    }

    void welcome_back() {
        std::cout << "PWA YOU ACTUALLY CAME BACK!" << '\n'
        << "Pwa pwa pwa we are so happy" << '\n'
        << "Before we come back, do you want to continue last save, or start fresh? [Y/N]" << '\n'
        << "> ";
        std::string response;
        std::cin >> response;
        while (response != "N" && response != "Y") {
            std::cout << "Please enter Y or N" << '\n' << "> ";
            std::cin >> response;
        }
        if (response == "Y") {
            std::cout << "Great! We may proceed" << '\n'
            << "For recap, this is all your info" << '\n'
            << "You have " << pwaherd.getsize() << " alpacas!" << '\n'
            << "Let's see your lineup again!" << '\n' << '\n';
            pwaherd.intro();
            std::cout << '\n' << "We ready for more fun again!" << '\n' << "> ";
            return;
        }
        else {
            std::cout << "Pwa... You will lose all your alpacas =(" << '\n'
            << "Are you sure?" << '\n' << "Look at your alpacas =(" << '\n';
            pwaherd.intro();
            std::cout << "Do you really want to say bye?" << '\n' << "> ";
            std::cin >> response;
            while (response != "N" && response != "Y") {
                std::cout << "Please enter Y or N" << '\n' << "> ";
                std::cin >> response;
            }
            if (response == "Y") {
                std::cout << "I guess you want it, bye *sobs*" << '\n';
                std::filesystem::remove("save1.txt");
                pwaherd.clear();
                auto i = playerinfo::instance().coindown(playerinfo::instance().getBalance());
                assert(i);
                std::cout << "See... ya... again..." << '\n' << '\n';
                welcome();
            }
            else {
                std::cout << "Pwa! Knew you would change your mind" << '\n'
                << "For recap, this is all your info" << '\n'
                << "You have " << pwaherd.getsize() << " alpacas!" << '\n'
                << "Let's see your lineup again!" << '\n';
                pwaherd.intro();
                std::cout << '\n' << "PWA! Back on track now!" << '\n' << "> ";
            }
        }
    }

    void day_ends() {
        long long pwacoins = pwaherd.getsize() * 3;
        struct Ticket {
            int weight;
            std::string dat;
        };
        std::vector<Ticket> tickets = {
            {   33,
                std::format("The day ends... your alpacas had given you {} pwacoins!", pwacoins)
            }, {
                33,
                std::format("Dusk and dawn, may the next day be peaceful, you have been awarded {} pwacoins!", pwacoins)
            }, {
                33,
                std::format("Sky had faded, day had ended. You are awarded with {} pwacoins!", pwacoins)
            }, {
                1,
                std::format("Secret lies upon, will you dare? You are awarded {} pwacoins...", pwacoins)
            }
        };
        int holy_judgement = util::rng();

        int sum = 0;
        for (auto& i : tickets) {
            sum += i.weight;
            if (sum >= holy_judgement) {
                std::cout << i.dat << '\n';
                playerinfo::instance().coinup(pwacoins);
                break;
            }
        }
    }

    void save_file() {
        std::ofstream save("save1.txt");
        if (save) {
            save << playerinfo::instance().getBalance() << '\n';
            save << pwaherd.getsize() << '\n';
            pwaherd.savepwa(save);
            meta.saveto(save);
            save.close();
            return;
        }
        else std::cout << "oops... no save file" << '\n';
        save.close();
    }

public:
    void mainpart() {
        cmdsys = cmdres(this->pwaherd);
        achieve_list.setup();
        if (util::if_save_exist()) {
            load_data();
            achieve_list.save_sync();
            welcome_back();
        }
        else {
            welcome();
        }

        std::string cmdline;
        int time_of_day = 0;

        while(std::getline(std::cin, cmdline)) {
            auto res = cmdsys.run(cmdline);
            if (!res) {
                std::string error_msg = res.error();
                if (error_msg == "Ending") break;
                meta.logfail();
                std::cout << error_msg;
            }
            else {
                time_of_day++;
                time_of_day %= 5;
                if (time_of_day == 0) day_ends();
            }
            achieve_list.check();
            std::cout << '\n' << "> ";
        }

        std::cout << "PWA here is all your alpaca info" << '\n';
        pwaherd.intro();
        std::cout << "Saving your herd data!" << '\n';
        save_file();
        std::cout << '\n' << "Pwa-bye! See ya again!" << '\n';
    }
};

int main() {
    PWALAND game;
    game.mainpart();
}
