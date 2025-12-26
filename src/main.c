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

  listInputSources();

  printf("Number of rules: %zu\n", cfg->rule_count);
  for (size_t i = 0; i < cfg->rule_count; i++) {
    printf("Rule %zu:\n", i + 1);
    printf("  AppName: %s\n", cfg->rules[i].AppName);
    printf("  Layout : %s\n", cfg->rules[i].layout);
  }

  layout_switcher_init(cfg);
  rules_free(cfg);

  startApplicationActivityAgent();

  layout_switcher_free();
  return 0;
}
