#include <stdio.h>
#include <assert.h>
#include "Lex.h"

void PrintToken(int tok) {
  switch (tok) {
    case TOK_WHILE:     printf("WHILE");      break;
    case TOK_FOR:       printf("FOR");        break;
    case TOK_TO:        printf("TO");         break;
    case TOK_BREAK:     printf("BREAK");      break;
    case TOK_LET:       printf("LET");        break;
    case TOK_IN:        printf("IN");         break;
    case TOK_END:       printf("END");        break;
    case TOK_FUNCTION:  printf("FUNCTION");   break;
    case TOK_VAR:       printf("VAR");        break;
    case TOK_TYPE:      printf("TYPE");       break;
    case TOK_ARRAY:     printf("ARRAY");      break;
    case TOK_IF:        printf("IF");         break;
    case TOK_THEN:      printf("THEN");       break;
    case TOK_ELSE:      printf("ELSE");       break;
    case TOK_DO:        printf("DO");         break;
    case TOK_OF:        printf("OF");         break;
    case TOK_NIL:       printf("NIL");        break;
    case TOK_ID:        printf("ID");         break;
    case TOK_INT:       printf("INT");        break;
    case TOK_STR:       printf("STR");        break;
    case TOK_GE:        printf("GE");         break;
    case TOK_LE:        printf("LE");         break;
    case TOK_LG:        printf("LG");         break;
    case TOK_ASSIGN:    printf("ASSIGN");     break;
    default:
      if (tok >= 32 && tok < 128) printf("'%c'", tok);
      else printf("UNKNOWN(%d)", tok);
  }
}

int main() {
  // 【终极全功能测试】
  // 关键字 + 数字 + 比较符 + 赋值符 + 字符串 + 换行 + 缩进
  char *input =
    "function main var x := 100\n"
    "if 55 <> 66 then x <= 999 else x >= 888\n"
    "while i to 10 do break end\n"
    "let type array of int in nil end\n"
    "1 22 333 4444 99999\n"
    "< > <= >= <> :=\n"
    "\"hello world\" \"test123\" \"abc\"\n"
    "id123 varxy ifx thenx elseend";

  printf("=== 开始词法分析 ===\n\n");

  GenTokens(input);

  printf("=== 分析结果 ===\n");
  for (int i = 0; i < tokens_size; ++i) {
    struct Token *t = &tokens[i];
    printf("%02d: ", i);
    PrintToken(t->tok);

    if (t->tok == TOK_ID)      printf(" \t-> %s", t->id);
    if (t->tok == TOK_STR)     printf(" \t-> \"%s\"", t->id);
    if (t->tok == TOK_INT)     printf(" \t-> %d", t->num);

    printf("\n");
  }

  return 0;
}
