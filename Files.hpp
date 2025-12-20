#pragma once
#include "Cpu.hpp"
#include <cstdint>
#include <string>

class FileManagement {
public:
  FileManagement();
  const uint16_t START_ADDRESS;
  void LoadRom(std::string file, Cpu &c);
};
