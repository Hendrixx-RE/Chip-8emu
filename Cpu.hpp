#pragma once
#include <cstdint>
#include <vector>

class Cpu {
public:
  Cpu();
  std::vector<uint8_t> Memory, Register, Opstack, Keypad,
      CharacterSet; // 4096,16,16
  uint16_t IndexRegister, ProgramCounter, OperationCode;
  uint8_t DelayTimer, StackPointer;
  uint8_t RandomNumberGenerator();

  // Loads the 16 Characters
  void LoadCharacters();

  // Returns from a subroutine
  void Op_00EE();

  // Jump to loacation nnn
  void Op_1nnn();

  // Call the subroutine at nnn
  void Op_2nnn();

  // Skip the next instruction if Register[x]=kk
  // This uses the Opcodes 3xkk form to compare the value in IndexRegister[x] to
  // number kk
  void Op_3xkk();

  // Skip the next instruction if Register[x]!=kk
  void Op_4xkk();

  // skips the next instruction if Register[x]==Register[y]
  void Op_5xy0();

  // sets Register[x] to value kk
  void Op_6xkk();

  // adds kk to Register[x]
  void Op_7xkk();

  // sets Register[x]=Register[y]
  void Op_8xy0();

  // sets
  // Register[x]=Register[x]|Register[y],Register[x]&Register[y],Register[x]^Register[y]
  void Op_8xy1();
  void Op_8xy2();
  void Op_8xy3();

  // Register[x]+=Register[y] but only the last 8 bits if it overflows,its
  // stored in the flag
  void Op_8xy4();

  // Register[x]-=Register[y] but if Register[x]>Register[y] set flag to 1 else
  // false
  void Op_8xy5();

  // Store the last bit of Register[x] in Register[F] and then right shift
  // Register[x] by 1
  void Op_8xy6();

  // Register[x]=Register[y]-Register[x] but if Register[y]>Register[x] set flag
  // to 1 else false
  void Op_8xy7();

  // Store the last bit of Register[x] in Register[F] and then left shift
  // Register[x] by 1
  void Op_8xyE();

  // Skip next instruction if Register[x]!=Register[y]
  void Op_9xy0();

  // Set Index to a given address
  void Op_Annn();

  // Jump to location nnn after Register[0]
  void Op_Bnnn();

  // Set Register[x]=RandomNumber&kk
  void Op_Cxkk();
};
