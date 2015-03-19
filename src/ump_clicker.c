#include "ump_clicker_windows.h"
#include <pebble.h>






static void init(void) {
	show_ump_clicker_windows();
}

static void deinit(void) {
	hide_ump_clicker_windows();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

