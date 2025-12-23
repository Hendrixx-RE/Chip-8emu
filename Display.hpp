#pragma once
#include <cstdint>
#include <vector>

class UI {
public:
  UI();
  std::vector<std::vector<uint8_t>> Display; // 64x32
  std::vector<uint8_t> KeyPad;               // 16;
  std::vector<uint8_t> CharacterSet;         // 80

  // Clears the screen
  void Op_0E00();
};
