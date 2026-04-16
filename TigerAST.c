#include <stdlib.h>
#include <string.h>
#include "TigerAST.h"
#include "Lex.h"

static struct Token *curtok;

static void nexttok() {
  static int i;
  if (i >= tokens_size)
    curtok = NULL;
  else
    curtok = tokens[i++];
}

static void expecttok(int class) {
  static int i;
  if (i >= tokens_size)
    curtok = NULL;
  else
    curtok = tokens[i++];
  assert(curtok->tok == class);
}

static struct Token *peer() {
  if (i+1 >= tokens_size)
    return NULL;
  else
    return tokens[i+1];
}

struct A_Program *A_Program_Epsilon() {
  struct A_Program *p = malloc(sizeof *p);
  p->kind = EPSILON;
  return p;
}

struct A_Program *A_Program_Exp(struct A_Exp *exp) {
  struct A_Program *p = malloc(sizeof *p);
  p->kind = EXP;
  p->exp = exp;
  return p;
}

struct A_Exp *A_Exp_Int(int num) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_INT;
  e->u.num = num;
  return e;
}

struct A_Exp *A_Exp_String(char *str) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_STRING;
  strcpy(e->u.str, str);
  return e;
}

struct A_Exp *A_Exp_Nil() {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_NIL;
  return e;
}

struct A_Exp *A_Exp_Id(char *id, struct A_Arglist *arglist) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_ID;
  strcpy(e->u.id.id, id);
  e->u.id.arglist = arglist;
  return e;
}

struct A_Exp *A_Exp_Lvalue(struct A_Lvalue *lval) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LVALUE;
  e->u.lval = lval;
  return e;
}

struct A_Exp *A_Exp_Expseq(struct A_Expseq *expseq) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_EXPSEQ;
  e->u.expseq = expseq;
  return e;
}

struct A_Exp *A_Exp_Condexp(struct A_Condexp *condexp) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_CONDEXP;
  e->u.condexp = condexp;
  return e;
}

struct A_Exp *A_Exp_Letexp(struct A_Letexp *letexp) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LETEXP;
  e->u.letexp = letexp;
  return e;
}

struct A_Exp *A_Exp_Or(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_OR;
  e->u.or.left = left;
  e->u.or.right = right;
  return e;
}

struct A_Exp *A_Exp_And(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_AND;
  e->u.and.left = left;
  e->u.and.right = right;
  return e;
}

struct A_Exp *A_Exp_Lt(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LT;
  e->u.lt.left = left;
  e->u.lt.right = right;
  return e;
}

struct A_Exp *A_Exp_Gt(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_GT;
  e->u.gt.left = left;
  e->u.gt.right = right;
  return e;
}

struct A_Exp *A_Exp_Le(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LE;
  e->u.le.left = left;
  e->u.le.right = right;
  return e;
}

struct A_Exp *A_Exp_Ge(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_GE;
  e->u.ge.left = left;
  e->u.ge.right = right;
  return e;
}

struct A_Exp *A_Exp_Add(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_ADD;
  e->u.add.left = left;
  e->u.add.right = right;
  return e;
}

struct A_Exp *A_Exp_Sub(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_SUB;
  e->u.sub.left = left;
  e->u.sub.right = right;
  return e;
}

struct A_Exp *A_Exp_Times(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_TIMES;
  e->u.times.left = left;
  e->u.times.right = right;
  return e;
}

struct A_Exp *A_Exp_Div(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_DIV;
  e->u.div.left = left;
  e->u.div.right = right;
  return e;
}

struct A_Exp *A_Exp_Eq(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_EQ;
  e->u.eq.left = left;
  e->u.eq.right = right;
  return e;
}

struct A_Exp *A_Exp_Lg(struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_LG;
  e->u.lg.left = left;
  e->u.lg.right = right;
  return e;
}

struct A_Exp *A_Exp_Of(char id[], struct A_Exp *left, struct A_Exp *right) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_OF;
  strcpy(e->u.of.id, id);
  e->u.of.left = left;
  e->u.of.right = right;
  return e;
}

