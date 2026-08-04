#include <string>
#include <vector>
#include <print>
#include <iostream>
#include <ranges>

#include "Games.hpp"

void Gameplay::adventure(long long Entry) {
    const int base_chance = 200; //for now
    int Coins_Gained = 0;
    std::print("\nAdventure time!\n===================================\n\n");
    struct Question {
        int chance;
        std::string Q;
        std::string A;
        std::string B;
        std::string C;
        std::string D;
        char Ans;
        std::string Correct;
        std::string Wrong;
    };
    // Alright, here we go
    std::vector<Question> Q_set = {
        {
            base_chance,
            "Night falls. It's time to sleep. You have 83 alpacas, and each inn can fit a maximum of 7 alpacas, and you must not sleep with the alpacas, "
            "how many inns do you need to accompany everyone?", "A. 10", "B. 11", "C. 12", "D. 13", 'D', "Everyone slept well =)", 
            "You bought the wrong amount of inns, now the alpacas want to go home."
        }, {
            base_chance,
            "You find yourself arguing with a shepherd, who believes that sheep are better than alpacas (unacceptable), which argument do "
            "you make?", "A. Sheep are too ugly.", "B. Nobody likes sheep.", "C. Alpacas produce premium fiber.", "D. Sheep hate humans.", 'C', 
            "You won the argument, your alpacas give you a happy nod.", "The shepherd laughs at you, the alpacas are embarrassed and disappointed"
        }, {
            base_chance,
            "A merchant walks by and says he is willing to sell you an extra alpaca for only 20 pwacoins (a fantastic deal), what will you ask "
            "about the alpaca?", "A. Its color", "B. Whether it pwas", "C. Its appetite", "D. Is it raining tacos?", 'C', 
            "The sale goes through. The merchant explains everything thoroughly", "The merchant looks confused by your priorities"
        }, {
            base_chance,
            "A man saw your massive alpaca herd and immediately asked you for advice on how to manage livestock, how do you respond?",
            "A. Skill issue, get better", "B. Get emotionally attached", "C. Say nothing, just laugh", "D. Walk away", 'B', 
            "The man thanks you for your advice", "He gets confused and walks away, wondering why you are so rude?"
        }, {
            base_chance,
            "Two of your alpacas are arguing over who pwaed more this year loudly and is disrupting everyone, what will you do?",
            "A. Show the pwaing leaderboard", "B. Just ignore it", "C. Tell them the discussion is pointless", "D. Argue with them", 'A', 
            "The conflict is eventually resolved, everyone moves on", "That did nothing, the argument spreads and everyone is arguing"
        }
    };
    for (int i : std::views::iota(1,6)) {
        int result = util::rng();
        auto& Q = Q_set[result/base_chance - (result == 1000 ? 1 : 0)];
        std::print("Question {}:\n{}\n\nWhich option will you choose [A/B/C/D]?\n{}\n{}\n{}\n{}\n\nANSWER > ", i, Q.Q, Q.A, Q.B, Q.C, Q.D);
        std::string answer;
        std::getline(std::cin, answer);
        if (answer.size() != 1 || answer[0] != 'A' && answer[0] != 'B' && answer[0] != 'C' && answer[0] != 'D') {
            std::print("Pwa? That is not an answer! The alpacas are disappointed, you cannot even answer properly!\nYou lost the adventure!");
            return;
        }
        if (answer[0] != Q.Ans) {
            std::print("{}\nYou lost the adventure!", Q.Wrong);
            return;
        }
        Coins_Gained += 20 + 10*i + util::rng()/100;
        std::print("{}\nYour prize pool is now {} pwacoins! Do you want to continue for better prizes, or end the game now [Y/N]?\nCONTINUE?> ", Q.Correct, Coins_Gained);
        std::string response;
        std::getline(std::cin, response);
        while (response != "Y" && response != "N") {std::print("Please enter 'Y' or 'N'!\nCONTINUE?> "); std::getline(std::cin, response);}
        if (response == "N") {
            std::print("Ending the adventure!\nAdding your prizes to your coins!\n");
            player.coinup(Coins_Gained);
            std::print("You now have {} coins!\n See you next time!", player.getBalance());
            return;
        }
        std::print("Proceeding to the next question!\n");
    }
    Coins_Gained += Entry*3; 
    player.coinup(Coins_Gained);
    std::print("You really reached the end! Yay!!!\nYou got the grand jackpot of {}! Your total prize is {} pwacoins!\nBye!", Coins_Gained, player.getBalance());
}