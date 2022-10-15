%{
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
extern FILE *yyin;
int yylex();
int yyerror(char *);
extern int ligne;
extern int colone;
extern char *yytext;

int ind = 0;
int typ = 0;
int notypvar = 0;
int numRes = 1;
int numExp = 1;
int andcond;
int nb = 0;
char buffer[100];

struct Pile{
  char opd[20];
  struct Pile *next;
};

struct MisA{
  int opd;
  struct MisA *next;
};

typedef struct Pile Pile;
typedef struct MisA MisA;
MisA *mis2 = NULL;
MisA *mis = NULL;
char opd1[20];
char opd2[20];

Pile *tete = NULL;

void Empiler(char *opd){
    Pile *temp = malloc(sizeof(Pile));
    strcpy(temp->opd,opd);
    temp->next = tete;
    tete = temp;
}

void EmpMis(int opd){
    MisA *temp = malloc(sizeof(MisA));
    temp->opd = opd;
    temp->next = mis;
    mis = temp;
}

void EmpMisAndOr(int opd){
    MisA *temp = malloc(sizeof(MisA));
    temp->opd = opd;
    temp->next = mis2;
    mis2 = temp;
}

void DepMis(){
    MisA *temp = mis;
    if(temp != NULL){
      mis = mis->next;
      free(temp);
    }
}

void DepMisAndOr(){
    MisA *temp = mis2;
    if(temp != NULL){
      mis2 = mis2->next;
      free(temp);
    }
}

void Depiler(){
    Pile *temp = tete;
    if(temp != NULL){
      tete = tete->next;
      free(temp);
    }
}

void AffichePile(){
    MisA *temp = mis;
    while(temp != NULL){
        printf("%d\n",mis->opd);
        temp = temp->next;
    }
}

%}

%union
{
  char *str;
}

%token <str> IDF
%token <str> Uint
%token <str> Ufloat
%token <str> Uchar
%token RBRACE
%token LBRACE
%token RHBRACE
%token LHBRACE
%token POINTVIRGULE
%token VIRGUL
%token EQ
%token SUP
%token INF
%token SUPEQ
%token INFEQ
%token AFFECT
%token PLUS
%token MINUS
%token MUL
%token DIV
%token RPARENTHESE
%token LPARENTHESE
%token OR
%token AND
%token DIFFERENT 
%token <str> INT
%token <str> FLOAT
%token <str> CHAR
%token IF
%token ELIF
%token ELSE
%token FOR
%token IN
%token RANGE
%token WHILE
%token DPOINT
%token NEGATION

%type <str> varIDF varIDFF morevar morevarr value exp A B


%%

program : statement program | {printf("programme correct\n");};

statement : inst
          | variable POINTVIRGULE;

inst :  loopwhile
      | loopfor
      | checking;

variable :  CHAR {typ = 2;} morevar
          | INT {typ = 0;} morevar
          | FLOAT {typ = 1;} morevar
          | morevarr;

morevarr :  varIDFF AFFECT exp {
                                  modifyType(ind, typ);
                                  InsertQuad("=",tete->opd," ",$1);
                                  Depiler();
                                  numRes++;
                               }
          | varIDFF AFFECT exp {
                                  modifyType(ind, typ);
                                  InsertQuad("=",tete->opd," ",$1);
                                  Depiler();
                                  numRes++;
                               } VIRGUL morevarr;


morevar :  varIDF AFFECT value {
                                  modifyType(ind, typ);
                                  InsertQuad("=",tete->opd," ",$1);
                                  Depiler();
                                  numRes++;
                               }VIRGUL morevar
         | varIDF AFFECT value {
                                  modifyType(ind, typ);
                                  InsertQuad("=",tete->opd," ",$1);
                                  Depiler();
                                  numRes++;
                               }
         | varIDF {
                    InsertQuad("="," "," ",$1);
                    Depiler();
                    numRes++;
                  } VIRGUL morevar 
         | varIDF {
                    InsertQuad("="," "," ",$1);
                    Depiler();
                    numRes++;
                  }
         | varIDF LHBRACE Uint RHBRACE {modifySize(ind, atoi($3));};

varIDF : IDF { if( (ind = Insert($1, 1, typ, 0)) == -1){
                 printf("Erreur Semantique ligne : %d colonne : %d, Entite : %s\nDouble declaration\n",ligne,colone,$1); YYABORT;
               }
             };

varIDFF : IDF { 
                if( (ind = Insert($1, 1, typ, 0)) == -1){
                 ind = search($1);
                }
                Empiler($1); 
              }

exp :  A PLUS exp {
                    sprintf (buffer, "T%d", numExp++);
                    numRes++;
                    strcpy(opd1,tete->opd);
                    Depiler();
                    strcpy(opd2,tete->opd);
                    Depiler();
                    InsertQuad("+",opd2,opd1,buffer);
                    Empiler(buffer);
                  }
     | A MINUS exp {
                    sprintf (buffer, "T%d", numExp++);
                    numRes++;
                    strcpy(opd1,tete->opd);
                    Depiler();
                    strcpy(opd2,tete->opd);
                    Depiler();
                    InsertQuad("-",opd2,opd1,buffer);
                    Empiler(buffer);
                   }
     | A ;

