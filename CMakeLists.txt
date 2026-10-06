cmake_minimum_required(VERSION 3.28)

# Unused for now
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD
    "f35a9ac6-8463-4d38-8eec-5d6008153e7d"
)

project(PWALAND)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Potentially unused for now.
set(NLOHMANN_JSON_BUILD_MODULES ON)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

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
    src/CommandLoader.cpp
    src/Setup.cpp
)

# Unused for now
# Until GCC fix their bug
# Turn CXX_MODULE_STD on when use
set_property(
    TARGET PWALAND
    PROPERTY CXX_MODULE_STD OFF
)

target_sources(
    PWALAND
    PRIVATE
        FILE_SET CXX_MODULES
        FILES
            #
)

target_include_directories(PWALAND PRIVATE include)

target_link_libraries(
    PWALAND PRIVATE
    stdc++exp
    nlohmann_json::nlohmann_json
)