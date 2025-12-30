#include "layout_switcher.h"
#include "agent_keyboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *appName;
  char *layout;
} RuleEntry;

static RuleEntry *ruleEntries = NULL;
static size_t ruleCount = 0;
static char *currentLayout = NULL;

static const char *get_system_layout(void) {
  const char *id = get_active_input_source_id();
  return id;
}

void layout_switcher_init(RulesConfig *cfg) {
  if (!cfg || cfg->rule_count == 0)
    return;

  ruleCount = cfg->rule_count;
  ruleEntries = (RuleEntry *)calloc(ruleCount, sizeof(RuleEntry));

  for (size_t i = 0; i < ruleCount; i++) {
    ruleEntries[i].appName = strdup(cfg->rules[i].app_name);
    ruleEntries[i].layout = strdup(cfg->rules[i].layout);
  }

  const char *active = get_active_input_source_id();
  currentLayout = strdup(active);
  free((void *)active);
}

void layout_switcher_on_active_app_changed(const char *appName) {
  if (!appName || !ruleEntries)
    return;

  const char *systemLayout = get_system_layout();

  for (size_t i = 0; i < ruleCount; i++) {
    if (strcmp(appName, ruleEntries[i].appName) == 0) {

      if (strcmp(systemLayout, ruleEntries[i].layout) != 0) {
        if (switch_input_source(ruleEntries[i].layout)) {
          printf("[LayoutSwitcher] %s → %s\n", appName, ruleEntries[i].layout);
        }
      }
      break;
    }
  }

  free((void *)systemLayout);
}

void layout_switcher_free(void) {
  if (ruleEntries) {
    for (size_t i = 0; i < ruleCount; i++) {
      free(ruleEntries[i].appName);
      free(ruleEntries[i].layout);
    }
    free(ruleEntries);
    ruleEntries = NULL;
  }
  if (currentLayout) {
    free(currentLayout);
    currentLayout = NULL;
  }
}

void layout_switcher_reload_rules(RulesConfig *cfg) {
  layout_switcher_free();
  layout_switcher_init(cfg);
}
