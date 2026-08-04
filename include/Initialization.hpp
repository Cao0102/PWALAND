#pragma once

#include "CommandSys.hpp"
#include "AlpacaHerd.hpp"

class Initialization {
    CommandSystem setup_commands(Herd& pwaherd);
public:
    void run(CommandSystem& cmdsys, Herd& Alpacaherd);
};
extern Initialization setup;