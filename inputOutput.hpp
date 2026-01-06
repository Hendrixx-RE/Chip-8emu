#pragma once
#include <cstdint>
#include <notcurses/notcurses.h>
#include <vector>

class Platform {
public:
  Platform(const char *title) {
    struct notcurses_options nopts = {.flags = 0}; // Seperate screen enabled
    nc = notcurses_init(&nopts, NULL);
    if (!nc)
      exit(1);

    struct ncplane *stdp = notcurses_stdplane(nc);
    unsigned term_rows, term_cols;
    ncplane_dim_yx(stdp, &term_rows, &term_cols);
    int game_rows = 16;
    int game_cols = 32;
    int start_y = (term_rows - game_rows) / 2;
    int start_x = (term_cols - game_cols) / 2;
    struct ncplane_options gopts = {.y = start_y,
                                    .x = start_x,
                                    .rows = uint32_t(game_rows),
                                    .cols = uint32_t(game_cols)};
    game_plane = ncplane_create(stdp, &gopts);
  }

  ~Platform() { notcurses_stop(nc); }

  void Update(const std::vector<std::vector<uint32_t>> &display) {
    // Flatten the 2D vector into 1D contiguous memory for the C-API
    static std::vector<uint32_t> flat_buffer(64 * 32);
    for (int y = 0; y < 32; ++y) {
      for (int x = 0; x < 64; ++x) {
        // Map internal "on" to Green, "off" to Transparent
        flat_buffer[y * 64 + x] =
            (display[y][x] != 0) ? 0xFF00FF00 : 0x00000000;
      }
    }

    struct ncvisual *ncv =
        ncvisual_from_rgba(flat_buffer.data(), 32, 64 * 4, 64);
    if (ncv) {
      struct ncvisual_options vopts = {.n = game_plane,
                                       .blitter = NCBLIT_2x2,
                                       .flags = NCVISUAL_OPTION_NODEGRADE};
      ncvisual_blit(nc, ncv, &vopts);
      ncvisual_destroy(ncv);
    }
    notcurses_render(nc);
  }

  bool ProcessInput(std::vector<uint8_t> &keys) {
    ncinput ni;
    uint32_t key = notcurses_get_nblock(nc, &ni);

    if (key == 'P' || key == NCKEY_ESC)
      return true; // Quit signal

    // Simulation of SDL KeyUp: Clear the keypad if no key is detected
    // Note: Terminals repeat keys while held, so this keeps the key 'active'
    std::fill(keys.begin(), keys.end(), 0);

    if (key != (uint32_t)-1 && key != 0) {
      switch (key) {
      case 'x':
        keys[0x0] = 1;
        break;
      case '1':
        keys[0x1] = 1;
        break;
      case '2':
        keys[0x2] = 1;
        break;
      case '3':
        keys[0x3] = 1;
        break;
      case 'q':
        keys[0x4] = 1;
        break;
      case 'w':
        keys[0x5] = 1;
        break;
      case 'e':
        keys[0x6] = 1;
        break;
      case 'a':
        keys[0x7] = 1;
        break;
      case 's':
        keys[0x8] = 1;
        break;
      case 'd':
        keys[0x9] = 1;
        break;
      case 'z':
        keys[0xA] = 1;
        break;
      case 'c':
        keys[0xB] = 1;
        break;
      case '4':
        keys[0xC] = 1;
        break;
      case 'r':
        keys[0xD] = 1;
        break;
      case 'f':
        keys[0xE] = 1;
        break;
      case 'v':
        keys[0xF] = 1;
        break;
      }
    }
    return false;
  }

private:
  struct notcurses *nc;
  struct ncplane *game_plane;
};
