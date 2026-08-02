#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <expected>
#include <vector>

#include "Metadata.hpp"

namespace achievement_state {
    enum class Status {
        Already,
        JustDone,
        NotDone
    };
}

class Achievement {
    std::string name;
    std::string description;
    std::vector<std::function<bool()>> condition;
    bool completed = false;
    bool secret = false;
public:
    Achievement(std::string n, std::string d, std::vector<std::function<bool()>> c);
    achievement_state::Status evaluate();
    void hide();
    void info();
    void complete();
};

class AchievementsManager {
    std::unordered_map<std::string, Achievement> achievement_list;
public:
    void add(std::string name, std::string description,std::vector<std::function<bool()>> conditions);
    void hide(const std::string& name) ;
    class requirements {
    public:
        std::vector<std::function<bool()>> pwa(int amount);
        std::vector<std::function<bool()>> fal(int amount);
        std::vector<std::function<bool()>> lvl(int amount);
        std::vector<std::function<bool()>> cmd(const std::string& name, int amount);
    };
    void setup();
    std::expected<void, std::string> show (std::string& name);
    void list_out();
    void check();
    void save_sync();
};
extern AchievementsManager Achievements;