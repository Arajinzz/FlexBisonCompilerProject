#ifndef ts_h
#define ts_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//types
// 0 int , 1 float , 2 char
// size is size of table variables
// nature 0 var 1 constant
// 1 not empty 0 empty
struct symboles{
       
       int type;
       char value[100];
       int size;
       int nature;
       int empty;
       double val;
              
};

typedef struct symboles symboles;


int size = 1000000;


int primeee(int s){
    int i,j,flag=0;
    for(i=s; i >0 ; i++){
        for (j = 2; j <= i / 2; j++)
        {
            if ((i % j) == 0)
            {
                flag = 1;
                break;
            }
        }
        if(flag){
            return i;       
        }
    }      
}

int prime;
symboles *T;

void initTable(){
     T = malloc(sizeof(symboles)*size);
     prime = primeee(size);
     int i;
     for(i = 0 ; i < size ; i++){
           
           T[i].empty = 0;      
           
     }     
}

// djb2
unsigned long
hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int hash1(long value){
    
    return value % size;
    
}

int hash2(long value, int ind){

    return (ind+(prime - (value % size)))%size;
   
}

int Insert(char* value, int size, int type, int nature){
    
    int index, i;
    long gvalue = hash(value);
    
    index = hash1(gvalue);
    
    if(T[index].empty == 0){
        strcpy(T[index].value, value);
        T[index].nature = nature;
        T[index].size = size;
        T[index].type = type;
        T[index].empty = 1;
        return index;
    }
    
    if(strcmp(T[index].value, value) == 0){
        return -1;
    }
    
    index = hash2(gvalue, index);

    if(index >= size){
        index -= size;
    }
    
    if(T[index].empty == 0){
        strcpy(T[index].value, value);
        T[index].nature = nature;
        T[index].size = size;
        T[index].type = type;
        T[index].empty = 1;
        return index;
    }
    
    if(strcmp(T[index].value, value) == 0){
        return -1;
    }
        
    
    return -1;
       
}

int search(char* value){
    
    long gvalue = hash(value);
    int index = hash1(gvalue);
    
    if(strcmp(T[index].value, value) == 0){
        return index;
    }
    
    index = hash2(gvalue, index);
    
    if(index >= size){
        index -= size;
    }
    
    if(strcmp(T[index].value, value) == 0){
        return index;
    }
    
    return -1;
    
}

void displayTable(){
     
     int i;
     char c[20];
     for(i=0 ; i<size; i++){
          if(T[i].type == 0){
             strcpy(c, "int");      
          }
          
          if(T[i].type == 1){
             strcpy(c, "float");      
          }
          
          if(T[i].type == 2){
             strcpy(c, "char");      
          }
          
          if(T[i].empty == 1){
              printf("nameVar : %s\tsize : %d\ttype : %s\tkey : %d\n", T[i].value, T[i].size, c, i);
          }    
     }     
     
}

void modifySize(int index, int s){
     
     T[index].size = s;
     
}

void modifyType(int index, int t){
     
     T[index].type = t;
     
}

int CheckUnderScore(char *c,int length){
    if(c[length-1]=='_'){
        return 0;
    }
    return 1;
}

struct Quad{
    char opt[10];
    char *opd1;
    char opd2[20];
    char res[20];
};

typedef struct Quad Quad;


Quad Q[1000];
Quad optimizedQ[1000];

int k = 0;
int ok = 0;

double tTable[1000][1];
int tk = -1;

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


