#pragma once
#include <iostream>
#include <cstdlib>
#include <cstdio>

inline void clear_screen() {
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #elif defined(__linux__) || defined(__APPLE__)
        std::system("clear")
    #else
        std::cout << "\033[2J\033[1;1H"; ///https://stackoverflow.com/questions/17335816/clear-screen-using-c
    #endif
}