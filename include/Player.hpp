#pragma once

#include <expected>
#include <string>

#include "Error_types.hpp"

class Player {
    ull money = 0;

public:
    void coinup(long long amount);
    std::expected<void,Error> coindown (ull amount);
    long long getBalance() const;
    void recoverBal(long long amount);
    void clear();
};
extern Player player;