struct A_Exp *A_Exp_Reclist(char id[], struct A_Reclist *reclist) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_RECLIST;
  strcpy(e->u.reclist.id, id);
  e->u.reclist.reclist = reclist;
  return e;
}

struct A_Exp *A_Exp_Assign(struct A_Lvalue *lvalue, struct A_Exp *exp) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_ASSIGN;
  e->u.assign.lvalue = lvalue;
  e->u.assign.exp = exp;
  return e;
}

struct A_Exp *A_Exp_Singlesub(struct A_Exp *exp) {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_SINGLESUB;
  e->u.exp = exp;
  return e;
}

struct A_Exp *A_Exp_Break() {
  struct A_Exp *e = malloc(sizeof *e);
  e->kind = A_EXP_BREAK;
  return e;
}

struct A_Condexp *A_Condexp_Ifelse(struct A_Exp *e1, struct A_Exp *e2, struct A_Exp *e3) {
  struct A_Condexp *c = malloc(sizeof *c);
  c->kind = A_CONDEXP_IFELSE;
  c->u.ifelse.e1 = e1;
  c->u.ifelse.e2 = e2;
  c->u.ifelse.e3 = e3;
  return c;
}

struct A_Condexp *A_Condexp_Ifthen(struct A_Exp *e1, struct A_Exp *e2) {
  struct A_Condexp *c = malloc(sizeof *c);
  c->kind = A_CONDEXP_IFTHEN;
  c->u.ifthen.e1 = e1;
  c->u.ifthen.e2 = e2;
  return c;
}

struct A_Condexp *A_Condexp_While(struct A_Exp *e1, struct A_Exp *e2) {
  struct A_Condexp *c = malloc(sizeof *c);
  c->kind = A_CONDEXP_WHILE;
  c->u.whi.e1 = e1;
  c->u.whi.e2 = e2;
  return c;
}

struct A_Condexp *A_Condexp_For(char id[], struct A_Exp *e1, struct A_Exp *e2, struct A_Exp *e3) {
  struct A_Condexp *c = malloc(sizeof *c);
  c->kind = A_CONDEXP_FOR;
  strcpy(c->u.fo.id, id);
  c->u.fo.e1 = e1;
  c->u.fo.e2 = e2;
  c->u.fo.e3 = e3;
  return c;
}


struct A_Letexp *A_Letexp(struct A_Decs *decs, struct A_Expseq *expseq) {
  struct A_Letexp *l = malloc(sizeof *l);
  l->decs = decs;
  l->expseq = expseq;
  return l;
}

struct A_Lvalue *A_Lvalue_Id(char id[]) {
  struct A_Lvalue *l = malloc(sizeof *l);
  l->kind = A_LVALUE_ID;
  strcpy(l->u.id, id);
  return l;
}

struct A_Lvalue *A_Lvalue_Idexp(char *id, struct A_Exp *exp) {
  struct A_Lvalue *l = malloc(sizeof *l);
  l->kind = A_LVALUE_IDEXP;
  strcpy(l->u.idexp.id, id);
  l->u.idexp.exp = exp;
  return l;
}

struct A_Lvalue *A_Lvalue_Lexp(struct A_Lvalue *lvalue, struct A_Exp *exp) {
  struct A_Lvalue *l = malloc(sizeof *l);
  l->kind = A_LVALUE_LEXP;
  l->u.lexp.lvalue = lvalue;
  l->u.lexp.exp = exp;
  return l;
}

struct A_Lvalue *A_Lvalue_Lid(struct A_Lvalue *lvalue, char *id) {
  struct A_Lvalue *l = malloc(sizeof *l);
  l->kind = A_LVALUE_LID;
  l->u.lid.lvalue = lvalue;
  strcpy(l->u.lid.id, id);
  return l;
}

