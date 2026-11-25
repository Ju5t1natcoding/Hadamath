#pragma once
#include <cstdint>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

namespace Keyboard {
    enum class Key {
        UP, DOWN, ENTER, ESC, C, NUM_1, NUM_2, NUM_3, OTHER
    };

    #ifndef _WIN32

    inline Key readChar() {
        termios oldSettings, newSettings;
        tcgetattr(STDIN_FILENO, &oldSettings); ///saves terminal configuration
        newSettings = oldSettings;
        newSettings.c_lflag &= ~(ICANON | ECHO);
        newSettings.c_cc[VMIN] = 0;
        newSettings.c_cc[VTIME] = 1; ///for a timeout of 0.1s
        tcsetattr(STDIN_FILENO, TCSANOW, &newSettings); ///deactivates reads up to ENTER and automated output
        char c;
        read(STDIN_FILENO, &c, 1); ///reads a singular character

        int32_t type = 0; ///0 -> ESC, 1 -> UP, 2 -> DOWN

        if (c == 27) {
            char seq[2];
            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);

            if (seq[0] == '[') {
                type = (seq[1] == 'A' ? 1 : seq[1] == 'B' ? 2 : -1);
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings); ///restores configuration

        switch (c) {
            case '\n':
            case '\r':
                return Key::ENTER;
            case 27:
                return !type ? Key::ESC : type == 1 ? Key::UP : Key::DOWN;
            case 'c':
            case 'C':
                return Key::C;
            case '1':
                return Key::NUM_1;
            case '2':
                return Key::NUM_2;
            case '3':
                return Key::NUM_3;
            default:
                return Key::OTHER;
        }
    }

    #else

    inline Key readChar() {
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        INPUT_RECORD input;
        DWORD events;

        while (true) {
            ReadConsoleInput(hInput, &input, 1, &events);

            if (input.EventType != KEY_EVENT) {
                continue;
            }

            if (!input.Event.KeyEvent.bKeyDown) {
                continue;
            }

            WORD key = input.Event.KeyEvent.wVirtualKeyCode;

            switch (key) {
                case VK_UP:
                    return Key::UP;
                case VK_DOWN:
                    return Key::DOWN;
                case VK_RETURN:
                    return Key::ENTER;
                case VK_ESCAPE:
                    return Key::ESC;
                case '1':
                    return Key::NUM_1;
                case '2':
                    return Key::NUM_2;
                case '3':
                    return Key::NUM_3;
                case 'C':
                    return Key::C;
                default:
                    return Key::OTHER;
            }
        }
    }

    #endif

    inline Key readKey() {
        return readChar();
    }
};