void calcTable(){

    int i = 0;
    int j = 0;
    int result = 0;
    

    for( i=0 ; i < k ; i++ ){
        
        if(Q[i].opt[0] == '='){
            int ind1, ind2;

            if(Q[i].res[0] != 'T'){

                ind1 = search(Q[i].res);
                ind2 = search(Q[i].opd1);

                if( ind2 == -1 && Q[i].opd1[0] != 'T'){
                    T[ind1].val = atoi(Q[i].opd1);
                }else if( Q[i].opd1[0] == 'T'){
                    T[ind1].val = tTable[tk][0];
                }else{
                    T[ind1].val = T[ind2].val;
                }
                
            }else{

                ind2 = search(Q[i].opd1);
                tk++;

                if( ind2 == -1 ){
                    tTable[tk][0] = atoi(Q[i].opd1);
                }else if( Q[i].opd1[0] == 'T'){
                    tTable[tk][0] = tTable[tk][0];
                }else{
                    tTable[tk][0] = T[ind2].val;
                }

            }

        }

        if(Q[i].opt[0] == '*'){
            int ind1, ind2;
            
            double value1;
            double value2;

            if(Q[i].opd1[0] != 'T'){

                ind2 = search(Q[i].opd1);

                if( ind2 == -1 ){
                    value1 = atoi(Q[i].opd1);
                }else{
                    value1 = T[ind2].val;
                }
                
            }else{
                value1 = tTable[tk][0];
            }

            if(Q[i].opd2[0] != 'T'){

                ind2 = search(Q[i].opd2);

                if( ind2 == -1 ){
                    value2 = atoi(Q[i].opd2);
                }else{
                    value2 = T[ind2].val;
                }
                
            }else{
                value2 = tTable[tk][0];
            }

            value1 *= value2;
            
            tk++;
            tTable[tk][0] = value1;
            

        }

        if(Q[i].opt[0] == '-'){
            int ind1, ind2;
            
            double value1;
            double value2;

            if(Q[i].opd1[0] != 'T'){

                ind2 = search(Q[i].opd1);

                if( ind2 == -1 ){
                    value1 = atoi(Q[i].opd1);
                }else{
                    value1 = T[ind2].val;
                }
                
            }else{
                value1 = tTable[tk][0];
            }

            if(Q[i].opd2[0] != 'T'){

                ind2 = search(Q[i].opd2);

                if( ind2 == -1 ){
                    value2 = atoi(Q[i].opd2);
                }else{
                    value2 = T[ind2].val;
                }
                
            }else{
                value2 = tTable[tk][0];
            }

            value1 -= value2;
            
            tk++;
            tTable[tk][0] = value1;
            

        }

        if(Q[i].opt[0] == '+'){
            int ind1, ind2;
            
            double value1;
            double value2;

            if(Q[i].opd1[0] != 'T'){

                ind2 = search(Q[i].opd1);

                if( ind2 == -1 ){
                    value1 = atoi(Q[i].opd1);
                }else{
                    value1 = T[ind2].val;
                }
                
            }else{
                value1 = tTable[tk][0];
            }

            if(Q[i].opd2[0] != 'T'){

                ind2 = search(Q[i].opd2);

                if( ind2 == -1 ){
                    value2 = atoi(Q[i].opd2);
                }else{
                    value2 = T[ind2].val;
                }
                
            }else{
                value2 = tTable[tk][0];
            }

            value1 += value2;
            
            tk++;
            tTable[tk][0] = value1;
            

        }

        if(Q[i].opt[0] == '/'){
            int ind1, ind2;
            
            double value1;
            double value2;

            if(Q[i].opd1[0] != 'T'){

                ind2 = search(Q[i].opd1);

                if( ind2 == -1 ){
                    value1 = atoi(Q[i].opd1);
                }else{
                    value1 = T[ind2].val;
                }
                
            }else{
                value1 = tTable[tk][0];
            }

            if(Q[i].opd2[0] != 'T'){

                ind2 = search(Q[i].opd2);

                if( ind2 == -1 ){
                    value2 = atoi(Q[i].opd2);
                }else{
                    value2 = T[ind2].val;
                }
                
            }else{
                value2 = tTable[tk][0];
            }

            value1 /= value2;
            
            tk++;
            tTable[tk][0] = value1;
            

        }


    }

}

