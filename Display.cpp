#include "Display.hpp"

Screen::Screen() : Display(64, std::vector<uint32_t>(32)) {};

void Screen::Op_0E00() { Display.clear(); }
