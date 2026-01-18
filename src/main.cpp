#include "../includes/Cpu.hpp"
#include "../includes/inputOutput.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char **argv) {
  int cycleDelay;
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << "<Delay> <ROM>\n";
    std::exit(EXIT_FAILURE);
  }
  cycleDelay = std::stoi(argv[1]);
  Platform platform;
  Cpu chip8;
  std::string romPath = std::string("../roms/") + argv[2];
  chip8.LoadRom(romPath);
  using clock = std::chrono::high_resolution_clock;
  using ms = std::chrono::duration<float, std::milli>;
  auto lastCycleTime = clock::now();
  while (true) {
    if (platform.ProcessInput(chip8.Keypad)) {
      break;
    }
    auto now = clock::now();
    float cycleDt = std::chrono::duration_cast<ms>(now - lastCycleTime).count();
    if (cycleDt >= cycleDelay) {
      chip8.Cycle();
      platform.Update(chip8.Display);
      lastCycleTime = now;
    }
    std::this_thread::yield();
  }
}
