#include "Cpu.hpp"
#include "inputOutput.hpp"
#include <chrono>
#include <thread>

int main(int argc, char **argv) {
  int cycleDelay = 2;
  Platform platform("CHIP-8 Emulator");
  Cpu chip8;
  chip8.LoadCharacters();
  chip8.LoadRom("./roms/Pong [Paul Vervalin, 1990].ch8");

  auto lastCycleTime = std::chrono::high_resolution_clock::now();
  bool quit = false;

  while (!quit) {
    // quit = platform.ProcessInput(chip8.Keypad);
    auto currentTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(
                   currentTime - lastCycleTime)
                   .count();
    if (dt >= cycleDelay) {
      lastCycleTime = currentTime;

      chip8.Cycle();

      platform.Update(chip8.Display);
    }
    static float timerAccumulator = 0;
    timerAccumulator += dt;
    if (timerAccumulator >= 16.67f) {
      if (chip8.DelayTimer > 0)
        chip8.DelayTimer--;
      timerAccumulator = 0;
    }

    std::this_thread::yield();
  }
}
