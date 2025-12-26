#ifndef RULES_MANAGER_H
#define RULES_MANAGER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char *AppName;
  char *layout;
} AppRule;

typedef struct {
  AppRule *rules;
  size_t rule_count;
} RulesConfig;

char *read_file_to_string(const char *path);
RulesConfig *rules_load_from_file(const char *path);
void rules_free(RulesConfig *config);

#ifdef __cplusplus
}
#endif

#endif
