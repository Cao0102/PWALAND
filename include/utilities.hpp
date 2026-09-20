#pragma once

#include <iostream>
#include <string>
#include <limits>
#include <expected>
#include <random>
#include <format>
#include <charconv>
#include <chrono>
#include <thread>
#include <array>
#include <ranges>
#include <cassert>

#include "Error_types.hpp"

namespace util {
    template<typename T, typename K>
    concept Castable = requires(K x) { static_cast<T>(x); };
    template<typename T, typename K>
        requires Castable<T, K>
    constexpr T to(K x) {
        return static_cast<T>(x);
    }
    inline void delay(int ms) {std::this_thread::sleep_for(std::chrono::milliseconds(ms));}
    inline int get_date() {
        auto now = std::chrono::system_clock::now();
        auto today = std::chrono::floor<std::chrono::days>(now);
        std::chrono::year_month_day date{today};
        int year = to<int>(date.year());
        int month = to<unsigned int>(date.month());
        int day = to<unsigned int>(date.day());
        return year*10'000 + month*100 + day;
    }
    inline void clearo() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    inline int rng() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> rand(1, 1000);
        return rand(gen);
    }
    inline std::expected<int, Error> parse_num(const std::string& str) {
        int value;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
        if (ec == std::errc::invalid_argument) return error(InvalidNumber::NotANumber);
        if (ptr != str.data() + str.size()) return error(InvalidNumber::TrailingChar);
        if (ec == std::errc::result_out_of_range || value >= 10000) return error(InvalidNumber::TooBig);
        if (value < 0) return error(InvalidNumber::Negative);
        if (value == 0) return error(InvalidNumber::Zero);
        return value;
    }
    
    template <typename T, int... W>
        requires((W + ...) == 1000)
    inline T w_rand(const std::array<T, sizeof...(W)>& a) {
        int result = rng();
        constexpr std::array w = {W...};
        for (int index : std::views::iota(0, to<int>(w.size()))) if ((result -= w[index]) <= 0) return a[index];
        assert(false);
    }
}