void Optimize(){

    int i;
    int j;
    int z;
    int r = 0;
    int lastCondition = -1;

    for(i = 0 ; i < tk+1 ; i++){

        char st[100] = "T";
        char num[100];

        sprintf(num, "%d", i+1);
        strcat(st, num);

        for( j = 0 ; j < i ; j++ ){

            if(i == j){
                continue;
            }

            if(tTable[i][0] == tTable[j][0]){
                //printf("%d duplicated %d\n", i+1, j+1);

                char stt[100] = "T";
                char numm[100];

                sprintf(numm, "%d", j+1);
                strcat(stt, numm);

                for(z = 0 ; z < k ; z++){

                    if(strcmp(Q[z].opt, "BLE") == 0 || strcmp(Q[z].opt, "BGE") == 0 || strcmp(Q[z].opt, "BL") == 0 || strcmp(Q[z].opt, "BG") == 0 ||
                     strcmp(Q[z].opt, "BE") == 0 || strcmp(Q[z].opt, "BNE") == 0 || strcmp(Q[i].opt, "BR") == 0){
                         continue;
                     }
                    
                    if(strcmp(st, Q[z].res) == 0 && strcmp("r", Q[z].opt) != 0){
                        strcpy(Q[z].opt, "r");
                        r++;
                        continue;
                    }

                    if(strcmp(st, Q[z].opd1) == 0 && strcmp("r", Q[z].opt) != 0){
                        strcpy(Q[z].opd1, stt);
                    }

                    if(strcmp(st, Q[z].opd2) == 0 && strcmp("r", Q[z].opt) != 0){
                        strcpy(Q[z].opd2, stt);
                    }

                }      

                break;
            }

        }

    }
    
    z = 0;
    int m;

    for(i = 0 ; i < k ; i++){

        if(strcmp(Q[i].opt, "BLE") == 0 || strcmp(Q[i].opt, "BGE") == 0 || strcmp(Q[i].opt, "BL") == 0 || strcmp(Q[i].opt, "BG") == 0 ||
                        strcmp(Q[i].opt, "BE") == 0 || strcmp(Q[i].opt, "BNE") == 0 || strcmp(Q[i].opt, "BR") == 0){
                            continue;
                        }
        
        if(Q[i].res[0] != 'T'){

            int index = search(Q[i].res);
            
            for(j = z ; j < tk+1 - r ; j++){
                
                if(i == j){
                    continue;
                }

                if(tTable[j][0] == T[index].val){
                    

                    char stt[100] = "T";
                    char numm[100];

                    sprintf(numm, "%d", j+1);
                    strcat(stt, numm);

                    for(m = 0 ; m < k ; m++){

                        if(strcmp(Q[z].opt, "BLE") == 0 || strcmp(Q[z].opt, "BGE") == 0 || strcmp(Q[z].opt, "BL") == 0 || strcmp(Q[z].opt, "BG") == 0 ||
                        strcmp(Q[z].opt, "BE") == 0 || strcmp(Q[z].opt, "BNE") == 0 || strcmp(Q[i].opt, "BR") == 0){
                            continue;
                        }
                        
                        if(strcmp(stt, Q[m].res) == 0 && strcmp("r", Q[m].opt) != 0){
                            strcpy(Q[m].opt, "r");
                            r++;
                            continue;
                        }

                        if(strcmp(stt, Q[m].opd1) == 0 && strcmp("r", Q[m].opt) != 0){
                            strcpy(Q[m].opd1, Q[i].res);
                        }

                        if(strcmp(stt, Q[m].opd2) == 0 && strcmp("r", Q[m].opt) != 0){
                            strcpy(Q[m].opd2, Q[i].res);
                        }

                    }      

                    break;

                }

            }

        }else{

            z++;

        }

    }


    for( i=0 ; i < k-1 ; i++ ){

        if(strcmp(Q[i].opt, "BLE") == 0 || strcmp(Q[i].opt, "BGE") == 0 || strcmp(Q[i].opt, "BL") == 0 || strcmp(Q[i].opt, "BG") == 0 ||
                     strcmp(Q[i].opt, "BE") == 0 || strcmp(Q[i].opt, "BNE") == 0 || strcmp(Q[i].opt, "BR") == 0){
                         continue;
        }

        if(strcmp(Q[i].opt, "r") != 0 && Q[i].res[0] == 'T' && Q[i+1].res[0] != 'T'){
            
            strcpy(Q[i+1].opt, "r");
            r++;

            for( j = i ; j < k ; j++){
                
                if(j == i){
                    continue;
                }

                if(strcmp(Q[j].opt, "BLE") == 0 || strcmp(Q[j].opt, "BGE") == 0 || strcmp(Q[j].opt, "BL") == 0 || strcmp(Q[j].opt, "BG") == 0 ||
                     strcmp(Q[j].opt, "BE") == 0 || strcmp(Q[j].opt, "BNE") == 0 || strcmp(Q[i].opt, "BR") == 0){
                         continue;
                     }

                if(strcmp(Q[j].opt, "r") != 0 && strcmp(Q[j].opd1, Q[i].res) == 0){
                    strcpy(Q[j].opd1, Q[i+1].res);
                }

                if(strcmp(Q[j].opt, "r") != 0 && strcmp(Q[j].opd2, Q[i].res) == 0){
                    strcpy(Q[j].opd2, Q[i+1].res);
                }

            }

            strcpy(Q[i].res, Q[i+1].res);

        }

    }


    for(i = 0 ; i < k-1 ; i++){

        if(strcmp(Q[i].opt, "BLE") == 0 || strcmp(Q[i].opt, "BGE") == 0 || strcmp(Q[i].opt, "BL") == 0 || strcmp(Q[i].opt, "BG") == 0 ||
                     strcmp(Q[i].opt, "BE") == 0 || strcmp(Q[i].opt, "BNE") == 0 || strcmp(Q[i].opt, "BR") == 0){
                         continue;
                     }

        int found = 0;

        if(strcmp(Q[i].opt, "r") != 0){

            for( j = i ; j < k ; j++){

                if(i == j){
                    continue;
                }

                if(strcmp(Q[j].opt, "BLE") == 0 || strcmp(Q[j].opt, "BGE") == 0 || strcmp(Q[j].opt, "BL") == 0 || strcmp(Q[j].opt, "BG") == 0 ||
                     strcmp(Q[j].opt, "BE") == 0 || strcmp(Q[j].opt, "BNE") == 0 || strcmp(Q[i].opt, "BR") == 0){
                         continue;
                     }

                if(strcmp(Q[i].res, Q[j].opd1) == 0 && strcmp(Q[j].opt, "r") != 0){
                    
                    found = 1;
                    break;

                }

                if(strcmp(Q[i].res, Q[j].opd2) == 0 && strcmp(Q[j].opt, "r") != 0){
                    
                    found = 1;
                    break;

                }

                if(strcmp(Q[i].res, Q[j].res) == 0 && strcmp(Q[j].opt, "r") != 0){
                    
                    found = 1;
                    break;

                }
            }

        }

        if(found == 0){

            strcpy(Q[i].opt, "r");
            r++;

        }

    }

    int qqnum = 0;
    
    for(i = 0 ; i < k ; i++){

        if(strcmp(Q[i].opt, "r") != 0){
            qqnum++;
        }

        if(strcmp(Q[i].opt, "BLE") == 0 || strcmp(Q[i].opt, "BGE") == 0 || strcmp(Q[i].opt, "BL") == 0 || strcmp(Q[i].opt, "BG") == 0 ||
            strcmp(Q[i].opt, "BE") == 0 || strcmp(Q[i].opt, "BNE") == 0 || strcmp(Q[i].opt, "BR") == 0 ){
                
                
                int x = qqnum;
                
                for(j = i ; j < atoi(Q[i].opd1)-1 ; j++){

                    if(strcmp(Q[j].opt, "r") != 0){
                        x++;
                    }

                }   
                

                char cnum[100];
                sprintf(cnum, "%d", x);
                strcpy(Q[i].opd1, cnum);

        }
    }


}