struct A_Arglist *A_Arglist_Epsilon() {
  struct A_Arglist *a = malloc(sizeof *a);
  a->kind = A_ARGLIST_EPSILON;
  return a;
}

struct A_Arglist *A_Arglist_Nonarglist(struct A_Nonarglist *nonarglist) {
  struct A_Arglist *a = malloc(sizeof *a);
  a->kind = A_ARGLIST_NONARGLIST;
  a->nonarglist = nonarglist;
  return a;
}

struct A_Nonarglist *A_Nonarglist_Exp(struct A_Exp *exp) {
  struct A_Nonarglist *n = malloc(sizeof *n);
  n->kind = A_NONARGLIST_EXP;
  n->u.exp = exp;
  return n;
}

struct A_Nonarglist *A_Nonarglist_Expnonarglist(struct A_Exp *exp, struct A_Nonarglist *arglist) {
  struct A_Nonarglist *n = malloc(sizeof *n);
  n->kind = A_NONARGLIST_EXPNONARGLIST;
  n->u.expnonarglist.exp = exp;
  n->u.expnonarglist.arglist = arglist;
  return n;
}

struct A_Reclist *A_Reclist_Epsilon() {
  struct A_Reclist *r = malloc(sizeof *r);
  r->kind = A_RECLIST_EPSILON;
  return r;
}

struct A_Reclist *A_Reclist_Nonreclist(struct A_Nonreclist *nonreclist) {
  struct A_Reclist *r = malloc(sizeof *r);
  r->kind = A_RECLIST_NONRECLIST;
  r->nonreclist = nonreclist;
  return r;
}

struct A_Nonreclist *A_Nonreclist_Idexp(char *id, struct A_Exp *exp) {
  struct A_Nonreclist *n = malloc(sizeof *n);
  n->kind = A_NONRECLIST_IDEXP;
  strcpy(n->u.idexp.id, id);
  n->u.idexp.exp = exp;
  return n;
}

struct A_Nonreclist *A_Nonreclist_Nonreclist(char *id, struct A_Exp *exp, struct A_Nonreclist *nonreclist) {
  struct A_Nonreclist *n = malloc(sizeof *n);
  n->kind = A_NONRECLIST_NONRECLIST;
  strcpy(n->u.nonreclist.id, id);
  n->u.nonreclist.exp = exp;
  n->u.nonreclist.nonreclist = nonreclist;
  return n;
}

struct A_Expseq *A_Expseq_Epsilon() {
  struct A_Expseq *e = malloc(sizeof *e);
  e->kind = A_EXPSEQ_EPSILON;
  return e;
}

struct A_Expseq *A_Expseq_Exp(struct A_Exp *exp) {
  struct A_Expseq *e = malloc(sizeof *e);
  e->kind = A_EXPSEQ_EXP;
  e->u.exp = exp;
  return e;
}

struct A_Expseq *A_Expseq_Expseq(struct A_Exp *exp, struct A_Expseq *expseq) {
  struct A_Expseq *e = malloc(sizeof *e);
  e->kind = A_EXPSEQ_EXPSEQ;
  e->u.expseq.exp = exp;
  e->u.expseq.expseq = expseq;
  return e;
}

struct A_Decs *A_Decs_Epsilon() {
  struct A_Decs *d = malloc(sizeof *d);
  d->kind = A_DECS_EPSILON;
  return d;
}

struct A_Decs *A_Decs_Decs(struct A_Dec *dec, struct A_Decs *decs) {
  struct A_Decs *d = malloc(sizeof *d);
  d->kind = A_DECS_DECS;
  d->dec = dec;
  d->decs = decs;
  return d;
}

struct A_Dec *A_Dec_Tydecs(struct A_Tydecs *tydecs) {
  struct A_Dec *d = malloc(sizeof *d);
  d->kind = A_DEC_TYDECS;
  d->u.tydecs = tydecs;
  return d;
}

struct A_Dec *A_Dec_Vardec(struct A_Vardec *vardec) {
  struct A_Dec *d = malloc(sizeof *d);
  d->kind = A_DEC_VARDEC;
  d->u.vardec = vardec;
  return d;
}

