# PWALAND
## WELCOME! TRAVELLERS!
*This is of course a normal introduction to PWALAND*  
***<sub>Ok MAYBE not normal</sub>***  

***Please be aware that the game is indeed in development and the code may be messy***  

## What is PWALAND?
PWALAND is apparently my side little project. It is a text based terminal "game" (if you'd call it so).  

## What can I do inside PWALAND?
The games revolves around the existence of alpacas and your herd, which you will take care of.  
You could also feed them, play with them, add more alpacas and gain several achievements!  

## How will I do these things?
You will do them using **commands** in the terminal!  
Generally a command will have this form:  
`<command_name> <argument_1> <argument_2> <...>`  
You could also use double braces like  
`"<command_name>" <argument_1> "<argument_2>"`
Or with any other combinations, this might be useful if you want many words in one argument  
Each of the command has its own amount of arguments and a *scheme* to determin what each means (usually fixed)    
For example: `FED Brown 5`  
As you see, `FED` is the command name, this refers to the feeding command.  
`Brown` is the alpaca name (must be exact, case sensitive!).  
`5` is the amount, aka how many times will you feed this alpaca?  
**For specific implementation details, please scroll down**  

## What language is this written in?
The entirety of the code is written in C++26  

## How to compile this?
Requirements:
- **Compiler with C++26 support**. This is actually crucial, the code uses elements only available under the C++26 language standard.
- **CMake 3.20 or newer**  
Instructions:  
- Build in CMake, either through your editor's extension or through terminal
- Run the file through the executable directly or through your code editor of choice

## What commands do we have?  
***Please be aware that you could also access these using the `HLP` command***  
***This will be updated***  

### Before that: TAGS
**Name carefully**: The name is case sensitive, and there is no name matchmaking, so remember your names, name your alpacas something memorable
**Use quotes**: Use double quotes to group words into one argument in order to ensure correct parsing
**Cost money**: This operation will cost money (pwacoins!)
**Valid integer**: You must enter a valid positive integer that is below 10000
**All or nothing**: This activity includes an all or nothing mechanic

1. HLP - The help command, show commands details  
HLP takes no argument, all you need to enter is `HLP`  
It will list all commands that exist, excluding only commands marked secret  

2. FAQ - Common questions and errors  
FAQ takes no argument, all you need to enter is `FAQ`  
It will list *prewritten* Frequently-Asked-Questions.  

3. MTD - Shows some data about your gameplay.  
MTD takes no argument, all you need to enter is `MTD`  
Shows several data about your gameplay, such as command usage, total level, total pwas, total command fail (yes we count that :D)

4. END - Stop the program and exit
END takes no argument, all you need to enter is `END`  
End the program and exit, changes are saved.

5. BAL - Show your pwacoin balance!
BAL takes no argument, all you need to enter is `BAL`  
Show your pwacoin (the in game currency) balance

6. ACH - Show all achievements
ACH takes no argument, all you need to enter is `ACH`  
Show all achievements. Apparently there is no sorting or filtering

7. AIF - View information on a specific achievement  
AIF takes one argument - the achievement name  
`AIF "<name>"`  
**Tags**: Name carefully, Use Quotes  
Show specific info on an achievement

8. DLY - Claim daily reward  
DLY takes no argument, all you need to enter is `DLY`  
Claim your daily reward, rewards are random  

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
Let the alpaca says "PWA" =D  

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

15. ADV - Go on an adventure!  
ADV takes no argument, all you need to enter is `ADV`  
**Tags**: Costs money, All or nothing  
Go on an all or nothing adventure with your alpacas and answer questions for the grand prize!  

## Final notes
This is a personal hobby project not made for any assignment at all and gotta say, it was fun!
I will not put any licensing on this project, at least in the short term future, meaning: All rights reserved
That being said please do not distribute the code or game without my consent and credit for me
=D

<br>
<br>

-----------------------------------------------------------------------
# BELOW IS SEVERAL EXTRAS, YOU MAY NOT FIND THIS SECTION INTERESTING  
Yes, If you are a normal player, you could stop reading now

## COMMAND EXTRA: ADMIN
The `ADMIN` command takes a non fixed amount of arguments and follow a fixed schema, several options include  
`ADMIN "Save clear"`, delete the save currently loaded without going through the usual pathway  

## IMPLEMENTATION OVERVIEW
*These are specific implementation overview of the system*  

**Setup and loading**
- The program initializes by loading save file `save1.txt`, if this file does not exist then the run will be considered new.
    - If it is a new run, there will be no prior data saved and an introductory (onboarding) sequence to teach the players on the mechanics of the game and award them their first alpaca (see: `Onboarding.hpp` and `Onboarding.cpp`)
    - If there is an existing save, the program will check the save file version (see: `SAVE_VERSION`, this is crucial for save file integrity), all information related to Alpacas, metadata, anything that cannot be derived from other parts will be loaded from the file.
    - If there is a mismatch on the save file version or the save file cannot be opened, the program will inform the user and start fresh
- Other important parts that can be derived from other loaded data (such as achievements deriving on metadata), this derivation will happen silently before the main game loop.

**Input layer:**
- The player inputs and enter commands through the terminal, each line is received by the command registry independently (See: `CommandSys.hpp` and `.cpp`)
- Within the command registry, the **parser** is called. This simple parser has two modes, specifically the **default** (with spaces as delimiters) and **quoted** (within double quotes, everything is included in one token)
- These tokens are returned as an `std::vector<std::string>`, the arguments. The first token (meaning, the command name) is used to determine what command is called.

**Commands layer:**
- The commands receive the argument lists and verify their own arguments
    - Most commands will verify if the size of the argument list (meaning, the number of arguments passed) match with the needed arguments
    - Then each argument's validity is examined.
    - Numeric arguments are converted from `std::string` to `int` using `util::str_to_num`, returning an `std::expected<int, std::string>`. All numbers must be a **valid positive integer**, **lower than 1e4** and **mustn't have any trailing or in-between characters**
    - Name arguments (meaning: alpaca or achievement names) are checked inside `AchievementsManager::show` or using `Herd::findpwa` depending on the semantic meaning and contexts (**Important**: Names are **case sensitive** and there is **no matching**)
    - Other exclusive argument may be checked differently, refer to the `HLP` command (during gameplay) or the brief on commands above
    - A command can take no argument
- After checking validity, the command executes according to its purpose, calling other system parts
- Internally, the command registry is implemented using an unordered map of name as keys and callables as values, taking a vector of strings (the arguments) returning void if successful and a string (error message) if an error occurred, specifically `std::unordered_map<std::string, std::function<std::expected<void, std::string>(std::vector<std::string>)>>`. If an error occurred, the function will exit early, returning an error string, which the command registry will output to the terminal

**The herd and alpacas**
- The herd serves as an **interface** to each individual alpacas with utility functions such as `pwafind` returning a pointer to the individual alpaca or iterate through the alpaca herd for commands like `LNP`
- The herd will receive calls mainly from the command registry in order to interact with the alpacas.
- The herd is internally implemented using an unordered map of name to alpacas, specifically `std::unordered_map<std::string, Alpaca>`
- Each individual Alpacas stores individual state like `pwatimes`, the number of times it had pwa-ed or levels and personal ID. It will also contain the behaviors of the alpacas such as responding to feeding, leveling up, etc. (see: `Alpaca.hpp` and `.cpp`)

**Metadata and Achievements**
- The metadata stores all information regarding your gameplay or your alpaca herd with a general use for **viewing** and **checking achievements**
- The metadata system is implemented using an unordered map of category names to categories, with categories being a map of metadata name to value, specifically `std::unordered_map <std::string, std::map<std::string, long long>>` with several layers of abstraction down to several command calls through `class metause`, most of which starts with the prefix `get-` or `log-`, though there are other commands such as `saveto()`, `loadin()` or `listout` (see: `Metadata.hpp` and `.cpp`)
- Achievements rely solely on metadata with several helper commands to be called from the outside and internal helpers such as `hide()` to hide several secret achievements or `class requirements` providing a convenient template to help with the adding of new achievements to the achievement list.
- The achievement list is implemented using an unordered map of name to achievements, specifically `std::unordered_map<std::string, Achievements>` with helper functions for external or internal usage.

**Miscellaneous Components**
- The save system overrides the previous save, saving to file `save1.txt` under a fixed scheme
- There exists other systems such as utility functions (see: `namespace util`) or player info (such as the amount of pwacoins a player has) but is not significant enough to cover in this comprehensive overview. Please look into the code if necessary.
