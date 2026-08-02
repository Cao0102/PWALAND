#pragma once

#include <expected>
#include <string>

class Player {
    long long money = 0;

public:
    void coinup(long long amount);
    std::expected<void,std::string> coindown (long long amount);
    long long getBalance() const;
    void recoverBal(long long amount);
    void clear();
};
extern Player player;