%{
#include<stdio.h>
#include "ts.h"
#include "projetb.tab.h"
extern YYSTYPE yylval;
int ligne = 1;
int colone = 1;
%}

%%
\n { ligne++; colone = 1; }
[\t] { colone+=yyleng; }
[ ] { colone+=yyleng; }
"}" { return RBRACE; }
"{" { return LBRACE; }
"]" { colone++; return RHBRACE; }
"[" { colone++; return LHBRACE; }
";" { return POINTVIRGULE; }
"," { colone++; return VIRGUL; }
"==" { colone+=2; return EQ; }
">" { colone++; return SUP; }
"<" { colone++; return INF; }
">=" { colone+=2; return SUPEQ; }
"<=" { colone+=2; return INFEQ; }
"=" { colone++; return AFFECT; }
"+" { colone++; return PLUS; }
"-" { colone++; return MINUS; }
"*" { colone++; return MUL; }
"/" { colone++; return DIV; }
"(" { colone++; return LPARENTHESE; }
")" { colone++; return RPARENTHESE; }
"|" { colone++; return OR; }
"&" { colone++; return AND; }
"!=" { colone+=2; return DIFFERENT; }
":" { colone++; return DPOINT; }
"!" { colone++; return NEGATION; }
"int" { colone +=  yyleng; yylval.str=strdup(yytext); return INT; }
"float" { colone +=  yyleng; yylval.str=strdup(yytext); return FLOAT; }
"char" { colone += yyleng; yylval.str=strdup(yytext); return CHAR; }
"if" { colone +=  yyleng; return IF; }
"elif" { colone += yyleng; return ELIF; }
"else" { colone += yyleng; return ELSE; }
"for" { colone +=  yyleng; return FOR; }
"while" { colone +=  yyleng; return WHILE; }
"in" { colone += yyleng; return IN; }
"range" { colone+=yyleng; return RANGE; }
[0-9]+ { colone += yyleng; if(atoi(strdup(yytext)) <= 32767){ yylval.str=strdup(yytext); return Uint;} }
[0-9]+"."[0-9]+ { colone += yyleng; yylval.str=strdup(yytext); return Ufloat; }
[a-z][_a-z0-9]* { colone += yyleng;
                  if(yyleng < 9 && CheckUnderScore((char*)strdup(yytext),yyleng)){ yylval.str=strdup(yytext); return IDF; }
                  else{printf("erreur syntaxique de IDF\n");}}
"'"[^']"'" { colone +=yyleng; yylval.str=strdup(yytext); return Uchar; }

"#".* { colone += yyleng; }
"#".*"\n" { ligne++; colone = 1; }

%%

int yywrap(){
  return 1;
}
