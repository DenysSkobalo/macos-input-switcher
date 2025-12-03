#include "macos_bridge.h"

#include "macos_bridge.h"
#include <stdio.h>

int main(void) {
  const char *title = get_active_window_title();
  printf("Active window: %s\n", title);

  return 0;
}
