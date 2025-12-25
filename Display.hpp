#pragma once
#include <cstdint>
#include <vector>

class Screen {
public:
  Screen();
  std::vector<std::vector<uint32_t>> Display; // 64x32

  // Clears the screen
  void Op_0E00();
};
