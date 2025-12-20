#include "Cpu.hpp"

Cpu::Cpu() : Memory(4096), Registers(16), Opstack(16), ProgramCounter(0x200) {};

uint16_t Cpu::RandomNumGen() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint8_t> distr(0, 255);
  uint8_t randomByte = distr(gen);
  return randomByte;
}
