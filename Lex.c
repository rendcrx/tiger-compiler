#include <assert.h>
#include <string.h>

#include "Lex.h"

#define MAX_TOKENS_SIZE 500
struct Token tokens[MAX_TOKENS_SIZE];
int tokens_size;

void GenTokens(char *str) {
  struct Token *t;
  char *p, c;
  int slen, n;
  tokens_size = 0;

  for (c = *str, t = &tokens[0], tokens_size = 0; *str; ++tokens_size, t = &tokens[tokens_size]) {
    if (c == 0)
      break;

    // enter space machine
    if (c == ' ' || c == '\t' || c == '\n') {
      do
        c = *++str;
      while (c == ' ' || c == '\t' || c == '\n');
      --tokens_size;
      continue;
    }

    // enter id machine
    if (c >= 'a' && c <= 'z' ||
        c >= 'A' && c <= 'Z') {
      p = str;
      do
        c = *++str;
      while (c >= 'a' && c <= 'z' ||
             c >= 'A' && c <= 'Z' ||
             c >= '0' && c <= '9');

      // enter keyword machine
      slen = str - p;
      if (slen == 5 && strncmp(p, "while", slen) == 0) {
        t->tok = TOK_WHILE;
      } else if (slen == 3 && strncmp(p, "for", slen) == 0) {
        t->tok = TOK_FOR;
      } else if (slen == 2 && strncmp(p, "to", slen) == 0) {
        t->tok = TOK_TO;
      } else if (slen == 5 && strncmp(p, "break", slen) == 0) {
        t->tok = TOK_BREAK;
      } else if (slen == 3 && strncmp(p, "let", slen) == 0) {
        t->tok = TOK_LET;
      } else if (slen == 2 && strncmp(p, "in", slen) == 0) {
        t->tok = TOK_IN;
      } else if (slen == 3 && strncmp(p, "end", slen) == 0) {
        t->tok = TOK_END;
      } else if (slen == 8 && strncmp(p, "function", slen) == 0) {
        t->tok = TOK_FUNCTION;
      } else if (slen == 3 && strncmp(p, "var", slen) == 0) {
        t->tok = TOK_VAR;
      } else if (slen == 4 && strncmp(p, "type", slen) == 0) {
        t->tok = TOK_TYPE;
      } else if (slen == 5 && strncmp(p, "array", slen) == 0) {
        t->tok = TOK_ARRAY;
      } else if (slen == 2 && strncmp(p, "if", slen) == 0) {
        t->tok = TOK_IF;
      } else if (slen == 4 && strncmp(p, "then", slen) == 0) {
        t->tok = TOK_THEN;
      } else if (slen == 4 && strncmp(p, "else", slen) == 0) {
        t->tok = TOK_ELSE;
      } else if (slen == 2 && strncmp(p, "do", slen) == 0) {
        t->tok = TOK_DO;
      } else if (slen == 2 && strncmp(p, "of", slen) == 0) {
        t->tok = TOK_OF;
      } else if (slen == 3 && strncmp(p, "nil", slen) == 0) {
        t->tok = TOK_NIL;
      } else {
        t->tok = TOK_ID;
        strncpy(t->id, p, slen);
        t->id[slen] = 0;
      }
      continue;
    }

    // number marchine
    if (c >= '0' && c <= '9') {
      n = 0;
      do {
        n = n * 10 + c - '0';
        c = *++str;
      } while (c >= '0' && c <= '9');
      t->tok = TOK_INT;
      t->num = n;
      continue;
    }

    // >=
    if (c == '>') {
      c = *++str;
      if (c == '=') {
        t->tok = TOK_GE;
        c = *++str;
      } else {
        t->tok = '>';
      }
      continue;
    }

    // <=
    // <>
    if (c == '<') {
      c = *++str;
      if (c == '=') {
        t->tok = TOK_LE;
        c = *++str;
      } else if (c == '>') {
        t->tok = TOK_LG;
        c = *++str;
      } else
        t->tok = '<';
      continue;
    }

    // :=
    if (c == ':') {
      c = *++str;
      if (c == '=') {
	t->tok = TOK_ASSIGN;
	c = *++str;
      } else
	t->tok = ':';
      continue;
    }

    // string
    if (c == '"') {
      c = *++str;
      p = str;
    redo:
      /* if (c == '\') */
      if (c != '"') {
        c = *++str;
        goto redo;
      }
      slen = str - p;
      t->tok = TOK_STR;
      strncpy(t->id, p, slen);
      t->id[slen] = 0;
      c = *++str;
      continue;
    }

    t->tok = c;
    c = *++str;
  }
}
