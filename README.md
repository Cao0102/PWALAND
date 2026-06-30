# PWALAND
## WELCOME! TRAVELLERS!
*This is of course a normal introduction to PWALAND*
<sub>Not normal</sub>

***Please be aware that the game is indeed in development and the code may be messy***

## What is PWALAND?
PWALAND is apparently my side little project. It is a text based terminal "game" (if you'd call it so).

## What can I do inside PWALAND?
Apparently and up until now, you could let alpacas "PWA" which is, you can imagine, dogs bark or cat meows.
You could also feed them, play with them, add more alpacas and gain several achievements!

## How will I do these things?
You will do them using **commands** in the terminal!
Generally a command will have this form:
`<command_name> <argument_1> <argument_2> <...>`
Each of the command has its own amount of arguments and what those aguments will represent!
For example: `FED Brown 5`
As you see, `FED` is the command name, this refers to the feeding command.
`Brown` is the alpaca name (must be exact, case sensitive!).
`5` is the amount, aka how many times will you feed this alpaca?

## What commands do we have?
***Please be aware that you could also access these using the `HLP` command***
***This will be updated***

1. HLP - The help command, show commands details
HLP takes no argument, all you need to enter is `HLP`
It will list all commands that exist, excluding only commands marked secret

2. FAQ - Common questions and errors
FAQ takes no argument, all you need to enter is `FAQ`
It will list *prewritten* Frequently-Asked-Questions.

3. JGL - Show the changelog for the recent updates.
JGL takes no argument, all you need to enter is `JGL`
It will list all recent updates to the game.

4. MTD - Shows some data about your gameplay.
MTD takes no argument, all you need to enter is `MTD`
Shows several data about your gameplay, such as command usage, total level, total pwas, total command fail (yes we count that :D)

5. END - Stop the program and exit
END takes no argument, all you need to enter is `END`
End the program and exit, changes are saved.

6. BAL - Show your pwacoin balance!
BAL takes no argument, all you need to enter is `BAL`
Show your pwacoin (the in game currency) balance

7. ACH - Show all achievements
ACH takes no argument, all you need to enter is `ACH`
Show all achievements. Apparently there is no sorting or filtering

8. AIF - View information on a specific achievement
AIF takes one argument - the achievement name
`AIF <name>`
The name must be exact, and there is not matching feature yet, so it is prefered that you use `ACH` instead
Show specific info on an achievement

9. INF - Show information on your alpaca
INF takes one argument - the alpaca name
`INF <name>`
The name must be exact as you originally entered it, there is no matching or suggestion, so it is prefered that you name your alpacas something memorable and easy to enter
Show your alpaca's details and statistics

10. FED - Feed the alpaca
FED takes two arguments - the alpaca name and the amount
`FED <name> <amount>`
The name must be exact as you originally entered it, there is no matching or suggestion, so it is prefered that you name your alpacas something memorable and easy to enter
The amount must be a **positive integer below 10000**
**Accepted amounts: "1", "2", "5", "7", "99", "9999"**
**Not accepted amounts: "0", "10000", "9999999", "-3", "two", "95 thousand"**
Please be aware that this operation costs pwacoins
Feed the alpaca given to level them up

11. PWA - Let them PWA!
PWA takes two arguments - the alpaca name and the amount
`PWA <name> <amount>`
The name must be exact as you originally entered it, there is no matching or suggestion, so it is prefered that you name your alpacas something memorable and easy to enter
The amount must be a **positive integer below 10000**
**Accepted amounts: "1", "2", "5", "7", "99", "9999"**
**Not accepted amounts: "0", "10000", "9999999", "-3", "two", "95 thousand"**
Let the alpaca says pwa =D

12. PLY - Play with your alpaca!
PLY takes one argument - the alpaca name
`PLY <name>`
The name must be exact as you originally entered it, there is no matching or suggestion, so it is prefered that you name your alpacas something memorable and easy to enter
Please be aware that this operation costs pwacoins
Play with your alpaca, they will pwa and gain a random amount of xp

13. ADD - Grow your herd! Add another alpaca!
ADD takes one argument - the alpaca name
`ADD <name>`
Please remember this name! It is prefered that you name your alpacas something memorable and easy to enter.
Please be aware that this operation costs pwacoins
Add another alpaca into your herd

14. LNP - Show your alpaca formation!
LNP takes no argument, all you need to enter is `LNP`
All your alpacas will introduce themselves with their own statitics.

## What language is this written in?
The entirety of the code is written in C++23

## How to compile this?
Compile normally using C++23 and preferably a GNU compiler
Prefered compile command: `g++ main.cpp -o game.exe -std=c++23 -O3 -Wall -Wextra -Wpedantic -Werror`
Yes, you will complile on main.cpp

## Real talk...
This is a personal hobby project not made for any assignment at all and gotta say, it was fun!
I will not put any licensing on this project, at least in the short term future
That being said please do not distribute the code or game without my consent and credit for me, still
=D