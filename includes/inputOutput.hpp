#pragma once
#include <cstdint>
#include <notcurses/notcurses.h>
#include <vector>

class Platform {
public:
  Platform();

  ~Platform();

  void Update(const std::vector<std::vector<uint32_t>> &display);

  bool ProcessInput(std::vector<uint8_t> &keys);

private:
  struct notcurses *nc;
  struct ncplane *game_plane;
  struct ncplane *border_plane;
};
