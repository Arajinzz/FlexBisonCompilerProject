/*
Binome
Hadjerci Mohammed Allaeddine
Mezhoud Abdelmoumen
ACAD A
*/

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
char buffer[100];
int numRes = 1;
int numExp = 1;
char *ty;
int typing;

int repeatcond;
int andcond;
int nb = 0;

struct Pile{
  char opd[20];
  struct Pile *next;
};

struct MisA{
  int opd;
  struct MisA *next;
};

struct Type{
  int t;
  struct Type *next;
};

typedef struct Pile Pile;
typedef struct MisA MisA;
typedef struct Type Type;

Pile *tete = NULL;
MisA *mis2 = NULL;
MisA *mis = NULL;
Type *tytete = NULL;
char opd1[20];
char opd2[20];

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

void EmpType(int t){
    Type *temp = malloc(sizeof(Type));
    temp->t = t;
    temp->next = tytete;
    tytete = temp;
}

void ViderType(){
    Type *temp;
    while(tytete != NULL){
      temp = tytete;
      tytete = tytete->next;
      free(temp);
    }
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

int CheckType(int t){
    Type *temp = tytete;
    while(temp != NULL){
        if(temp->t != t){
            return 0;
        }
        temp = temp->next;
    }
    return 1;
}

%}

%union
{
  char *str;
}

%token <str> IDF
%token <str> Uint
%token <str> Ufloat
%token RBRACE
%token LBRACE
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
%token DEF
%token <str> INT
%token <str> FLOAT
%token CHECK
%token ENDCHECK
%token REPEAT
%token ENDREPEAT
%token DPOINT
%token NEGATION

%type <str> type morevar value exp A B DEF

%%

program : IDF RBRACE declaration RBRACE instructions LBRACE LBRACE {printf("programme correct\n");} ;

declaration :  variable POINTVIRGULE declaration
             | constant POINTVIRGULE declaration
             | variable POINTVIRGULE
             | constant POINTVIRGULE;

variable :  type { ty=$1; } morevar { if(Recherche($3) != NULL){ printf("Erreur Semantique ligne : %d colonne : %d, Entite : %s\nDouble declaration\n",ligne,colone,$3); YYABORT; }
                                      Insert($3,ty,"VAR",1);
                                      ViderType();};

morevar :  IDF VIRGUL morevar { if(Recherche($3) != NULL){ printf("Erreur Semantique ligne : %d colonne : %d, Entite : %s\nDouble declaration\n",ligne,colone,$3); YYABORT; }
                                Insert($3,ty,"VAR",1); }
         | IDF ;

constant :  DEF type IDF AFFECT value { if(Recherche($3) != NULL){ printf("Erreur Semantique ligne : %d colonne : %d, Entite : %s\nDouble declaration\n",ligne,colone,$3); YYABORT; }
                                        if(CheckType(tytete->t) == 0){ printf("Erreur Semantique Ligne %d , Colonne %d , Entite %s\nImcompatibilite de type\n",ligne,colone,$1); YYABORT; }
                                        Insert($3,$2,"CONST",1); ViderType();};

type :  INT {EmpType(1);} | FLOAT {EmpType(0);} ;

instructions : instruction instructions
             | instruction ;

instruction :  affectation POINTVIRGULE
             | checking
             | loop;

affectation : IDF { EmpType(GetType($1));} AFFECT exp{ if(RechercheConst($1,"CONST") != NULL){ printf("Erreur Semantique ligne : %d colonne : %d, Entite %s\nAffectation de constant\n",ligne,colone,$1); YYABORT; }
                                                       if(Recherche($1) == NULL){ printf("Erreur Semantique Ligne %d , Colonne %d , Entite %s\nVariable non declaree\n",ligne,colone,$1); YYABORT; }

                                                       InsertQuad("=",tete->opd," ",$1);
                                                       Depiler();
                                                       numRes++;
                                                       ViderType();
                                                     };

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

