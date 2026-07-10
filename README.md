# PWALAND
## WELCOME! TRAVELLERS!
*This is of course a normal introduction to PWALAND*  
***<sub>Ok MAYBE not normal</sub>***  

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

### Before that: TAGS
**Name carefully**: The name is case sensitive, there is no name matchmaking, so remember your names, name your alpacas something memorable
**Use quotes**: Use quotation mark to ensure correct parsing
**Cost money**: This operation will cost money
**Valid integer**: You must enter a valid positive integer that is numeral and below 10000

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
`AIF "<name>"`  
**Tags**: Name carefully, Use Quotes  
Show specific info on an achievement

9. INF - Show information on your alpaca  
INF takes one argument - the alpaca name  
`INF <name>`  
**Tags**: Name carefully  
Show your alpaca's details and statistics

10. FED - Feed the alpaca
FED takes two arguments - the alpaca name and the amount  
`FED <name> <amount>`  
**Tags**: Name carefully, Costs money, Valid integer  
Feed the alpaca given to level them up  

11. PWA - Let them PWA!
PWA takes two arguments - the alpaca name and the amount  
`PWA <name> <amount>`  
**Tags**: Name carefully, Valid integer  
Let the alpaca says pwa =D  

12. PLY - Play with your alpaca!
PLY takes one argument - the alpaca name  
`PLY <name>`  
**Tags**: Name carefully, Costs money  
Play with your alpaca, they will pwa and gain a random amount of xp

13. ADD - Grow your herd! Add another alpaca!  
ADD takes one argument - the alpaca name  
`ADD <name>`  
**Tags**: Name carefully, Costs money  
Add another alpaca into your herd

14. LNP - Show your alpaca formation!  
LNP takes no argument, all you need to enter is `LNP`  
All your alpacas will introduce themselves with their own statistics.

## What language is this written in?
The entirety of the code is written in C++26  

## How to compile this?
Compile normally using C++26 and preferably a GNU compiler
Preferred compile command: `g++ main.cpp -o game.exe -std=c++26 -lstdc++exp -O3 -Wall -Wextra -Wpedantic -Werror`
Yes, you will complile on main.cpp
Yes, I ALSO know that C++26 is not even done, but gnu had indeed made its features and I kinda used some of them  
Trust me on the `-std=c++26 -lstdc++exp -O3` fr =D  

## Real talk...
This is a personal hobby project not made for any assignment at all and gotta say, it was fun!
I will not put any licensing on this project, at least in the short term future, meaning: All rights reserved
That being said please do not distribute the code or game without my consent and credit for me
=D