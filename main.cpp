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
#include <print>

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
        std::print("\nPWA HERE IS YOUR METADATA\n");
        std::print("\n1. Commands usage\n");
        for (auto& [name, amount] : metadata[sc::cmd]) std::print("{}: {}\n", name, amount);
        std::print("\n2. Alpaca stats\n");
        for (auto& [name, amount] : metadata[sc::pwa]) std::print("You and your pwas together had achieved {} {}\n", amount, name);
        std::print("\n3. Exotic stats\n");
        std::print("Wondered how you failed your commands {} times? Pwa!\n", metadata[sc::spc]["fail"]);
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
        if (ec == std::errc::invalid_argument) return std::unexpected("Pwa? Isn't this supposed to be... a numbber?\n");
        if (ec == std::errc::result_out_of_range || value >= 10000) return std::unexpected("Pwa... big number...\n");
        if (value < 0) return std::unexpected("But pwa no learn negative numbers!\n");
        if (value == 0) return std::unexpected("Pwa why would you do something 0 times?\n");
        if (ptr != str.data() + str.size()) return std::unexpected("Tricky owner sneak trailing characters!\n");
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
    }
    void set_secret() {secret = true;}
    void present() {
        if (secret == true) return;
        std::print("\nAchievement: {}\nDescription {}\nYou have {} this achievement!\n", name, des, (completed ? "completed " : "not completed "));
    }
    void complete() {std::print("\nYou have completed {}! Check AIF \"{}\" for more info!\n", name, name);}
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
    class requirements {
    public:
        std::vector<std::function<bool()>> pwa(int amount) {return {[amount] {return meta.getpwa() >= amount;}};}
        std::vector<std::function<bool()>> fal(int amount) {return {[amount] {return meta.getfail() >= amount;}};}
        std::vector<std::function<bool()>> lvl(int amount) {return {[amount] {return meta.getlvl() >= amount;}};}
        std::vector<std::function<bool()>> cmd(const std::string& name, int amount) {
            return {[name, amount] {return meta.getcmd(name) >= amount;}};
        }
        // if cmd not available here or is exotic, use lambda
    };
    requirements req;
    void setup() {
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
        add( "Curious person", "Use the JGL command", req.cmd("JGL", 1));
        add( "Update inspector", "Use the JGL command more than 10 times", req.cmd("JGL", 10));
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
    std::expected<void, std::string> show (std::string& name) {
        std::print("Searching for [{}]!\n", name);
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
        for (auto& [_, ach] : achi) int _ = ach.check();
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

    void setid(int newid) {
        std::print("Pwa setting my pwaid to {}!\n", newid);
        pwaid = newid;
        pwatimes += 2;
    }

    void pwa(int times) {
        std::print("{} pwaing!\n", name);
        pwatimes += times;
        meta.logpwa(times);
        for (int i = 0; i < times; i++) std::print("pwa ");
        std::print("\n");
    }

    std::expected<void, std::string> feed(int herdsz) {
        long long cost;
        cost = util::calc_fed_cost(herdsz);
        auto res = playerinfo::instance().coindown(cost);
        if (!res) return std::unexpected (res.error());
        std::print("PWA {} THANK YOU! PWA! NHOM NHOM NOHM NOM\n", name);
        pwatimes += 2;
        meta.logpwa(2);
        xp += 5;
        determine_levelup();
        return {};
    }

    void play() {
        std::print("Playing with {} PWA PWA!\n", name);
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
                int pwaadded = nows_your_chance.pwam;
                int xpadded = nows_your_chance.expm;
                std::print("Pwa is very happy! You got {} xp and {} pwas!\n", xpadded, pwaadded);
                pwatimes += pwaadded;
                xp += xpadded;
                meta.logpwa(pwaadded);
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
                std::print("{}\n",SHOT.contents);
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
        std::print(out, "{} {} {} {} {}\n", name, pwaid, pwatimes, level, xp);
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
        std::print(R"(

COMMANDS GUIDE
----------------------------------

1. General commands
HLP - The help command, show commands details
FAQ - Common questions and errors
JGL - Show the changelog for the recent updates.
MTD - Shows some data about your gameplay.
END - Stop the program and exit, progress is indeed saved

2. Player information
BAL - Show your pwacoin balance!
ACH - Show all achievements
AIF "<name>" - View information on a specific achievement

3. Alpaca interactions
INF <name> - Show information on your alpaca
FED <name> <amount> - Feed the alpacas, may they pwa more and be well fed
PWA <name> <amount> - Let them PWA!
PLY <name> - Play with your alpaca!
ADD <name> - Grow your herd! Add another alpaca!
LNP - Show your alpaca formation!

More coming soon! =)
)");
        return {};
    });

    cmdsys.add("MTD", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        meta.listout();
        return {};
    });


    cmdsys.add("FAQ", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        std::print (R"( 

Frequently Asked Questions
==================================

Q: Oh no my save / load is not working, is there any way to recover this?
A: Right now, unfortunately not. 
This could be that the file is deleted for it is a program error =(. 
    If this were load, try running the file again. 
    If this is save, then there is sadly no way to recover your progress.
One way to avoid this is to save regularly and do not make changes to the project folder. 
Always using the END command to end the game and save it before closing the terminal

Q: I entered the right arguments, why does it says no such alpaca / command exist?
A: There is a good chance you are mixing lowercase and uppercase.
    If you are entering a command, all letters uppercase
    If you are entering an alpaca, case sensitive and be exact
    This is another reason to enter a memorable and easy to type alpaca name

Q: Is there any help entering achievement names? It's hard to enter!
A: No, at least not yet, for now:
    Always surround the names with quotes ("), this avoid parsing problems
    Beware of case sensitivity
    The program will show "Searching for: [Your_Input]" to help
More recently, parser was updated to ignore leading and trailing spaces to help safer parsing

)");
        return {};
    });

    cmdsys.add("JGL", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        // DEPRECIATED
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
            std::print("\n{}: Version {}.{}\n{}\n", log.date, log.large_v, log.small_v, log.contents);
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

        std::print("Adding alpaca {} into your herd!\n", pwaname);
        pwaherd.addpwa(pwaname);
        auto it2 = pwaherd.findpwa(pwaname);
        it2->setid(pwaherd.getsize());
        std::print("THe herd continues to grow\n You now have {} alpacas!\n", pwaherd.getsize());
        return {};

    });

    cmdsys.add("BAL", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        long long balance = playerinfo::instance().getBalance();
        std::print("Your balance is {} pwacoins\n", balance);
        return {};
    });

    cmdsys.add("LNP", [&](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        std::print("March! March! Pwa... Introduce!\nPwacount: {}!\n\n", pwaherd.getsize());
        pwaherd.intro();
        return {};
    });

    cmdsys.add("ACH", [](std::vector<std::string>& args) -> std::expected<void, std::string> {
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        std::print("PWA ACHIEVEMENTS!\n\n");
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
        if (args.size() != 1) return std::unexpected(std::format("Expected 0 arguments, got {}\n", args.size()-1));
        std::print("Pwa, goodbye that fast?\n");
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
            std::print("Pwa data recovered!\n");
        }
        else std::print("Pwa... can't load the file... where? maybe pwa start again?\n");

        load.close();
    }

    void welcome() {
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
        pwaherd.addpwa(name);
        auto pwaplace = pwaherd.findpwa(name);
        pwaplace->setid(1);
        util::clearo();
        std::print(R"(

All done!
Now...
These alpacas needed to be fed, played with and always say pwa!
You will do this with commands!
Enter "HLP" for help!
> )");
    }

    void welcome_back() {
        std::print ("*Sleeping pwa mumbles* \nPWA! You came back? YAY PWA! You want to continue last save though? [Y/N]\n> ");
        std::string response;
        std::cin >> response;
        while (response != "N" && response != "Y") {
            std::print("Please enter Y or N\n> ");
            std::cin >> response;
        }
        if (response == "Y") {
            std::print("Pwa! The fun starts again! For recap, you have {} alpacas!\nYou remember them?\n", pwaherd.getsize());
            pwaherd.intro();
            std::print("Let's go! We start pwa! Enter HLP for help!\n> ");
            return;
        }
        util::clearo();
        std::print("Wait... you-- want to delete our world?\nBut... we had so much fun");
        pwaherd.intro();
        std::print(R"(
Look... those are the alpacas

Remember them? 

You... had have so much fun... 

...

Why...

Why would you want...

To do such cruel things

But, I cannot stop you can't I? 

Do... do you still want to say goodbye?;
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
            std::filesystem::remove("save1.txt");
            pwaherd.clear();
            auto i = playerinfo::instance().coindown(playerinfo::instance().getBalance());
            assert(i);
            std::print("Until... Gone was the world you built...\n");
            std::print("See... you... again...\n");
            welcome();
            std::print("And indeed... nothing, happened, nothing ever happened\nA world GONE\njust, like that..\n");
        }
        else {std::print("...\nYou caught yourself in the middle of the thought to delete a save, nevermind, enter \"HLP for cmd list\"\n> ");}
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
                std::print("{}\n", i.dat);
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
        else std::print("Oops, no save file found, this is an internal error, we are like, really sorry\n");
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
                std::print("{}\n",error_msg);
            }
            else {
                time_of_day++;
                time_of_day %= 5;
                if (time_of_day == 0) day_ends();
            }
            achieve_list.check();
            std::print("\nUSER_COMMAND > ");
        }

        std::print("Here is your herd info up until now\n");
        pwaherd.intro();
        std::print("Saving your data\n");
        save_file();
        std::print("Pwa-bye! See ya again\n");
    }
};

int main() {
    PWALAND game;
    game.mainpart();
}
