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

namespace util {
    inline void delay(int ms) {std::this_thread::sleep_for(std::chrono::milliseconds(ms));}
    inline int get_date() {
        auto now = std::chrono::system_clock::now();
        auto today = std::chrono::floor<std::chrono::days>(now);
        std::chrono::year_month_day date{today};
        int year = (int) date.year();
        int month = (unsigned int) date.month();
        int day = (unsigned int) date.day();
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
    inline std::expected<int, std::string> parse_num(const std::string& str) {
        int value;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
        if (ec == std::errc::invalid_argument) return std::unexpected("Pwa? Isn't this supposed to be... a numbber?");
        if (ec == std::errc::result_out_of_range || value >= 10000) return std::unexpected("Pwa... big number...");
        if (value < 0) return std::unexpected("But pwa no learn negative numbers!");
        if (value == 0) return std::unexpected("Pwa why would you do something 0 times?");
        if (ptr != str.data() + str.size()) return std::unexpected("Tricky owner sneak trailing characters!");
        return value;
    }
    inline std::string Argnum_err(int expect, int got) {return std::format("Expected {} {}, got {}", expect, expect == 1 ? "argument" : "arguments", got);}
    inline std::string Nopwa_err() {return "No such alpaca pwa!";}
}