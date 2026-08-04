cmake_minimum_required(VERSION 3.20)

project(PWALAND)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PWALAND
    src/main.cpp
    src/Metadata.cpp
    src/Achievements.cpp
    src/Player.cpp
    src/Alpaca.cpp
    src/CommandSys.cpp
    src/AlpacaHerd.cpp
    src/Adventure.cpp
    src/Onboarding.cpp
    src/SaveManager.cpp
)

target_include_directories(PWALAND PRIVATE include)
target_link_libraries(PWALAND PRIVATE stdc++exp)