struct A_Dec *A_Dec_Fundecs(struct A_Fundecs *fundecs) {
  struct A_Dec *d = malloc(sizeof *d);
  d->kind = A_DEC_FUNDECS;
  d->u.fundecs = fundecs;
  return d;
}

struct A_Tydecs *A_Tydecs_Tydec(struct A_Tydec *tydec) {
  struct A_Tydecs *t = malloc(sizeof *t);
  t->kind = A_TYDECS_TYDEC;
  t->u.tydec = tydec;
  return t;
}

struct A_Tydecs *A_Tydecs_Tydecs(struct A_Tydec *tydec, struct A_Tydecs *tydecs) {
  struct A_Tydecs *t = malloc(sizeof *t);
  t->kind = A_TYDECS_TYDECS;
  t->u.tydecs.tydec = tydec;
  t->u.tydecs.tydecs = tydecs;
  return t;
}

struct A_Tydec *A_Tydec(char *id, struct A_Ty *ty) {
  struct A_Tydec *t = malloc(sizeof *t);
  strcpy(t->id, id);
  t->ty = ty;
  return t;
}

struct A_Ty *A_Ty_Id(char *id) {
  struct A_Ty *t = malloc(sizeof *t);
  t->kind = A_TY_ID;
  strcpy(t->u.id, id);
  return t;
}

struct A_Ty *A_Ty_Tyfields(struct A_Tyfields *tyfields) {
  struct A_Ty *t = malloc(sizeof *t);
  t->kind = A_TY_TYFIELDS;
  t->u.tyfields = tyfields;
  return t;
}

struct A_Ty *A_Ty_Of(char *ofid) {
  struct A_Ty *t = malloc(sizeof *t);
  t->kind = A_TY_OF;
  strcpy(t->u.ofid, ofid);
  return t;
}

struct A_Tyfields *A_Tyfields_Epsilon() {
  struct A_Tyfields *t = malloc(sizeof *t);
  t->kind = A_TYFIELDS_EPSILON;
  return t;
}

struct A_Tyfields *A_Tyfields_Tyfields(struct A_Tyfields1 *tyfields1) {
  struct A_Tyfields *t = malloc(sizeof *t);
  t->kind = A_TYFIELDS_TYFIELDS1;
  t->tyfields1 = tyfields1;
  return t;
}

struct A_Tyfields1 *A_Tyfields1_Tyfield(struct A_Tyfield *tyfield) {
  struct A_Tyfields1 *t = malloc(sizeof *t);
  t->kind = A_TYFIELDS1_TYFIELD;
  t->u.tyfield = tyfield;
  return t;
}

struct A_Tyfields1 *A_Tyfields1_Tyfield1(struct A_Tyfield *tyfield, struct A_Tyfields1 *tyfields1) {
  struct A_Tyfields1 *t = malloc(sizeof *t);
  t->kind = A_TYFIELDS1_TYFIELD1;
  t->u.tyfield1.tyfield = tyfield;
  t->u.tyfield1.tyfields1 = tyfields1;
  return t;
}

struct A_Tyfield *A_Tyfield(char *id1, char *id2) {
  struct A_Tyfield *t = malloc(sizeof *t);
  strcpy(t->id1, id1);
  strcpy(t->id2, id2);
  return t;
}

struct A_Vardec *A_Vardec_Id(char *id, struct A_Exp *exp) {
  struct A_Vardec *v = malloc(sizeof *v);
  v->kind = A_VARDEC_ID;
  strcpy(v->u.id.id, id);
  v->u.id.exp = exp;
  return v;
}

struct A_Vardec *A_Vardec_Idid(char *id1, char *id2, struct A_Exp *exp) {
  struct A_Vardec *v = malloc(sizeof *v);
  v->kind = A_VARDEC_IDID;
  strcpy(v->u.idid.id1, id1);
  strcpy(v->u.idid.id2, id2);
  v->u.idid.exp = exp;
  return v;
}

