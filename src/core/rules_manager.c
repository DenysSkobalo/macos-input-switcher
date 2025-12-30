#include "rules_manager.h"
#include "file_utils.h"
#include "parser_json.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void parse_rule_array(Parser *p, RulesConfig *cfg);
static void parse_rule_object(Parser *p, RulesConfig *cfg);

void print_parsed_rules(RulesConfig *cfg) {
  if (!cfg)
    return;
  printf("Parsed rules (%zu):\n", cfg->rule_count);
  for (size_t i = 0; i < cfg->rule_count; i++) {
    printf("  AppName: '%s', Layout: '%s'\n", cfg->rules[i].app_name,
           cfg->rules[i].layout);
  }
}

RulesConfig *rules_load_from_file(const char *path) {
  char *content = read_file_to_string(path);
  if (!content)
    return NULL;

  RulesConfig *cfg = malloc(sizeof(RulesConfig));
  cfg->rules = NULL;
  cfg->rule_count = 0;

  Parser parser;
  parser_init(&parser, content);
  parser_expect(&parser, TOKEN_LBRACE);

  while (parser.current.type != TOKEN_RBRACE &&
         parser.current.type != TOKEN_EOF) {
    if (parser.current.type == TOKEN_STRING) {
      const char *key_start = parser.current.start;
      size_t key_len = parser.current.length;

      parser_advance(&parser);
      parser_expect(&parser, TOKEN_COLON);

      if (key_len == strlen(RULES) && strncmp(key_start, RULES, key_len) == 0) {
        parse_rule_array(&parser, cfg);
      } else {
        parser_advance(&parser);
      }
    } else {
      parser_advance(&parser);
    }

    if (parser.current.type == TOKEN_COMMA)
      parser_advance(&parser);
  }

  parser_expect(&parser, TOKEN_RBRACE);

  free(content);
  return cfg;
}

static void parse_rule_array(Parser *p, RulesConfig *cfg) {
  parser_expect(p, TOKEN_LBRACKET);
  while (p->current.type != TOKEN_RBRACKET && p->current.type != TOKEN_EOF) {
    parse_rule_object(p, cfg);
    if (p->current.type == TOKEN_COMMA) {
      parser_advance(p);
    }
  }
  parser_expect(p, TOKEN_RBRACKET);
}

static void parse_rule_object(Parser *p, RulesConfig *cfg) {
  parser_expect(p, TOKEN_LBRACE);

  char *app_name = NULL;
  char *layout = NULL;

  while (p->current.type != TOKEN_RBRACE && p->current.type != TOKEN_EOF) {
    if (p->current.type == TOKEN_STRING) {
      size_t key_len = p->current.length;
      char *key = strndup(p->current.start, key_len);
      parser_advance(p);
      parser_expect(p, TOKEN_COLON);

      if (p->current.type == TOKEN_STRING) {
        char *value = strndup(p->current.start, p->current.length);
        if (key_len == strlen(APP) && strncmp(key, APP, key_len) == 0)
          app_name = value;
        else if (key_len == strlen(LAYOUT) &&
                 strncmp(key, LAYOUT, key_len) == 0)
          layout = value;
        else
          free(value);
        parser_advance(p);
      }
      free(key);
    } else {
      parser_advance(p);
    }

    if (p->current.type == TOKEN_COMMA)
      parser_advance(p);
  }
  parser_expect(p, TOKEN_RBRACE);

  if (app_name && layout) {
    cfg->rules =
        (AppRule *)realloc(cfg->rules, sizeof(AppRule) * (cfg->rule_count + 1));
    cfg->rules[cfg->rule_count].app_name = app_name;
    cfg->rules[cfg->rule_count].layout = layout;
    cfg->rule_count++;
  } else {
    free(app_name);
    free(layout);
  }
}
