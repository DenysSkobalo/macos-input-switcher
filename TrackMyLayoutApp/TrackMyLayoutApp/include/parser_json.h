#ifndef PARSER_JSON_H
#define PARSER_JSON_H

#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef enum {
  TOKEN_LBRACE,   // {
  TOKEN_RBRACE,   // }
  TOKEN_LBRACKET, // [
  TOKEN_RBRACKET, // ]
  TOKEN_COLON,    // :
  TOKEN_COMMA,    // ,

  TOKEN_STRING, // "text"
  TOKEN_NUMBER, // 123, -3.14, 1e10
  TOKEN_TRUE,   // true
  TOKEN_FALSE,  // false
  TOKEN_NULL,   // null

  TOKEN_EOF,  // end of input
  TOKEN_ERROR // lexical error
} TokenType;

typedef struct {
  TokenType type;
  const char *start;
  size_t length;

  int row;
  int column;
} Token;

typedef struct {
  const char *input;
  size_t length;

  size_t pos;
  int row;
  int column;
} Lexer;

typedef struct {
  Lexer lexer;
  Token current;
} Parser;

Token lexer_next_token(Lexer *l);
Token lexer_read_string(Lexer *l);
Token lexer_read_number(Lexer *l);
Token lexer_read_keyword(Lexer *l);
void lexer_init(Lexer *lexer, const char *input);

void parser_init(Parser *p, const char *json);
void parser_advance(Parser *p);
void parser_expect(Parser *p, TokenType type);

/*
 parse_rules_json
 └── parse_root
      └── parse_rules_array
           ├── parse_rule_object
           │    ├── parse_app_field
           │    └── parse_layout_field
           ├── parse_rule_object
           └── ...
*/

#ifdef __cplusplus
}
#endif

#endif
