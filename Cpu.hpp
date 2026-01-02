#pragma once
#include <array>
#include <cstdint>
#include <vector>
class Cpu {
public:
  Cpu();
  std::vector<uint8_t> Memory, Register, Opstack, Keypad,
      CharacterSet;                           // 4096,16,16
  std::vector<std::vector<uint32_t>> Display; // 64x32
  uint16_t IndexCounter, ProgramCounter, OperationCode;
  uint8_t DelayTimer, StackPointer;
  using Op = void (Cpu::*)(void);
  using opArray =
      std::array<std::array<std::array<std::array<Op, 16>, 16>, 16>, 16>;
  static constexpr opArray makeOpTable();
  // Clears the screen
  void Op_00E0();

  // draws the sprite stored in Register[x] and Register[y] to screen
  void Op_DXYN();

  // self explanatory
  uint8_t RandomNumberGenerator();

  // Loads the 16 Characters
  void LoadCharacters();

  // Returns from a subroutine
  void Op_00EE();

  // Jump to loacation nnn
  void Op_1NNN();

  // Call the subroutine at nnn
  void Op_2NNN();

  // Skip the next instruction if Register[x]=kk
  // This uses the Opcodes 3xkk form to compare the value in Register[x] to
  // number kk
  void Op_3XKK();

  // Skip the next instruction if Register[x]!=kk
  void Op_4XKK();

  // skips the next instruction if Register[x]==Register[y]
  void Op_5XY0();

  // sets Register[x] to value kk
  void Op_6XKK();

  // adds kk to Register[x]
  void Op_7XKK();

  // sets Register[x]=Register[y]
  void Op_8XY0();

  // sets
  // Register[x]=Register[x]|Register[y],Register[x]&Register[y],Register[x]^Register[y]
  void Op_8XY1();
  void Op_8XY2();
  void Op_8XY3();

  // Register[x]+=Register[y] but only the last 8 bits if it overflows,its
  // stored in the flag
  void Op_8XY4();

  // Register[x]-=Register[y] but if Register[x]>Register[y] set flag to 1 else
  // false
  void Op_8XY5();

  // Store the last bit of Register[x] in Register[F] and then right shift
  // Register[x] by 1
  void Op_8XY6();

  // Register[x]=Register[y]-Register[x] but if Register[y]>Register[x] set flag
  // to 1 else false
  void Op_8XY7();

  // Store the last bit of Register[x] in Register[F] and then left shift
  // Register[x] by 1
  void Op_8XYE();

  // Skip next instruction if Register[x]!=Register[y]
  void Op_9XY0();

  // Set Index to a given address
  void Op_ANNN();

  // Jump to location nnn after Register[0]
  void Op_BNNN();

  // Set Register[x]=RandomNumber&kk
  void Op_CXKK();

  // skip next instruction if key with value Register[x] is pressed
  void Op_EX9E();

  // skip next instruction if key with value Register[x] is not pressed
  void Op_EXA1();

  // Set Register[x]=DelayTimer value
  void Op_FX07();

  // wait for a key press and set Register[x]=key
  void Op_FX0A();

  // Set DelayTimer = Register[x]
  void Op_FX15();

  // set index+=Register[x]
  void Op_FX1E();

  // store bcd rep of Register[x] in IndexCounter,IndexCounter+1 and
  // IndexCounter+2
  void Op_FX33();

  // store register[0] to register[x] in Memory starting at IndexCounter
  void Op_FX55();

  // store register[0] to register[x] from Memory starting at IndexCounter
  void Op_FX65();

  void Op_NULL();

  void Cycle();
};

