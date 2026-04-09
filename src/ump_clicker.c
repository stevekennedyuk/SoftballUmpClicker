#include "ump_clicker.h"
#include "ump_clicker_windows.h"
#include <pebble.h>

/* Initialise inning arrays to -1 (unrecorded) */
static void init_inning_arrays(void) {
  for (int i = 0; i < MAX_INNINGS_BASEBALL + MAX_EXTRA_INNINGS; i++) {
    ump_AwayInning[i] = -1;
    ump_HomeInning[i] = -1;
  }
}

static void init(void) {
  init_inning_arrays();
  show_setup_window();
}

static void deinit(void) {
  /* windows clean up themselves on unload */
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
