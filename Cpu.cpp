#include "Cpu.hpp"
#include "Constants.hpp"
#include <random>

Cpu::Cpu()
    : Memory(MEM_SIZE), Register(REG_SIZE), Opstack(REG_SIZE), Keypad(REG_SIZE),
      CharacterSet({
          0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
          0x20, 0x60, 0x20, 0x20, 0x70, // 1
          0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
          0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
          0x90, 0x90, 0xF0, 0x10, 0x10, // 4
          0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
          0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
          0xF0, 0x10, 0x20, 0x40, 0x40, // 7
          0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
          0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
          0xF0, 0x90, 0xF0, 0x90, 0x90, // A
          0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
          0xF0, 0x80, 0x80, 0x80, 0xF0, // C
          0xE0, 0x90, 0x90, 0x90, 0xE0, // D
          0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
          0xF0, 0x80, 0xF0, 0x80, 0x80  // F
      }),
      ProgramCounter(START_ADDRESS), StackPointer(0) {};

uint8_t Cpu::RandomNumberGenerator() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint8_t> distr(0, 255);
  uint8_t randomByte = distr(gen);
  return randomByte;
}

void Cpu::LoadCharacters() {
  uint16_t iterator = 0x50;
  for (auto it : CharacterSet) {
    Memory[iterator] = it;
    ++iterator;
  }
}

void Cpu::Op_00EE() {
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
  if (Register[index] == number) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_4xkk() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t number = OperationCode & 0x00FF;
  if (Register[index] != number) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_5xy0() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  if (Register[index1] == Register[index2]) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_6xkk() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t value = OperationCode & 0x00FF;
  Register[index] = value;
}

void Cpu::Op_7xkk() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t value = OperationCode & 0x00FF;
  Register[index] += value;
}

void Cpu::Op_8xy0() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] = Register[index2];
}

void Cpu::Op_8xy1() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] |= Register[index2];
}

void Cpu::Op_8xy2() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] &= Register[index2];
}

void Cpu::Op_8xy3() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] ^= Register[index2];
}

void Cpu::Op_8xy4() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  uint16_t sum = Register[index1] + Register[index2];
  Register[index1] = sum;
  Register[0xF] = (1 & (sum >> 8u));
}

void Cpu::Op_8xy5() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] > Register[index2] ? Register[0xF] = 1 : Register[0xF] = 0;
  Register[index1] -= Register[index2];
}

void Cpu::Op_8xy6() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  Register[0xF] = Register[index1] & 1u;
  Register[index1] >>= 1;
}

void Cpu::Op_8xy7() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index2] > Register[index1] ? Register[0xF] = 1 : Register[0xF] = 0;
  Register[index1] = Register[index2] - Register[index1];
}

void Cpu::Op_8xyE() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  Register[0xF] = Register[index1] & 1u;
  Register[index1] <<= 1;
}

void Cpu::Op_9xy0() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  if (Register[index1] != Register[index2]) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_Annn() {
  uint16_t location = (OperationCode & 0x0FFF) >> 8u;
  IndexRegister = location;
}

void Cpu::Op_Bnnn() {
  uint16_t location = (OperationCode & 0x0FFF) >> 8u;
  ProgramCounter = Register[0] + location;
}

void Cpu::Op_Cxkk() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint8_t val = OperationCode & 0x00FF;
  uint8_t ranbyte = Cpu::RandomNumberGenerator();
  Register[index1] = ranbyte & val;
}
