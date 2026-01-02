#include "Cpu.hpp"
#include "Constants.hpp"
#include <iostream>
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
      Display(HEIGHT, std::vector<uint32_t>(WIDTH)),
      ProgramCounter(START_ADDRESS), StackPointer(0), OperationCode(0) {};

void Cpu::Op_00E0() { Display.clear(); }

void Cpu::Op_DXYN() {
  uint8_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint8_t index2 = (OperationCode & 0x00F0) >> 4u;
  uint8_t height = OperationCode & 0x000F; // all sprites are a byte wide
  uint8_t Posx =
      Register[index1] % WIDTH; // these two are the starting to the spirte
  uint8_t Posy = Register[index2] % HEIGHT;
  Register[0xF] = 0;
  for (unsigned int row = 0; row < height; ++row) {
    uint8_t spriteByte = Memory[IndexCounter + row];
    for (unsigned int col = 0; col < 8; ++col) {
      uint8_t spritePixel = spriteByte & (0x80 >> col);
      uint32_t &screenPixel = Display[Posy + row][Posx + col];
      if (spritePixel) {
        if (screenPixel == 0xFFFFFFFF) {
          Register[0xF] = 1;
        }
        screenPixel ^= 0xFFFFFFFF;
      }
    }
  }
}

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

void Cpu::Op_1NNN() {
  uint16_t address = OperationCode & 0x0FFF;
  ProgramCounter = address;
}

void Cpu::Op_2NNN() {
  uint16_t address = OperationCode & 0x0FFF;
  Opstack[StackPointer] = ProgramCounter;
  ++StackPointer;
  ProgramCounter = address;
}

void Cpu::Op_3XKK() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t number = OperationCode & 0x00FF;
  if (Register[index] == number) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_4XKK() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t number = OperationCode & 0x00FF;
  if (Register[index] != number) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_5XY0() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  if (Register[index1] == Register[index2]) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_6XKK() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t value = OperationCode & 0x00FF;
  Register[index] = value;
}

void Cpu::Op_7XKK() {
  uint16_t index = (OperationCode & 0x0F00) >> 8u;
  uint16_t value = OperationCode & 0x00FF;
  Register[index] += value;
}

void Cpu::Op_8XY0() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] = Register[index2];
}

void Cpu::Op_8XY1() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] |= Register[index2];
}

void Cpu::Op_8XY2() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] &= Register[index2];
}

void Cpu::Op_8XY3() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] ^= Register[index2];
}

void Cpu::Op_8XY4() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  uint16_t sum = Register[index1] + Register[index2];
  Register[index1] = sum;
  Register[0xF] = (1 & (sum >> 8u));
}

void Cpu::Op_8XY5() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index1] > Register[index2] ? Register[0xF] = 1 : Register[0xF] = 0;
  Register[index1] -= Register[index2];
}

void Cpu::Op_8XY6() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  Register[0xF] = Register[index1] & 1u;
  Register[index1] >>= 1;
}

void Cpu::Op_8XY7() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  Register[index2] > Register[index1] ? Register[0xF] = 1 : Register[0xF] = 0;
  Register[index1] = Register[index2] - Register[index1];
}

void Cpu::Op_8XYE() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  Register[0xF] = Register[index1] & 1u;
  Register[index1] <<= 1;
}

void Cpu::Op_9XY0() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint16_t index2 = (OperationCode & 0x00F0) >> 4u;
  if (Register[index1] != Register[index2]) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_ANNN() {
  uint16_t location = (OperationCode & 0x0FFF) >> 8u;
  IndexCounter = location;
}

void Cpu::Op_BNNN() {
  uint16_t location = (OperationCode & 0x0FFF) >> 8u;
  ProgramCounter = Register[0] + location;
}

void Cpu::Op_CXKK() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint8_t val = OperationCode & 0x00FF;
  uint8_t ranbyte = Cpu::RandomNumberGenerator();
  Register[index1] = ranbyte & val;
}

void Cpu::Op_EX9E() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint8_t val = Register[index1];
  if (Keypad[val]) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_EXA1() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  uint8_t val = Register[index1];
  if (!Keypad[val]) {
    ProgramCounter += 2;
  }
}

void Cpu::Op_FX07() {
  uint16_t index1 = (OperationCode & 0x0F00) >> 8u;
  Register[index1] = DelayTimer;
}

void Cpu::Op_FX0A() {
  uint8_t index1 = (OperationCode & 0x0F00u) >> 8u;

  if (Keypad[0]) {
    Register[index1] = 0;
  } else if (Keypad[1]) {
    Register[index1] = 1;
  } else if (Keypad[2]) {
    Register[index1] = 2;
  } else if (Keypad[3]) {
    Register[index1] = 3;
  } else if (Keypad[4]) {
    Register[index1] = 4;
  } else if (Keypad[5]) {
    Register[index1] = 5;
  } else if (Keypad[6]) {
    Register[index1] = 6;
  } else if (Keypad[7]) {
    Register[index1] = 7;
  } else if (Keypad[8]) {
    Register[index1] = 8;
  } else if (Keypad[9]) {
    Register[index1] = 9;
  } else if (Keypad[10]) {
    Register[index1] = 10;
  } else if (Keypad[11]) {
    Register[index1] = 11;
  } else if (Keypad[12]) {
    Register[index1] = 12;
  } else if (Keypad[13]) {
    Register[index1] = 13;
  } else if (Keypad[14]) {
    Register[index1] = 14;
  } else if (Keypad[15]) {
    Register[index1] = 15;
  } else {
    ProgramCounter -= 2;
  }
}

void Cpu::Op_FX15() {
  uint8_t index1 = (OperationCode & 0x0F00) >> 8u;
  DelayTimer = Register[index1];
}

void Cpu::Op_FX1E() {
  uint8_t index1 = (OperationCode & 0x0F00) >> 8u;
  IndexCounter += Register[index1];
}

void Cpu::Op_FX33() {
  uint8_t num = Register[(OperationCode & 0x0F00) >> 8u];
  Memory[IndexCounter] = (num - (num % 100)) / 100;
  Memory[IndexCounter + 1] = (num % 100 - (num % 10)) / 10;
  Memory[IndexCounter + 2] = num % 10;
}

void Cpu::Op_FX55() {
  uint8_t index1 = (OperationCode & 0x0F00) >> 8u;
  for (uint16_t i = IndexCounter; i <= IndexCounter + index1; ++i) {
    Memory[i] = Register[i - IndexCounter];
  }
}

void Cpu::Op_FX65() {
  uint8_t index1 = (OperationCode & 0x0F00) >> 8u;
  for (uint16_t i = IndexCounter; i <= IndexCounter + index1; ++i) {
    Register[i - IndexCounter] = Memory[i];
  }
}

void Cpu::Op_NULL() { std::cout << "invalid Opcode"; };

void Cpu::Cycle() {
  // Fetch
  uint16_t Temporary = Memory[ProgramCounter];
  OperationCode = (Temporary << 8u) | Memory[ProgramCounter + 1];

  // Increment the PC before we execute anything
  ProgramCounter += 2;

  // Decode and Execute
  Op Operation =
      OperationArray[(OperationCode >> 12) & 0xF][(OperationCode >> 8) & 0xF]
                    [(OperationCode >> 4) & 0xF][OperationCode & 0xF];

  (this->*Operation)();

  // Decrement the delay timer if it's been set
  if (DelayTimer > 0) {
    --DelayTimer;
  }
}
