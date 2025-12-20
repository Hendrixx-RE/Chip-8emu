#include "Files.hpp"
#include <fstream>
#include <vector>

FileManagement::FileManagement() : START_ADDRESS(0x200) {};

void FileManagement::LoadRom(std::string file, Cpu &c) {
  std::ifstream filestream(file, std::ios::binary | std::ios::ate);
  if (filestream.is_open()) {
    std::streampos size = filestream.tellg();
    std::vector<uint8_t> Buffer(size);
    filestream.seekg(0);
    filestream.read(reinterpret_cast<char *>(Buffer.data()), size);
    filestream.close();
    uint16_t Iterator = START_ADDRESS;
    for (auto it : Buffer) {
      c.Memory[Iterator] = it;
    }
  }
}
