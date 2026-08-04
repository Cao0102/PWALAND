#include <fstream>
#include <print>
#include <filesystem>

#include "SaveManager.hpp"

std::expected<void, std::string> SaveManager::load(Herd& pwaherd) {
    std::ifstream load("save1.txt");

    if (load.is_open()) {
        int SaveVInFile; int lastDaily;
        load >> SaveVInFile;
        if (SaveVInFile != SAVE_VERSION) return std::unexpected("Save file version mismatch");
        load >> lastDaily;
        meta.loglastdaily(lastDaily);
        int nAlpacas;
        long long coinmount;
        load >> coinmount >> nAlpacas;
        player.recoverBal(coinmount);
        pwaherd.recoverpwa(nAlpacas, load);
        meta.loadin(load);
        std::print("Pwa data recovered!\n");
    }
    else return std::unexpected("Pwa... System error... Failed to load save file");

    load.close();
    return {};
}

void SaveManager::save(Herd& pwaherd) {
    std::ofstream save("save1.txt");
    if (save) {
        save << SAVE_VERSION << '\n';
        save << meta.getlastdaily() << '\n';
        save << player.getBalance() << '\n';
        save << pwaherd.getsize() << '\n';
        pwaherd.savepwa(save);
        meta.saveto(save);
        save.close();
        return;
    }
    else std::print("Oops, we cannot open the safe file, this is an internal error, we are sorry pwa...\n");
    save.close();
}

void SaveManager::savecheck(Herd& pwaherd) {
    if (std::filesystem::exists("save1.txt")) {
        auto load_result = load(pwaherd);
        if (load_result) {
            std::print("PWA LOAD SUCESSFUL!\n");
            Achievements.save_sync();
            welcomer.welcome_back(pwaherd);
        }
        else {std::print("Error: {}\nPerhaps this is an internal error, for now we can restart the game\n", load_result.error()); welcomer.welcome(pwaherd);}
    }
    else welcomer.welcome(pwaherd);
}
SaveManager Save;