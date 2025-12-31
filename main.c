#include "agent_active_app.h"
#include "agent_keyboard.h"
#include "layout_switcher.h"
#include "rules_manager.h"
#include <CoreFoundation/CoreFoundation.h>

#include <stdio.h>

int main() {
  const char *path_json = "./rules/rules.json";

  RulesConfig *cfg = rules_load_from_file(path_json);
  if (!cfg) {
    fprintf(stderr, "Failed to read JSON file\n");
    return 1;
  }

  agent_active_app_start(layout_switcher_on_active_app_changed);

  print_parsed_rules(cfg);
  listInputSources();

  layout_switcher_init(cfg);

  CFRunLoopRun();

  agent_active_app_stop();

  layout_switcher_free();
  return 0;
}
