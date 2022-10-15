/*
Binome
Hadjerci Mohammed Allaeddine
Mezhoud Abdelmoumen
ACAD A
*/

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
[ \t] { colone++; }
"{" { colone++; return RBRACE; }
"}" { colone++; return LBRACE; }
";" { colone++; return POINTVIRGULE; }
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
"//"(.)* { colone += yyleng; }
"/*"[^*/]*"*/" { colone +=  yyleng; }
"define" { colone +=  yyleng; return DEF; }
"Uint" { colone +=  yyleng; yylval.str=strdup(yytext); return INT; }
"Ufloat" { colone +=  yyleng; yylval.str=strdup(yytext); return FLOAT; }
"CHECK" { colone +=  yyleng; return CHECK; }
"ENDCHECK" { colone +=  yyleng; return ENDCHECK; }
"REPEAT" { colone +=  yyleng; return REPEAT; }
"ENDREPEAT" { colone +=  yyleng; return ENDREPEAT; }
[0-9]+ { colone += yyleng; if(atoi(strdup(yytext)) <= 32767){ yylval.str=strdup(yytext); return Uint;} }
[0-9]+"."[0-9]+ { colone += yyleng; yylval.str=strdup(yytext); return Ufloat; }
[a-z][_a-z0-9]* { colone += yyleng;
                  if(yyleng < 9 && CheckUnderScore((char*)strdup(yytext),yyleng)){ yylval.str=strdup(yytext); return IDF; }
                  else{printf("erreur syntaxique de IDF\n");}}


%%

int yywrap(){
  return 1;
}
