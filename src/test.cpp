#include <filesystem>
#include <iostream>

int main() {
    std::cout << std::filesystem::exists("save1.txt") << '\n';
    std::filesystem::remove("save1.txt");
}