// Compile time oparray generator
inline constexpr Cpu::opArray makeOpTable() {
  Cpu::opArray t{}; // value-initialized

  // 1. Default everything to illegal
  for (auto &dim1 : t) {
    for (auto &dim2 : dim1) {
      for (auto &dim3 : dim2) {
        dim3.fill(&Cpu::Op_NULL); // Fills the final 16-element array safely
      }
    }
  }

  // 2. 00E0, 00EE
  t[0x0][0x0][0xE][0x0] = &Cpu::Op_00EE;
  t[0x0][0x0][0xE][0xE] = &Cpu::Op_00EE;

  // 3. 1NNN, 2NNN, ANNN, BNNN
  for (int b = 0; b < 16; ++b)
    for (int c = 0; c < 16; ++c)
      for (int d = 0; d < 16; ++d) {
        t[0x1][b][c][d] = &Cpu::Op_1NNN;
        t[0x2][b][c][d] = &Cpu::Op_2NNN;
        t[0xA][b][c][d] = &Cpu::Op_ANNN;
        t[0xB][b][c][d] = &Cpu::Op_BNNN;
      }

  // 4. 3XKK, 4XKK, 6XKK, 7XKK, CXKK
  for (int x = 0; x < 16; ++x)
    for (int c = 0; c < 16; ++c)
      for (int d = 0; d < 16; ++d) {
        t[0x3][x][c][d] = &Cpu::Op_3XKK;
        t[0x4][x][c][d] = &Cpu::Op_4XKK;
        t[0x6][x][c][d] = &Cpu::Op_6XKK;
        t[0x7][x][c][d] = &Cpu::Op_7XKK;
        t[0xC][x][c][d] = &Cpu::Op_CXKK;
      }

  // 5. 5XY0, 9XY0
  for (int x = 0; x < 16; ++x)
    for (int y = 0; y < 16; ++y) {
      t[0x5][x][y][0x0] = &Cpu::Op_5XY0;
      t[0x9][x][y][0x0] = &Cpu::Op_9XY0;
    }

  // 6. 8XY*
  for (int x = 0; x < 16; ++x)
    for (int y = 0; y < 16; ++y) {
      t[0x8][x][y][0x0] = &Cpu::Op_8XY0;
      t[0x8][x][y][0x1] = &Cpu::Op_8XY1;
      t[0x8][x][y][0x2] = &Cpu::Op_8XY2;
      t[0x8][x][y][0x3] = &Cpu::Op_8XY3;
      t[0x8][x][y][0x4] = &Cpu::Op_8XY4;
      t[0x8][x][y][0x5] = &Cpu::Op_8XY5;
      t[0x8][x][y][0x6] = &Cpu::Op_8XY6;
      t[0x8][x][y][0x7] = &Cpu::Op_8XY7;
      t[0x8][x][y][0xE] = &Cpu::Op_8XYE;
    }

  // 7. DXYN
  for (int x = 0; x < 16; ++x)
    for (int y = 0; y < 16; ++y)
      for (int n = 0; n < 16; ++n)
        t[0xD][x][y][n] = &Cpu::Op_DXYN;

  // 8. EX9E, EXA1
  for (int x = 0; x < 16; ++x) {
    t[0xE][x][0x9][0xE] = &Cpu::Op_EX9E;
    t[0xE][x][0xA][0x1] = &Cpu::Op_EXA1;
  }

  // 9. FX**
  for (int x = 0; x < 16; ++x) {
    t[0xF][x][0x0][0x7] = &Cpu::Op_FX07;
    t[0xF][x][0x0][0xA] = &Cpu::Op_FX0A;
    t[0xF][x][0x1][0x5] = &Cpu::Op_FX15;
    t[0xF][x][0x1][0xE] = &Cpu::Op_FX1E;
    t[0xF][x][0x3][0x3] = &Cpu::Op_FX33;
    t[0xF][x][0x5][0x5] = &Cpu::Op_FX55;
    t[0xF][x][0x6][0x5] = &Cpu::Op_FX65;
  }

  t[0][0][0][0] = &Cpu::Op_NULL;
  return t;
}
constexpr Cpu::opArray OperationArray = makeOpTable();
