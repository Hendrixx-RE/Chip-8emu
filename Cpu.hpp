#pragma once
#include <cstdint>
#include <vector>

class UI;

class Cpu {
public:
  Cpu();
  std::vector<uint8_t> Memory, Registers, Opstack; // 4096,16,16
  uint16_t IndexRegister, ProgramCounter, StackPointer;
  uint8_t DelayTimer, OperationCode;
  uint16_t RandomNumberGenerator();

  // Loads the 16 Characters
  void LoadCharacters(UI *Displaypointer);

  // Returns from a subroutine
  void Op_00EE();

  // Jump to loacation nnn
  void Op_1nnn();

  // Call the subroutine at nnn
  void Op_2nnn();

  // Skip the next instruction if Registers[x]=kk
  // This uses the Opcodes 3xkk form to compare the value in IndexRegister[x] to
  // number kk
  void Op_3xkk();

  // Skip the next instruction if Registers[x]!=kk
  void Op_4xkk();

  // skips the next instruction if Registers[x]==Registers[y]
  void Op_5xy0();

  // sets Registers[x] to value kk
  void Op_6xkk();

  // adds kk to Registers[x]
  void Op_7xkk();

  // sets Registers[x]=Registers[y]
  void Op_8xy0();

  // sets
  // Registers[x]=Registers[x]|Registers[y],Registers[x]&Registers[y],Registers[x]^Registers[y]
  void Op_8xy1();
  void Op_8xy2();
  void Op_8xy3();

  // Registers[x]+=Registers[y] but only the last 8 bits if it overflows,its
  // stored in the flag
  void Op_8xy4();
};
