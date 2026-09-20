#include <print>
#include <string>
#include <utility>
#include <variant>

#include "CommandSys.hpp"
#include "ErrorHandler.hpp"

void CommandSystem::add(std::string name, Command command) {
    commands.emplace(name, command);
}

CommandSystem::Command* CommandSystem::find (const std::string& name) {
    auto it = commands.find(name);
    if (it == commands.end()) return nullptr;
    else return &it->second;
}

std::expected<std::vector<std::string>, Error> CommandSystem::parse(std::string& cmd) {
    //ignores all spaces, supports quoted strings
    if (cmd.empty()) return error(ParseError::EmptyLine);
    std::vector<std::string> a;
    std::string cur;
    bool iQuoted = false;
    a.reserve(10);
    for (char c : cmd) {
        if (iQuoted && c == '"') {
            iQuoted = false; 
            a.push_back(cur); 
            cur.clear();
            if (a.size() > 10) return error(ParseError::TooManyArgs);
            continue;
        }
        if (c == '"') {iQuoted = true; continue;}
        if (c != ' ' || iQuoted) {cur.push_back(c); continue;}
        if (!cur.empty()) {
            a.push_back(cur);
            cur.clear();
            if (a.size() > 10) return error(ParseError::TooManyArgs);
        }
    }
    if (!cur.empty()) a.push_back(cur);
    if (a.size() > 10) return error(ParseError::TooManyArgs);
    return a;
}

CommandSystem::State CommandSystem::run(std::string& cmd) {
    auto parseres = parse(cmd);
    if (!parseres) {
        ErrorHandler(parseres.error());
        meta.logfail();
        return Error;
    }
    auto& args = parseres.value();
    auto check = find(args[0]);
    if (!check) {
        ErrorHandler(NoSuchCmd{args[0]});
        meta.logfail();
        return Error;
    }
    auto res = (*check)(args);
    if (res) {
        meta.logcmd("ALL");
        meta.logcmd(args[0]);
        return Success;
    }
    if (std::holds_alternative<GameEnd>(res.error())) return Ending;
    ErrorHandler(res.error());
    meta.logfail();
    return Error;
}