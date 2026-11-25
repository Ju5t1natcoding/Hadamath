#include "utilities/clearScreen.hpp"
#include "utilities/keyboard.hpp"
#include "op_helpers/complexNumbers.hpp"
#include "op_helpers/matrix.hpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <variant>
#include <string_view> ///only "looks" at the string without memorising it

enum class Op {
    ///real
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    POW,
    FACT,
    SQRT,
    LOG, ///with 2 arguments
    LN,
    EXP,
    ABS,
    SIN,
    COS,
    TAN,

    ///bitwise
    AND,
    OR,
    XOR,
    SHL, ///<<
    SHR, ///>>
    NOT,

    ///quantum -> for an angle theta
    RX,
    RY,
    RZ,
    P, ///phase

    ///matrixes
    TENSOR,
    DAG,

    ///unary
    NEG,
    POS,
    COUNT
};

///quantum gates
enum class Gate {
    I, X, Y, Z, S, T, H, CX, CY, CZ, CCX, CCZ, SWAP
};

using Value = std::variant<uint64_t, double, complexNumber<double>, Matrix<complexNumber<double>>>;

enum class Kind {
    INF, PREF, POST, FUNC
};

enum Modes : uint8_t { REAL = 1, BITS = 2, QUANT = 4 };

struct OpInfo {
    const char* name;
    Kind kind;
    int32_t prec; ///precedency - only for INF/PREF/POST
    bool rightAssoc;
    int32_t arity; ///1, 2 (for example LOG has 2 arguments)
    uint8_t modes;
};

constexpr OpInfo OPS[] = {
    {"+", Kind::INF, 10, false, 2, REAL | QUANT},
    {"-", Kind::INF, 10, false, 2, REAL},
    {"*", Kind::INF, 20, false, 2, REAL},
    {"/", Kind::INF, 20, false, 2, REAL},
    {"%", Kind::INF, 20, false, 2, REAL},
    {"^", Kind::INF, 30, true, 2, REAL},
    {"!", Kind::POST, 40, false, 1, REAL},
    {"sqrt", Kind::FUNC, 0, false, 1, REAL},
    {"log", Kind::FUNC, 0, false, 2, REAL},
    {"ln", Kind::FUNC, 0, false, 1, REAL},
    {"exp", Kind::FUNC, 0, false, 1, REAL},
    {"abs", Kind::FUNC, 0, false, 1, REAL},
    {"sin", Kind::FUNC, 0, false, 1, REAL},
    {"cos", Kind::FUNC, 0, false, 1, REAL},
    {"tan", Kind::FUNC, 0, false, 1, REAL},
    {"&", Kind::INF, 30, false, 2, BITS},
    {"|", Kind::INF, 10, false, 2, BITS},
    {"^", Kind::INF, 20, false, 2, BITS},
    {"<<", Kind::INF, 40, false, 2, BITS},
    {">>", Kind::INF, 40, false, 2, BITS},
    {"~", Kind::PREF, 50, true, 1, BITS},
    {"RX", Kind::FUNC, 0, false, 1, QUANT},
    {"RY", Kind::FUNC, 0, false, 1, QUANT},
    {"RZ", Kind::FUNC, 0, false, 1, QUANT},
    {"P", Kind::FUNC, 0, false, 1, QUANT},
    {"tensor", Kind::FUNC, 0, false, 2, QUANT},
    {"'", Kind::POST, 40, false, 1, QUANT},
    {"-", Kind::PREF, 25, true, 1, REAL},
    {"+", Kind::PREF, 25, true, 1, REAL}
};

///assert to verify if the table is syncronised with the enum class or not
static_assert(std::size(OPS) == static_cast<size_t>(Op::COUNT), "OPS is not syncronized with Op");

inline const OpInfo* info(Op o) {
    return &OPS[static_cast<size_t>(o)];
}

struct Node {
    const OpInfo *op = nullptr;
    Value v;
    Node *l = nullptr, *r = nullptr;
};

struct TreeBuilder {
    Node *root = nullptr;
    std::vector<Node*> spine; ///right chain

    void addOperand(Node* x) {
        if (spine.empty()) {
            root = x;
        } else {
            spine.back()->r = x;
        }
    }

    void addInf(Node* x) {
        while (!spine.empty() && (spine.back()->op->prec > x->op->prec || (spine.back()->op->prec == x->op->prec && !x->op->rightAssoc))) {
            spine.pop_back();
        }

        if (spine.empty()) {
            x->l = root;
            root = x;
        } else {
            x->l = spine.back()->r;
            spine.back()->r = x;
        }

        spine.push_back(x);
    }

    void addPref(Node* x) {
        addOperand(x);
        spine.push_back(x);
    }

    void addPost(Node* x) {
        Node*& slot = spine.empty() ? root : spine.back()->r;
        x->l = slot;
        slot = x;
    }
};

void runMode(uint8_t m) {
    std::string s;
    ///placeholder
}

const std::array<std::string_view, 3> opts = {"1. real", "2. bits", "3. qubits"};

int main() {
    auto are_you_sure = []() -> void {
        int8_t selected = 0;

        while (true) {
            clearScreen();
            std::cout << "Do you want to exit the app?";

            for (int8_t i = 0; i < 2; ++i) {
                std::cout << (selected == i ? "  > " : "    ") << (!i ? "No\n" : "Yes\n");
            }

            std::cout.flush();
            Keyboard::Key answer = Keyboard::readKey();

            switch (answer) {
                case Keyboard::Key::UP:
                    selected = (selected + 1) % 2;
                    break;
                case Keyboard::Key::ENTER:
                    if (!selected) {
                         exit(0);
                    }

                    return;
                case Keyboard::Key::ESC:
                    return;
                default:
                     break;
            }
        }
    };

    int8_t selected = 0;

    while (true) {
        clearScreen();
        std::cout << "Welcome to Hadamath, an interpreter in 3 modes! Choose in whichever you would like to work:\n\n";

        for (int8_t i = 0; i < 3; ++i) {
            std::cout << (selected == i ? "  > " : "    ") << opts[i] << "\n";
        }

        std::cout << "\n\nNavigation guide:\n";
        ///placeholder on guide

        std::cout.flush();
        Keyboard::Key key = Keyboard::readKey();

        switch (key) {
            case Keyboard::Key::UP:
                selected = (selected + 2) % 3;
                break;
            case Keyboard::Key::DOWN:
                selected = (selected + 1) % 3;
                break;
            case Keyboard::Key::ENTER:
                runMode(1 << selected);
                break;
            case Keyboard::Key::NUM_1:
                runMode(REAL);
                break;
            case Keyboard::Key::NUM_2:
                runMode(BITS);
                break;
            case Keyboard::Key::NUM_3:
                runMode(QUANT);
                break;
            case Keyboard::Key::C:
                clearScreen();
                break;
            case Keyboard::Key::ESC:
                are_you_sure();
                break;
            default:
                break;
        }
    }
}