A :  B MUL A {
                sprintf (buffer, "T%d", numExp++);
                numRes++;
                strcpy(opd1,tete->opd);
                Depiler();
                strcpy(opd2,tete->opd);
                Depiler();
                InsertQuad("*",opd2,opd1,buffer);
                Empiler(buffer);
            }
   | B DIV A {
                sprintf (buffer, "T%d", numExp++);
                numRes++;
                strcpy(opd1,tete->opd);
                Depiler();
                strcpy(opd2,tete->opd);
                Depiler();
                InsertQuad("/",opd2,opd1,buffer);
                Empiler(buffer);
             }
   | B;

B : IDF { ind = search($1); 
          if(ind == -1){ printf("Erreur Semantique ligne : %d colonne : %d, Entite : %s\not declared\n",ligne,colone,$1); YYABORT; } Empiler($1); } 
  | value;

value : Uchar {typ = 2; Empiler($1);} 
      | Uint {typ=0; Empiler($1);} 
      | Ufloat {typ=1; Empiler($1);};


cmp : C { sprintf(buffer , "%d",numRes+1); MiseAjour(mis->opd-1,buffer); DepMis(); InsertQuad("BR","else"," "," "); EmpMisAndOr(numRes); numRes++; } OR cmp { sprintf(buffer , "%d",numRes); printf("%d\n", numRes); MiseAjour(mis2->opd-1,buffer); DepMisAndOr(); }
    | C ;

C : E { andcond = numRes; nb++; } AND C { Update(mis->opd-1,(andcond-nb)-1); DepMis(); }
  | E ;

E : E DIFFERENT F { 
                    EmpMis(numRes);
                    strcpy(opd1,tete->opd);
                    Depiler();
                    strcpy(opd2,tete->opd);
                    Depiler();
                    InsertQuad("BE","else",opd2,opd1);
                    numRes++;
                  }
  | E SUP F { 
              EmpMis(numRes);
              strcpy(opd1,tete->opd);
              Depiler();
              strcpy(opd2,tete->opd);
              Depiler();
              InsertQuad("BLE","else",opd2,opd1);
              numRes++;
             }
  | E INF F { 
              EmpMis(numRes);
              strcpy(opd1,tete->opd);
              Depiler();
              strcpy(opd2,tete->opd);
              Depiler();
              InsertQuad("BGE","else",opd2,opd1);
              numRes++;
             }
  | E SUPEQ F { 
                EmpMis(numRes);
                strcpy(opd1,tete->opd);
                Depiler();
                strcpy(opd2,tete->opd);
                Depiler();
                InsertQuad("BL","else",opd2,opd1);
                numRes++;
             }
  | E INFEQ F { 
                EmpMis(numRes);
                strcpy(opd1,tete->opd);
                Depiler();
                strcpy(opd2,tete->opd);
                Depiler();
                InsertQuad("BG","else",opd2,opd1);
                numRes++;
             }
  | E EQ F {  
              EmpMis(numRes);
              strcpy(opd1,tete->opd);
              Depiler();
              strcpy(opd2,tete->opd);
              Depiler();
              InsertQuad("BNE","else",opd2,opd1);
              numRes++;
             }
  | F;

F : exp
  | LPARENTHESE cmp RPARENTHESE;


stms : statement stms
      | statement ;

if_stm : IF LPARENTHESE cmp RPARENTHESE { nb=0; } DPOINT LBRACE stms RBRACE { 
                                                                              int misindex = mis->opd;
                                                                              printf("%d, %d\n\n", misindex, numRes);
                                                                              sprintf(buffer , "%d",numRes);
                                                                              MiseAjour(misindex-1, buffer);
                                                                              DepMis();
                                                                            };

elif_stm : ELIF LPARENTHESE cmp RPARENTHESE DPOINT LBRACE stms RBRACE { 
                                                                        int misindex = mis->opd;
                                                                        printf("%d, %d\n\n", misindex, numRes);
                                                                        sprintf(buffer , "%d",numRes);
                                                                        MiseAjour(misindex-1, buffer);
                                                                        DepMis();
                                                                      };

els : ELSE DPOINT LBRACE stms RBRACE ;

checking : if_stm morecond 
         | if_stm ;

morecond :  elif_stm morecond
          | elif_stm
          | els;


loopfor : FOR IDF IN RANGE LPARENTHESE Uint VIRGUL Uint {if(atoi($6)>atoi($8)){
      printf("Error ligne : %d colonne : %d Range Beginnig > End\n", ligne, colone);
      YYABORT;
}} RPARENTHESE DPOINT LBRACE stms RBRACE ;

loopwhile : WHILE LPARENTHESE cmp RPARENTHESE DPOINT LBRACE stms RBRACE;

%%

int yyerror(char *s)
{
    if(strcmp(yytext, "{") == 0){
      fprintf(stderr, "error: %s ligne : %d , colone %d ,entite :\n", s,ligne,colone);
      return 0;
    }
    if(strcmp(yytext, ";") == 0){
      fprintf(stderr, "error: %s ligne : %d , colone %d\n", s,ligne,colone);
      return 0;
    }
    if(strcmp(yytext, "}") == 0){
      fprintf(stderr, "error: %s ligne : %d , colone %d\n", s,ligne,colone);
      return 0;
    }
    
    fprintf(stderr, "error: %s ligne : %d , colone %d ,entite %s\n", s,ligne,colone,yytext);
    return 0;
}

int main(){
    initTable();
    yyin = fopen("temp.txt","r");
    yyparse();
    displayTable();
    AfficheQuad();
    return 0;
}
