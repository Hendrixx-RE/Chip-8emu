#pragma once
#include <cstdint>
#include <random>
#include <vector>

class Cpu {
public:
  Cpu();
  std::vector<uint8_t> Memory, Registers, Opstack; // 4096,16,16
  uint16_t IndexRegister, ProgramCounter, StackPointer;
  uint8_t DelayTimer, OperationCode;
  uint16_t RandomNumGen();
};
