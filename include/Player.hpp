#pragma once

#include <expected>

#include "Error_types.hpp"

class Player {
	unsigned long long money = 0;

public:
	void coinup(long long amount);
	std::expected<void,Error> coindown (unsigned long long amount);
	long long getBalance() const;
	void recoverBal(long long amount);
	void clear();
};
extern Player player;