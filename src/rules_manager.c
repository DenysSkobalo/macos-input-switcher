#include "rules_manager.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *extract_next_quoted_key(const char **p) {
  if (!p || !*p)
    return NULL;
  const char *start = strchr(*p, '"');
  if (!start)
    return NULL;
  const char *end = strchr(start + 1, '"');
  if (!end)
    return NULL;

  size_t len = end - start - 1;
  char *key = (char *)malloc(len + 1);
  strncpy(key, start + 1, len);
  key[len] = '\0';

  *p = end + 1;
  return key;
}

static char *extract_next_quoted_value(const char **p) {
  if (!p || !*p)
    return NULL;
  const char *start = strchr(*p, '"');
  if (!start)
    return NULL;
  const char *end = strchr(start + 1, '"');
  if (!end)
    return NULL;

  size_t len = end - start - 1;
  char *val = (char *)malloc(len + 1);
  strncpy(val, start + 1, len);
  val[len] = '\0';

  *p = end + 1;
  return val;
}

char *read_file_to_string(const char *path) {
  FILE *fp = fopen(path, "rb");
  if (!fp)
    return NULL;
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  char *buffer = (char *)malloc(size + 1);
  if (!buffer) {
    fclose(fp);
    return NULL;
  }
  fread(buffer, 1, size, fp);
  fclose(fp);
  buffer[size] = '\0';
  return buffer;
}

RulesConfig *rules_load_from_file(const char *path) {
  char *content = read_file_to_string(path);
  if (!content)
    return NULL;

  RulesConfig *cfg = malloc(sizeof(RulesConfig));
  cfg->rules = NULL;
  cfg->rule_count = 0;

  const char *p = content;

  const char *rules_pos = strstr(p, "\"rules\"");
  if (rules_pos) {
    const char *arr_start = strchr(rules_pos, '[');
    const char *arr_end = strchr(rules_pos, ']');
    if (arr_start && arr_end) {
      const char *curr = arr_start + 1;
      while (curr < arr_end) {
        const char *obj_start = strchr(curr, '{');
        const char *obj_end = strchr(curr, '}');
        if (!obj_start || !obj_end || obj_start > obj_end)
          break;

        const char *inner = obj_start + 1;
        char *app = NULL, *layout = NULL;
        while (inner < obj_end) {
          char *key = extract_next_quoted_key(&inner);
          char *value = extract_next_quoted_value(&inner);
          if (key && value) {
            if (strcmp(key, "app") == 0)
              app = value;
            else if (strcmp(key, "layout") == 0)
              layout = value;
            else
              free(value);
            free(key);
          } else {
            free(key);
            free(value);
          }
        }

        if (app && layout) {
          cfg->rules = (AppRule *)realloc(
              cfg->rules, sizeof(AppRule) * (cfg->rule_count + 1));
          cfg->rules[cfg->rule_count].AppName = app;
          cfg->rules[cfg->rule_count].layout = layout;
          cfg->rule_count++;
        } else {
          free(app);
          free(layout);
        }

        curr = obj_end + 1;
      }
    }
  }

  free(content);
  return cfg;
}

void rules_free(RulesConfig *config) {
  if (!config)
    return;
  for (size_t i = 0; i < config->rule_count; i++) {
    free(config->rules[i].AppName);
    free(config->rules[i].layout);
  }
  free(config->rules);
  free(config);
}
