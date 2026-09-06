# UTILITIES OVERVIEW
A lot of utilities were written for the convenience of writting code  
The following is an overview on some of them and how to use them

## How to get access to utilities?
Simply include the utility header
```cpp
#include "utilities.hpp"
```
**All utility functions**  are under `namespace util`, so always have `util::` before the function name  

## What utilities are there
1. `util::clearo()`
Soon will be renamed  
Flushes the output  
Takes 0 parameters

2. `util::w_rand<T, W...>(std::array<T, sizeof...(W)> a)`
Takes a weighted random elements
Template parameters: `T` - type of element and `W...` - Weight of elements  
Parameters: std::array of `T`  
**Example:**
```cpp
util::w_rand<Alpaca, 500, 300, 200>({Brown, Green, Yellow});
```

3. `util::rng()`
Give a random number from **1 to 1000**
No parameters

4. `util::parse_num(std::string& number)`
Translate a number from string to int
Returns an `std::expected<int,string>`
Parameter: the string containing a number

To be completed