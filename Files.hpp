#pragma once
#include "Cpu.hpp"
#include <string>

class FileManagement {
public:
  FileManagement();
  void LoadRom(std::string file, Cpu &c);
};
