#pragma once

#include <string>
#include <functional>
#include <expected>
#include <vector>
#include <unordered_map>

#include "Metadata.hpp"
#include "Error_types.hpp"

class CommandSystem {
public:
    using Command = std::function<std::expected<void, Error>(std::vector<std::string>& args)>;
private:
    std::unordered_map<std::string, Command> commands;
public:
    void add(std::string name, Command command);
    Command* find (const std::string& name);
    std::expected<std::vector<std::string>, Error> parse(std::string& cmd);
    enum State {
        Success,
        Error,
        Ending
    };
    State run(std::string& cmd);
};