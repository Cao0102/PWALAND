#include <format>

#include "Player.hpp"

void Player::coinup(long long amount) {money += amount;}

std::expected<void, Error> Player::coindown (ull amount) {
    if (money < amount) return error(NoMoney{amount, money});
    money -= amount;
    return {};
}

long long Player::getBalance() const {return money;}
void Player::recoverBal(long long amount) {money = amount;}
void Player::clear() {money = 0;}
Player player;