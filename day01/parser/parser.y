%{
#include <cstdio>
#include <cstdlib>

int yylex(void);
void yyerror(const char *s);
%}

%union {
  int ival;
}

%token <ival> NUMBER
%left '+' '-'
%left '*' '/'
%right UMINUS
%type <ival> expr

%%
input:
    /* empty */
  | input line
  ;

line:
    '\n'
  | expr '\n' { std::printf("= %d\n", $1); }
  ;

expr:
    NUMBER              { $$ = $1; }
  | expr '+' expr      { $$ = $1 + $3; }
  | expr '-' expr      { $$ = $1 - $3; }
  | expr '*' expr      { $$ = $1 * $3; }
  | expr '/' expr      { $$ = $3 == 0 ? 0 : $1 / $3; }
  | '-' expr %prec UMINUS { $$ = -$2; }
  | '(' expr ')'       { $$ = $2; }
  ;
%%

void yyerror(const char *s) {
  std::fprintf(stderr, "parser error: %s\n", s);
}

int main() {
  std::puts("Enter arithmetic expressions, then press Ctrl+D (Unix) or Ctrl+Z (Windows) to quit.");
  return yyparse();
}
