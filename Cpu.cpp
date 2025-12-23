#include "Cpu.hpp"
#include "Display.hpp"
#include <random>

Cpu::Cpu()
    : Memory(4096), Registers(16), Opstack(16), ProgramCounter(0x200),
      StackPointer(0) {};

uint16_t Cpu::RandomNumberGenerator() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint8_t> distr(0, 255);
  uint8_t randomByte = distr(gen);
  return randomByte;
}

void Cpu::LoadCharacters(UI *Displaypointer) {
  int iterator = 0x50;
  for (auto it : Displaypointer->CharacterSet) {
    Memory[iterator] = it;
  }
}

void Cpu::Op_00EE() {
  Opstack[StackPointer] = 0;
  --StackPointer;
  ProgramCounter = Opstack[StackPointer];
}

void Cpu::Op_1nnn() {
  uint16_t address = OperationCode & 0x0FFF;
  ProgramCounter = address;
}

void Cpu::Op_2nnn() {
  uint16_t address = OperationCode & 0x0FFF;
  Opstack[StackPointer] = ProgramCounter;
  ++StackPointer;
  ProgramCounter = address;
}

void Cpu::Op_3xkk() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t number = OperationCode & 0x00FF;
  if (Registers[index] == number) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_4xkk() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t number = OperationCode & 0x00FF;
  if (Registers[index] != number) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_5xy0() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  if (Registers[index1] == Registers[index2]) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_6xkk() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t value = OperationCode & 0x00FF;
  Registers[index] = value;
}

void Cpu::Op_7xkk() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t value = OperationCode & 0x00FF;
  Registers[index] += value;
}

void Cpu::Op_8xy0() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Registers[index1] = Registers[index2];
}

void Cpu::Op_8xy1() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Registers[index1] |= Registers[index2];
}

void Cpu::Op_8xy2() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Registers[index1] &= Registers[index2];
}

void Cpu::Op_8xy3() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Registers[index1] ^= Registers[index2];
}

void Cpu::Op_8xy4() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  uint16_t sum = Registers[index1] + Registers[index2];
}
