#ifndef RULES_MANAGER_H
#define RULES_MANAGER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RULES "rules"
#define APP "app"
#define LAYOUT "layout"

typedef struct {
  char *app_name;
  char *layout;
} AppRule;

typedef struct {
  AppRule *rules;
  size_t rule_count;
} RulesConfig;

RulesConfig *rules_load_from_file(const char *path);
void print_parsed_rules(RulesConfig *cfg);
#ifdef __cplusplus
}
#endif

#endif