B : IDF { if(Recherche($1) == NULL){ printf("Erreur Semantique Ligne %d , Colonne %d , Entite %s\nVariable non declaree\n",ligne,colone,$1); YYABORT; }
          EmpType(GetType($1));
          if(CheckType(tytete->t) == 0){ printf("Erreur Semantique Ligne %d , Colonne %d , Entite %s\nImcompatibilite de type\n",ligne,colone,$1); YYABORT; }
          Empiler($1); }
  | value {
            if(CheckType(tytete->t) == 0){ printf("Erreur Semantique Ligne %d , Colonne %d , Entite %s\nImcompatibilite de type\n",ligne,colone,$1); YYABORT; }
            Empiler($1); };

value : Uint { EmpType(1); } | Ufloat { EmpType(0); };

checking : CHECK LPARENTHESE cmp RPARENTHESE { nb=0; ViderType(); } DPOINT corp ENDCHECK ;

cmp : C { sprintf(buffer , "%d",numRes+1); MiseAjour(mis->opd-1,buffer); DepMis(); InsertQuad("BR","else"," "," "); EmpMisAndOr(numRes); numRes++; } OR cmp { sprintf(buffer , "%d",numRes); printf("%d\n", numRes); MiseAjour(mis2->opd-1,buffer); DepMisAndOr(); }
    | C ;

C : E { andcond = numRes; nb++; } AND C { Update(mis->opd-1,(andcond-nb)-1); DepMis(); }
  | E ;

E : E DIFFERENT F { EmpMis(numRes);
                    strcpy(opd1,tete->opd);
                    Depiler();
                    strcpy(opd2,tete->opd);
                    Depiler();
                    InsertQuad("BE","else",opd2,opd1);
                    numRes++;
                  }
  | E SUP F { EmpMis(numRes);
              strcpy(opd1,tete->opd);
              Depiler();
              strcpy(opd2,tete->opd);
              Depiler();
              InsertQuad("BLE","else",opd2,opd1);
              numRes++;
             }
  | E INF F { EmpMis(numRes);
              strcpy(opd1,tete->opd);
              Depiler();
              strcpy(opd2,tete->opd);
              Depiler();
              InsertQuad("BGE","else",opd2,opd1);
              numRes++;
             }
  | E SUPEQ F { EmpMis(numRes);
                strcpy(opd1,tete->opd);
                Depiler();
                strcpy(opd2,tete->opd);
                Depiler();
                InsertQuad("BL","else",opd2,opd1);
                numRes++;
             }
  | E INFEQ F { EmpMis(numRes);
                strcpy(opd1,tete->opd);
                Depiler();
                strcpy(opd2,tete->opd);
                Depiler();
                InsertQuad("BG","else",opd2,opd1);
                numRes++;
             }
  | E EQ F {  EmpMis(numRes);
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

corp : instructions DPOINT { InsertQuad("BR","else"," "," "); EmpMisAndOr(numRes); numRes++; sprintf(buffer , "%d",numRes);
                              MiseAjour(mis->opd-1,buffer);
                              DepMis();}
      instructions { sprintf(buffer , "%d",numRes);
                     MiseAjour(mis2->opd-1,buffer);
                     DepMisAndOr();
                   } ;

loop : REPEAT DPOINT affectation DPOINT { EmpMisAndOr(numRes); } LPARENTHESE cmp RPARENTHESE DPOINT affectation DPOINT instructions ENDREPEAT { sprintf(buffer , "%d",numRes+1); MiseAjour(mis->opd-1,buffer); sprintf(buffer , "%d",mis2->opd); DepMisAndOr(); InsertQuad("BR",buffer," "," "); numRes++; DepMis(); };



%%

int yyerror(char *s)
{
fprintf(stderr, "error: %s ligne : %d , colone %d ,entite %s\n", s,ligne,colone,yytext);
return 0;
}

int main(){
  yyin = fopen("tp.txt","r");
  yyparse();
  Affichage();
  AfficheQuad();
  AffichePile();
  Ecrire();
  return 0;
}
