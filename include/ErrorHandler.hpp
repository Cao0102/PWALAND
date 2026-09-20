#pragma once

#include <string>
#include <print>
#include <utility>

#include "Error_types.hpp"

struct HandlerAction {
    void operator()(InternalError x) {
        std::string s;
        switch (x.type) {
            case InternalError::SaveCorrupt: 
                s = "SaveCorrupt";
                break;
            case InternalError::SaveNotFound:
                s = "SaveNotFound";
                break;
            case InternalError::SaveWriteFailed:
                s = "SaveWriteFailed";
                break;
            case InternalError::SaveReadFailed:
                s = "SaveReadFailed";
                break;
            case InternalError::SaveVersionMismatch:
                s = "SaveVersionMismatch";
                break;
            default:
                s = "Unknown";
                break;
        }
        std::print("Oops... this seems to be an internal error of type: {}, please refer the FAQ.", s);
    }

    void operator()(ParseError x) {
        std::string s;
        switch (x.type) {
            case ParseError::EmptyLine: 
                s = "PWA... EMPTY LINE PWA";
                break;
            case ParseError::TooManyArgs: 
                s = "Pwa that's too many arguments... More than pwa ever need....";
                break;
        }
        std::print("{}", s);
    }

    void operator()(NoSuchCmd x) {
        std::print("Pwa? You are looking for [{}]? But that does not exist!", x.cmd);
    }

    void operator()(ArgumentError x) {
        std::print("Pwa! This command asked for {} arguments, but you give {}!", x.expected, x.got);
    }

    void operator()(NoSuchAlpaca x) {
        std::print("Pwa please don't tell us you forgot our names. [{}] doesn't exist!", x.name);
    }

    void operator()(InvalidNumber x) {
        switch(x.type) {
            case InvalidNumber::NotANumber:
                std::print("Pwa! That is definitely not a number!");
                break;
            case InvalidNumber::TrailingChar:
                std::print("Hmph, owner thinks he can trick us with the trailing characters!");
                break;
            case InvalidNumber::TooBig:
                std::print("Pwa... that's sooooooo bigggggg");
                break;
            case InvalidNumber::Negative:
                std::print("Pwa? What is that minus sign??? Pwa no understand!");
                break;
            case InvalidNumber::Zero:
                std::print("Pwa why would you do something 0 times?");
                break;
        }
    }

    void operator()(NoMoney x) {
        std::print("Pwa... *checks your wallet*. The action takes {}, but you only "
        "have {} pwacoins!\nThey look at you, judging your financial stability", x.needed, x.got);
    }

    void operator()(NoSuchAchievement x) {
        std::print("Hmmmm... There is no achievement called [{}], pwa!", x.name);
    }

    void operator()(AlpacaAlreadyExist x) {
        std::print("An alpaca with that name already exist, wouldn't be so convenient if "
        "every alpaca is named {}, would it?", x.name);
    }

    void operator()(AlreadyDaily x) {
        std::print("Be patient pwa! You already took your daily rewards today");
    }
    void operator()(GameEnd) {
        std::unreachable();
    }
};

inline void ErrorHandler (Error H) {
    std::visit(HandlerAction{}, H);
    std::print("\n");
}