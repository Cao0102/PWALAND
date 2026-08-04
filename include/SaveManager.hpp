#pragma once

#include <expected>
#include <string>

#include "Player.hpp"
#include "Metadata.hpp"
#include "Constants.hpp"
#include "AlpacaHerd.hpp"
#include "Onboarding.hpp"
#include "Achievements.hpp"

class SaveManager {
public:
    std::expected<void, std::string> load(Herd& pwaherd);
    void save(Herd& pwaherd);
    void savecheck(Herd& pwaherd);
};
extern SaveManager Save;