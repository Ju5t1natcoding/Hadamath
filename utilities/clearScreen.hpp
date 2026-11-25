#pragma once
#include <iostream>
#include <cstdlib>
#include <cstdio>

inline void clearScreen() {
    #ifdef _WIN32
        std::system("cls");
    #elif defined(__linux__) || defined(__APPLE__)
        std::system("clear")
    #else
        std::cout << "\033[2J\033[1;1H"; ///https://stackoverflow.com/questions/17335816/clear-screen-using-c
    #endif
}