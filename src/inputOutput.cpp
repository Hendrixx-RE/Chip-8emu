#include "../includes/inputOutput.hpp"

Platform::Platform() {
  struct notcurses_options nopts = {.flags = 0};
  nc = notcurses_init(&nopts, NULL);
  if (!nc)
    exit(1);
  struct ncplane *stdp = notcurses_stdplane(nc);
  unsigned term_rows, term_cols;
  ncplane_dim_yx(stdp, &term_rows, &term_cols);
  int game_rows = 16;
  int game_cols = 64;
  int start_y = (term_rows - game_rows) / 2;
  int start_x = (term_cols - game_cols) / 2;
  struct ncplane_options bopts = {
      .y = start_y - 1,
      .x = start_x - 1,
      .rows = static_cast<uint32_t>(game_rows + 2),
      .cols = static_cast<uint32_t>(game_cols + 2),
  };
  border_plane = ncplane_create(stdp, &bopts);
  uint64_t channels = 0;
  ncchannels_set_fg_rgb8(&channels, 152, 151, 26);
  ncplane_set_base(border_plane, " ", 0, channels);
  ncplane_perimeter_double(border_plane, 0, channels, 0);
  struct ncplane_options gopts = {.y = start_y,
                                  .x = start_x,
                                  .rows = static_cast<uint32_t>(game_rows),
                                  .cols = static_cast<uint32_t>(game_cols)};
  game_plane = ncplane_create(stdp, &gopts);
}

Platform::~Platform() { notcurses_stop(nc); }

void Platform::Update(const std::vector<std::vector<uint32_t>> &display) {
  static std::vector<uint32_t> flat_buffer(64 * 32);
  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 64; ++x) {
      flat_buffer[y * 64 + x] = (display[y][x] != 0) ? 0xFF1A9798 : 0x00000000;
    }
  }
  struct ncvisual *ncv = ncvisual_from_rgba(flat_buffer.data(), 32, 64 * 4, 64);
  if (ncv) {
    struct ncvisual_options vopts = {.n = game_plane,
                                     .blitter = NCBLIT_2x1,
                                     .flags = NCVISUAL_OPTION_NODEGRADE};
    ncvisual_blit(nc, ncv, &vopts);
    ncvisual_destroy(ncv);
  }
  notcurses_render(nc);
}

bool Platform::ProcessInput(std::vector<uint8_t> &keys) {
  ncinput ni;
  while (true) {
    uint32_t key = notcurses_get_nblock(nc, &ni);
    if (key == (uint32_t)-1 || key == 0) {
      break;
    }
    if (ni.id == 'P' || ni.id == 'p' || ni.id == NCKEY_ESC) {
      return true;
    }
    if (ni.evtype == 1) {
      switch (ni.id) {
      case '1':
        keys[0x1] = 1;
        break;
      case '2':
        keys[0x2] = 1;
        break;
      case '3':
        keys[0x3] = 1;
        break;
      case '4':
        keys[0xC] = 1;
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
      case 'r':
        keys[0xD] = 1;
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
      case 'f':
        keys[0xE] = 1;
        break;
      case 'z':
        keys[0xA] = 1;
        break;
      case 'x':
        keys[0x0] = 1;
        break;
      case 'c':
        keys[0xB] = 1;
        break;
      case 'v':
        keys[0xF] = 1;
        break;
      }
    } else if (ni.evtype == 3) {
      switch (ni.id) {
      case '1':
        keys[0x1] = 0;
        break;
      case '2':
        keys[0x2] = 0;
        break;
      case '3':
        keys[0x3] = 0;
        break;
      case '4':
        keys[0xC] = 0;
        break;
      case 'q':
        keys[0x4] = 0;
        break;
      case 'w':
        keys[0x5] = 0;
        break;
      case 'e':
        keys[0x6] = 0;
        break;
      case 'r':
        keys[0xD] = 0;
        break;
      case 'a':
        keys[0x7] = 0;
        break;
      case 's':
        keys[0x8] = 0;
        break;
      case 'd':
        keys[0x9] = 0;
        break;
      case 'f':
        keys[0xE] = 0;
        break;
      case 'z':
        keys[0xA] = 0;
        break;
      case 'x':
        keys[0x0] = 0;
        break;
      case 'c':
        keys[0xB] = 0;
        break;
      case 'v':
        keys[0xF] = 0;
        break;
      }
    }
  }
  return false;
}
