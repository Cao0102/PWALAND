#pragma once

#include <string>
#include <functional>
#include <expected>

#include "Metadata.hpp"

class CommandSystem {
public:
    using Command = std::function<std::expected<void, std::string>(std::vector<std::string>& args)>;
private:
    std::unordered_map<std::string, Command> commands;
public:
    void add(std::string name, Command command);
    Command* find (const std::string& name);
    std::expected<std::vector<std::string>, std::string> parse(std::string& cmd);
    std::expected<void, std::string> run(std::string& cmd);
};