void genCode(){


    int i, j;

    int added[10000];
    int ac = 0;

    printf("DATA SEGMENT\n");

    for(i = 0 ; i < k ; i++){

        if(strcmp(Q[i].opt, "BLE") == 0 || strcmp(Q[i].opt, "BGE") == 0 || strcmp(Q[i].opt, "BL") == 0 || strcmp(Q[i].opt, "BG") == 0 ||
            strcmp(Q[i].opt, "BE") == 0 || strcmp(Q[i].opt, "BNE") == 0 ){
                continue;
        }

        if(strcmp(Q[i].opt, "r") != 0){
            int index = search(Q[i].res);
            int found = 0;
            for(j = 0 ; j < ac ; j++){
                if(index == added[j]){
                    found = 1;
                    break;
                }
            }

            if(found == 0){
                added[ac] = index;
                ac++;
                printf("\t%s DW ?\n", Q[i].res);
            }

        }

    }

    printf("END DATA\n");



    printf("\nCODE SEGMENT\n");

    int qqnum = 1;

    for(i = 0 ; i < k ; i++){

        if(strcmp(Q[i].opt, "r") != 0){
            
            if( strcmp(Q[i].opt, "=") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd1);
                printf("\tMOV %s, AX\n",Q[i].res);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "*") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd1);
                printf("\tMOV BX, %s\n",Q[i].opd2);
                printf("\tMUL BX\n");
                printf("\tMOV %s, AX\n", Q[i].res);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "+") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd1);
                printf("\tADD AX, %s\n",Q[i].opd2);
                printf("\tMOV %s, AX\n", Q[i].res);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "-") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd1);
                printf("\tSUB AX, %s\n",Q[i].opd2);
                printf("\tMOV %s, AX\n", Q[i].res);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "/") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd1);
                printf("\tMOX BX, %s\n",Q[i].opd2);
                printf("\tDIV BX\n");
                printf("\tMOV %s, AX\n",Q[i].res);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "BE") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd2);
                printf("\tMOX BX, %s\n",Q[i].res);
                printf("\tCMP AX, BX\n");
                printf("\tJE %s\n",Q[i].opd1);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "BNE") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd2);
                printf("\tMOX BX, %s\n",Q[i].res);
                printf("\tCMP AX, BX\n");
                printf("\tJNE %s\n",Q[i].opd1);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "BLE") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd2);
                printf("\tMOX BX, %s\n",Q[i].res);
                printf("\tCMP AX, BX\n");
                printf("\tJLE %s\n",Q[i].opd1);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "BL") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd2);
                printf("\tMOX BX, %s\n",Q[i].res);
                printf("\tCMP AX, BX\n");
                printf("\tJL %s\n",Q[i].opd1);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "BGE") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd2);
                printf("\tMOX BX, %s\n",Q[i].res);
                printf("\tCMP AX, BX\n");
                printf("\tJGE %s\n",Q[i].opd1);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "BG") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tMOV AX, %s\n",Q[i].opd2);
                printf("\tMOX BX, %s\n",Q[i].res);
                printf("\tCMP AX, BX\n");
                printf("\tJG %s\n",Q[i].opd1);
                printf("\n");
            }

            if( strcmp(Q[i].opt, "BR") == 0 ){
                printf("%d : \n", qqnum);
                printf("\tJMP %s\n",Q[i].opd1);
                printf("\n");
            }

            qqnum++;

        }

    }

    printf("END CODE\n");

}

void AfficheQuad(){
    int num = 0;
    while(num < k){
      printf("%d-(%s,%s,%s,%s)\n",num+1,Q[num].opt,Q[num].opd1,Q[num].opd2,Q[num].res);
      num++;
    }

    calcTable();

    printf("\n\nQUADS OPTIMIZED\n\n");

    Optimize();
    int qnum = 1;
    num = 0;

    while(num < k){
        if(Q[num].opt[0] != 'r'){
            printf("%d-(%s,%s,%s,%s)\n",qnum,Q[num].opt,Q[num].opd1,Q[num].opd2,Q[num].res);
            qnum++;
        }
      
        num++;
    }

    printf("\n\nCODE\n\n");

    genCode();

}

#endif
