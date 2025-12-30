#include "parser_json.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void advance_pos(Lexer *l) {
  if (l->pos < l->length) {
    if (l->input[l->pos] == '\n') {
      l->row++;
      l->column = 1;
    } else {
      l->column++;
    }
    l->pos++;
  }
}

static bool is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

Token lexer_read_string(Lexer *l) {
  advance_pos(l);
  size_t start_index = l->pos;
  const char *start = &l->input[start_index];

  while (l->pos < l->length) {
    char c = l->input[l->pos];
    if (c == '"')
      break;
    if (c == '\\')
      advance_pos(l);
    advance_pos(l);
  }

  if (l->pos >= l->length || l->input[l->pos] != '"') {
    return (Token){TOKEN_ERROR, start, l->pos - start_index, l->row, l->column};
  }

  Token t = {TOKEN_STRING, start, l->pos - start_index, l->row, l->column};
  advance_pos(l);
  return t;
}

// TODO
Token lexer_read_number(Lexer *l) {
  Token t = {TOKEN_ERROR, NULL, 0, l->row, l->column};
  return t;
}

// TODO
Token lexer_read_keyword(Lexer *l) {
  Token t = {TOKEN_ERROR, NULL, 0, l->row, l->column};
  return t;
}

Token lexer_next_token(Lexer *l) {
  while (l->pos < l->length && is_whitespace(l->input[l->pos])) {
    advance_pos(l);
  }

  if (l->pos >= l->length) {
    return (Token){TOKEN_EOF, NULL, 0, l->row, l->column};
  }

  char c = l->input[l->pos];
  int row = l->row;
  int col = l->column;

  switch (c) {
  case '{':
    advance_pos(l);
    return (Token){TOKEN_LBRACE, &l->input[l->pos - 1], 1, row, col};
  case '}':
    advance_pos(l);
    return (Token){TOKEN_RBRACE, &l->input[l->pos - 1], 1, row, col};
  case '[':
    advance_pos(l);
    return (Token){TOKEN_LBRACKET, &l->input[l->pos - 1], 1, row, col};
  case ']':
    advance_pos(l);
    return (Token){TOKEN_RBRACKET, &l->input[l->pos - 1], 1, row, col};
  case ':':
    advance_pos(l);
    return (Token){TOKEN_COLON, &l->input[l->pos - 1], 1, row, col};
  case ',':
    advance_pos(l);
    return (Token){TOKEN_COMMA, &l->input[l->pos - 1], 1, row, col};
  }

  if (c == '"')
    return lexer_read_string(l);
  if (c == '-' || isdigit(c))
    return lexer_read_number(l);
  if (isalpha(c))
    return lexer_read_keyword(l);

  advance_pos(l);
  return (Token){TOKEN_ERROR, &l->input[l->pos - 1], 1, row, col};
}

void lexer_init(Lexer *lexer, const char *input) {
  lexer->input = input;
  lexer->length = input ? strlen(input) : 0;
  lexer->pos = 0;
  lexer->row = 1;
  lexer->column = 1;
}

void parser_init(Parser *p, const char *json) {
  lexer_init(&p->lexer, json);
  p->current = lexer_next_token(&p->lexer);
}

void parser_advance(Parser *p) { p->current = lexer_next_token(&p->lexer); }

void parser_expect(Parser *p, TokenType type) {
  if (p->current.type != type) {
    fprintf(
        stderr,
        "[PARSER ERROR] Expected token type %d, got %d at row %d column %d\n",
        type, p->current.type, p->current.row, p->current.column);
    exit(EXIT_FAILURE);
  }
  parser_advance(p);
}
