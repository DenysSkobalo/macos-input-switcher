#include "agent_active_app.h"
#include "agent_keyboard.h"
#include "layout_switcher.h"
#include "rules_manager.h"

#include <stdio.h>

int main() {
  const char *path_json = "./rules/rules.json";

  RulesConfig *cfg = rules_load_from_file(path_json);
  if (!cfg) {
    fprintf(stderr, "Failed to read JSON file\n");
    return 1;
  }

  print_parsed_rules(cfg);
  listInputSources();

  layout_switcher_init(cfg);

  startApplicationActivityAgent();

  layout_switcher_free();
  return 0;
}
