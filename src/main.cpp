#include "../includes/Cpu.hpp"
#include "../includes/inputOutput.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char **argv) {
  int IPF;
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << "<Delay> <ROM>\n";
    std::exit(EXIT_FAILURE);
  }
  IPF = std::stoi(argv[1]);
  Platform platform;
  Cpu chip8;
  std::string romPath = std::string("../roms/") + argv[2];
  chip8.LoadRom(romPath);
  auto next_frame = std::chrono::steady_clock::now();
  const std::chrono::microseconds frame_duration(16667);
  while (true) {
    next_frame += frame_duration;
    if (platform.ProcessInput(chip8.Keypad)) {
      break;
    }
    if (chip8.DelayTimer > 0) {
      --chip8.DelayTimer;
    }
    for (int i = 0; i < IPF; ++i) {
      chip8.Cycle();
    }
    platform.Update(chip8.Display);
    std::this_thread::sleep_until(next_frame);
  }
}
