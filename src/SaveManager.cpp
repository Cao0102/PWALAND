#include <fstream>
#include <print>
#include <filesystem>

#include "SaveManager.hpp"
#include "ErrorHandler.hpp"

std::expected<void, Error> SaveManager::load(Herd& pwaherd) {
    std::ifstream load("save1.txt");

    if (load.is_open()) {
        int SaveVInFile; int lastDaily;
        load >> SaveVInFile;
        if (SaveVInFile != SAVE_VERSION) return error(InternalError::SaveVersionMismatch);
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
    else return error(InternalError::SaveReadFailed);

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
    else ErrorHandler(error(InternalError::SaveWriteFailed).error());
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
        else {ErrorHandler(load_result.error()); welcomer.welcome(pwaherd);}
    }
    else welcomer.welcome(pwaherd);
}
SaveManager Save;