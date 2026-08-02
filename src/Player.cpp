#include <format>

#include "Player.hpp"

void Player::coinup(long long amount) {money += amount;}

std::expected<void,std::string> Player::coindown (long long amount) {
    if (money < amount) return std::unexpected(std::format("Not enough money pwa... You need {} more", amount-money));
    money -= amount;
    return {};
}

long long Player::getBalance() const {return money;}
void Player::recoverBal(long long amount) {money = amount;}
void Player::clear() {money = 0;}
Player player;