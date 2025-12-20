#include <cstdint>

class Cpu {
public:
  uint8_t Memory[4096];
  uint8_t Registers[16];
  uint16_t IndexRegister, ProgramCounter, StackPointer;
  uint8_t OpStack[16];
  uint8_t DelayTimer;
  uint8_t OperationCode;
};
