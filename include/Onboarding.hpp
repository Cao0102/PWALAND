#pragma once

#include "AlpacaHerd.hpp"
#include "Utilities.hpp"
#include "Player.hpp"

class Onboarding {
public:
    void welcome(Herd& pwaherd);
    void welcome_back(Herd& pwaherd);
};
extern Onboarding welcomer;