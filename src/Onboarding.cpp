#include <print>
#include <iostream>
#include <string>
#include <filesystem>

#include <Onboarding.hpp>


void Onboarding::welcome(Herd& pwaherd) {
    std::print(R"(
PWA WELCOME TO PWALAND!
----------------------------------
Newcommer pwa-eh? Pwa! You must be soooo pwa confused
Someone ought to show you how things are done here
You start off having one ALPACA. But soon will be many!
How would you name your first alpaca? (no spaces!)
> )");
    std::string name;
    std::cin >> name;
    (pwaherd.addpwa(name)).setid(1);
    util::clearo();
    std::print(R"(

All done!
Now...
These alpacas needed to be fed, played with and always say pwa!
You will do this with commands!
Enter "HLP" for help!
)");
}

void Onboarding::welcome_back(Herd& pwaherd) {
    std::print ("*Sleeping pwa mumbles* \nPWA! You came back? YAY PWA! You want to continue last save though? [Y/N]\n> ");
    std::string response;
    std::cin >> response;
    while (response != "N" && response != "Y") {
        std::print("Please enter Y or N\n> ");
        std::cin >> response;
    }
    if (response == "Y") {
        std::print("Pwa! The fun starts again!\n\nFor recap, you have {} alpacas!\nYou remember them?\nHere is all of them!\n\n", pwaherd.getsize());
        pwaherd.intro();
        std::print("\nLet's go! We start pwa! Enter HLP for help!\n");
        util::clearo();
        return;
    }
    util::clearo();
    std::print("Wait... you-- want to delete our world?\nBut... we had so much fun!\n\n");
    pwaherd.intro();
    std::print(R"(
Look... those are the alpacas

Remember them? 

You... had have so much fun... 

...

Why...

Why would you want...

To do such cruel things

But... I suppose I can't stop you...

Do... do you still want to say goodbye?

Proceed
Do Not
> )");
    std::getline(std::cin, response);
    while (response != "Proceed" && response != "Do Not") {
        std::print("\tProceed\n\tDo Not\n> ");
        std::getline(std::cin, response);
    }
    if (response == "Proceed") {
        std::print("Suddenly, the world starts fading, and fading, ...\n");
        util::delay(500);
        std::filesystem::remove("save1.txt");
        pwaherd.clear();
        player.clear();
        std::print("Until... Gone was the world you built...\n");
        util::delay(5000);
        std::print("\n\nAnd indeed... We guess...\n");
        util::delay(1000);
        std::print("Nothing, happened, nothing ever happened\n");
        util::delay(1000);
        std::print("A world GONE\n");
        util::delay(600);
        std::print("Just, like that...\n");
        util::delay(2000);
        std::print("No... We won't blame you...\n");
        util::delay(600);
        std::print("Just...\n");
        util::delay(1000);
        std::print("Next time...\n");
        util::delay(2000);
        std::print("Remember them, will you? The memories...\n");
        util::delay(3000);
        std::print("Let's begin again...\n\n");
        util::delay(1000);
        welcome(pwaherd);
    }
    else {std::print("...\nYou caught yourself in the middle of the thought to delete a save, nevermind, enter \"HLP for cmd list\"");}
}
Onboarding welcomer;