struct A_Fundecs *A_Fundecs_Fundec(struct A_Fundec *fundec) {
  struct A_Fundecs *f = malloc(sizeof *f);
  f->kind = A_FUNDECS_FUNDEC;
  f->u.fundec = fundec;
  return f;
}

struct A_Fundecs *A_Fundecs_Fundecs(struct A_Fundec *fundec, struct A_Fundecs *fundecs) {
  struct A_Fundecs *f = malloc(sizeof *f);
  f->kind = A_FUNDECS_FUNDECS;
  f->u.fundecs.fundec = fundec;
  f->u.fundecs.fundecs = fundecs;
  return f;
}

struct A_Fundec *A_Fundec_Id(char *id, struct A_Tyfields *tyfields, struct A_Exp *exp) {
  struct A_Fundec *f = malloc(sizeof *f);
  f->kind = A_FUNDEC_ID;
  strcpy(f->u.id.id, id);
  f->u.id.tyfields = tyfields;
  f->u.id.exp = exp;
  return f;
}

struct A_Fundec *A_Fundec_Idid(char *id1, struct A_Tyfields *tyfields, char *id2, struct A_Exp *exp) {
  struct A_Fundec *f = malloc(sizeof *f);
  f->kind = A_FUNDEC_IDID;
  strcpy(f->u.idid.id1, id1);
  f->u.idid.tyfields = tyfields;
  f->u.idid.exp = exp;
  strcpy(f->u.idid.id2, id2);
  return f;
}

struct A_Exp *ParseExp() {
  struct Token *p;
  struct A_Exp *e;
  char *a1;
  switch (curtok->tok) {
    case TOK_INT:
      e = A_Exp_Int(curtok->num);
      break;
    case TOK_STR:
      e = A_Exp_String(cutrok->id);
      break;
    case TOK_NIL:
      e = A_Exp_Nil();
      break;
    case TOK_ID:
      p = peer();
      switch (p) {
        case '(':
          a1 = e->id;
          expecttok('(');
          nexttok();
          e = A_Exp_Id(a1, ParseArglist());
          break;
        case '[':
        case '{':
        default: e = A_Exp_Lvalue(ParseLvalue()); break;
      }
      break;
    case '(':
      nexttok();
      e = A_Exp_Expseq(ParseExpseq());
      break;
    case TOK_IF:
    case TOK_WHILE:
    case TOK_FOR:
      e = A_Exp_Condexp(ParseCondexp());
      break;
    case TOK_LET:
      e = A_Exp_Letexp(ParseLetexp());
      break;
    case TOK_STR:
    case TOK_STR:
    case TOK_STR:
    case TOK_STR:
    case TOK_STR:
  }
  nexttok();
  return e;
}

struct A_Condexp *ParseCondexp() {
}

struct A_Letexp *ParseLetexp() {
}

struct A_Lvalue *ParseLvalue() {
}

struct A_Arglist *ParseArglist() {
}

struct A_Nonarglist *ParseNonarglist() {
}

struct A_Reclist *ParseReclist() {
}

struct A_Nonreclist *ParseNonreclist() {
}

struct A_Expseq *ParseExpseq() {
}

struct A_Decs *ParseDecs() {
}

struct A_Dec *ParseDec() {
}

struct A_Tydecs *ParseTydecs() {
}

struct A_Tydec *ParseTydec() {
}

struct A_Ty *ParseTy() {
}

struct A_Tyfields *ParseTyfields() {
}

struct A_Tyfields1 *ParseTyfields1() {
}

struct A_Tyfield *ParseTyfield() {
}

struct A_Vardec *ParseVardec() {
}

struct A_Fundecs *ParseFundecs() {
}

struct A_Fundec *ParseFundec() {
}

struct A_Program *ParseProgram() {
  nexttok();
  switch (curtok->tok) {
    case NULL: return A_Program_Epsilon();
    default: return ParseExp();
  }
}
