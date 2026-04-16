#include <stdio.h>
#include <stdlib.h>
#include "Lex.h"
#include "TigerAST.h"

int main() {
  FILE *i = fopen("merge.tig", "r");
  fseek(i, 0L, SEEK_END);
  long s = ftell(i);
  fseek(i, 0L, SEEK_SET);
  char *cont = malloc(s);
  fread(cont, s, 1, i);
  GenTokens(cont);
  Parse();
}
