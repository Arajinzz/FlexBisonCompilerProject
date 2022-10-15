/*
Binome
Hadjerci Mohammed Allaeddine
Mezhoud Abdelmoumen
ACAD A
*/

#ifndef ts_h
#define ts_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Quad{
    char opt[10];
    char *opd1;
    char opd2[20];
    char res[20];
};

struct Node{
    char *nom;
    char *type;
    char *nature;
    struct Node *next;
};

typedef struct Node Node;
typedef struct Quad Quad;

Node *head = NULL;
Quad Q[1000];
int k = 0;

void Insert(char *nom ,char *type,char *nature,int position){
    Node *temp = malloc(sizeof(Node));
    temp->type = type;
    temp->nom = nom;
    temp->nature = nature;
    temp->next = NULL;

    if(position == 1){
        temp->next = head;
        head = temp;
    }else{
        int i;
        Node *temp1 = head;
        for(i = 0;i<position - 2;i++){
            if(temp1->next == NULL)
                break;
            temp1 = temp1->next;
        }

        temp->next = temp1->next;
        temp1->next = temp;

    }
}

void InsertQuad(char *opt,char *opd1,char *opd2,char *res){
    Q[k].opd1 = malloc(20);
    strcpy(Q[k].opt, opt);
    strcpy(Q[k].opd1, opd1);
    strcpy(Q[k].opd2, opd2);
    strcpy(Q[k].res, res);
    k++;

}

void MiseAjour(int j,char *numQuad){
    strcpy(Q[j].opd1, numQuad);
}

void Update(int j,int k){
    char *c = Q[k].opd1;
    Q[j].opd1 = c;
}

void flipOperands(int j){
    char opd[100];
    strcpy(opd, Q[j].opd2);
    strcpy(Q[j].opd2, Q[j].res);
    strcpy(Q[j].res, opd);
}

int GetJump(int k){
    return atoi(Q[k].opd1);
}

void AfficheQuad(){
    int num = 0;
    while(num < k){
      printf("%d-(%s,%s,%s,%s)\n",num+1,Q[num].opt,Q[num].opd1,Q[num].opd2,Q[num].res);
      num++;
    }
}

Node* Recherche(char *nom){
    Node* temp = head;
    while(temp != NULL){
        if (strcmp(temp->nom,nom)==0) {
            return temp;
        }
        temp = temp->next;
    }
    return temp;
}

Node* RechercheConst(char *nom ,char *nature){
    Node* temp = head;
    while(temp != NULL){
        if (strcmp(temp->nom,nom)==0 && strcmp(temp->nature,nature)==0) {
            return temp;
        }
        temp = temp->next;
    }
    return temp;
}

void Affichage(){
    Node *temp = head;
    printf("\n\nLa List : \n");
    printf("NOM\tTYPE\tNATURE\n");
    while(temp != NULL){
        printf("%s\t",temp->nom);
        printf("%s\t",temp->type);
        printf("%s\t\n",temp->nature);
        temp = temp->next;
    }
    printf("\n");
}

void Ecrire(){
  Node *temp = head;
  FILE *f = fopen("ts.txt","w");
  fprintf(f,"%s","NOM\tTYPE\tNATURE\n");
  while(temp != NULL){
    fprintf(f,"%s\t",temp->nom);
    fprintf(f,"%s\t",temp->type);
    fprintf(f,"%s\t\n",temp->nature);
    temp = temp->next;
  }
  fclose(f);
}

int CheckUnderScore(char *c,int length){
    if(c[length-1]=='_'){
        return 0;
    }
    return 1;
}

int GetType(char *nom){
    Node *temp = head;
    while(temp != NULL){
        if(strcmp(temp->nom,nom) == 0){
            if(strcmp(temp->type,"Uint") == 0){
                return 1;
            }
            return 0;
        }
        temp = temp->next;
    }
    return -1;
}

#endif
