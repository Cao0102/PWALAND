#include <vector>

#include "CommandSys.hpp"

void CommandSystem::add(std::string name, Command command) {
    commands.emplace(name, command);
}

CommandSystem::Command* CommandSystem::find (const std::string& name) {
    auto it = commands.find(name);
    if (it == commands.end()) return nullptr;
    else return &it->second;
}

std::expected<std::vector<std::string>, std::string> CommandSystem::parse(std::string& cmd) {
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

std::expected<void, std::string> CommandSystem::run(std::string& cmd) {
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