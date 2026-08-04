#include "Initialization.hpp"
#include "Achievements.hpp"
#include "SaveManager.hpp"

void Initialization::run(CommandSystem& cmdsys, Herd& Alpacaherd) {
    cmdsys = setup_commands(Alpacaherd);
    Achievements.setup();
    Save.savecheck(Alpacaherd);
}
Initialization setup;