# CHIP-8 Emulator

A  CHIP-8 emulator written in **C++** with a terminal-based interface using **notcurses**.

![C++](https://img.shields.io/badge/C++-20-blue.svg)
![Interface](https://img.shields.io/badge/UI-notcurses-green.svg)
![License](https://img.shields.io/badge/license-MIT-blue)

---

### Prerequisites
You will need a C++ compiler (GCC or Clang) and the ncurses development libraries installed on your system.
```bash
sudo pacman -S notcurses
```

### Build
```bash
git clone https://github.com/Hendrixx-RE/Chip-8emu.git
cd Chip-8emu
cmake -B build
cmake --build build
cd build
```
### Running
```bash
./Chip8 {game delay in ms (2)} {game name as in /roms folder}

<img width="1920" height="1200" alt="image" src="https://github.com/user-attachments/assets/fd9393b3-1a62-4f65-a087-01953c7db3af" />
