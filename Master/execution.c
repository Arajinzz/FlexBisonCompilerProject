#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 10000

int intArray[MAX];
int front = 0;
int rear = -1;
int itemCount = 0;

int peek() {
   return intArray[front];
}

bool isEmpty() {
   return itemCount == 0;
}

bool isFull() {
   return itemCount == MAX;
}

int size() {
   return itemCount;
}  

void insert(int data) {

   if(!isFull()) {
	
      if(rear == MAX-1) {
         rear = -1;            
      }       

      intArray[++rear] = data;
      itemCount++;
   }
}

int removeData() {
   int data = intArray[front++];
	
   if(front == MAX) {
      front = 0;
   }
	
   itemCount--;
   return data;  
}


int main(){

    system("gcc strip.c -o strip");
    system("./strip");

    system("flex pytoc.l");
    system("gcc lex.yy.c -o test");
    system("./test > new.txt");

    remove("temp.txt");
    rename("new.txt", "temp.txt");
    remove("strip");
    remove("test");

    FILE *srcFile, *tempFile;

    srcFile  = fopen("temp.txt", "r");
    tempFile = fopen("new.txt", "w");

    int id;
    char last = ';';

    while(( id = fgetc(srcFile)) != EOF){

        char c = (char) id;

        if(c == '}' && (last != ';' && last != '}' && last != '{' && last != ':' && last != ')' && last != '(' && last != '\n')){
            fputc(';', tempFile);
        }

        fputc(c, tempFile);
        last = c;

    }

    fclose(srcFile);
    fclose(tempFile);

    remove("temp.txt");
    rename("new.txt", "temp.txt");

    srcFile = fopen("tp.txt", "r");
    tempFile = fopen("temp.txt", "r");

    FILE *resFile = fopen("new.txt", "w");

    int id1, id2;

    char *tpCode = malloc(1000000);
    char *tempCode = malloc(1000000);

    int n1=0, n2=0, size1=0, size2=0;

    while ((id1 = fgetc(srcFile)) != EOF)
    {
        char c = id1;
        tpCode[n1] = c;
        n1++;

    }

    size1 = n1;

    while ((id1 = fgetc(tempFile)) != EOF)
    {
        char c = id1;
        tempCode[n2] = c;
        n2++;

    }

    size2 = n2;

    n1 = 0;
    n2 = 0;

    int chars1 = 0, chars2 = 0;
    int lines = 1;

    while(n1 < size1){
        if(tpCode[n1] != ' ' && tpCode[n1] != '\t' 
            && tpCode[n1] != '\r' && tpCode[n1] !='\n'){

                chars1++;
            }

        if(tpCode[n1] == '\n'){
            if(chars1 < 3){
                insert(lines);
            }
            chars1 = 0;
            lines++;
        }
        n1++;

    }

    //insert lines
    lines = 1;
    while(n2 < size2){

        fputc(tempCode[n2], resFile);

        if(tempCode[n2] == '\n'){
            lines++;
            while(peek() == lines){
                lines++;
                fputc('\n', resFile);
                removeData();
            }
        }

        n2++;

    }

    fclose(srcFile);
    fclose(tempFile);
    fclose(resFile);
    
    free(tpCode);
    free(tempCode);

    remove("temp.txt");
    rename("new.txt", "temp.txt");

    srcFile = fopen("tp.txt", "r");
    tempFile = fopen("temp.txt", "r");
    resFile = fopen("new.txt", "w");

    tpCode = malloc(1000000);
    tempCode = malloc(1000000);

    chars1 = 0;
    n1=0;
    n2=0;
    size1=0;
    size2=0;

    while ((id1 = fgetc(srcFile)) != EOF)
    {
        char c = id1;
        tpCode[n1] = c;
        n1++;

    }

    size1 = n1;

    while ((id1 = fgetc(tempFile)) != EOF)
    {
        char c = id1;
        tempCode[n2] = c;
        n2++;

    }

    size2 = n2;

    n1 = 0;
    n2 = 0;

    fclose(srcFile);
    fclose(tempFile);

    while(n2 < size2){
        char c1 = tempCode[n2];

        if(c1 == '\n'){
            if(n2+1 < size2 && tempCode[n2+1] == '}'){
                n2++;
                continue;
            }
        }

        fputc(c1, resFile);
        n2++;
          
    }

    fclose(resFile);

    remove("temp.txt");
    rename("new.txt", "temp.txt");

    srcFile = fopen("tp.txt", "r");
    tempFile = fopen("temp.txt", "r");
    resFile = fopen("new.txt", "w");

    lines = 1;

    while((id1 = fgetc(srcFile)) != EOF){
        char c1 = id1;

        if(c1 == '\n'){
            while((id2 = fgetc(tempFile)) != EOF){
                char c2 = id2;

                if(c2 == ';' || c2 == '{' || c2 ==  '}'){
                    fputc(c2, resFile);
                }

                if(c2 == '\n'){
                    break;
                }
            }
        }

        fputc(c1, resFile);
    }

    while((id2 = fgetc(tempFile)) != EOF){
        char c2 = id2;

        if(c2 == ';' || c2 == '{' || c2 ==  '}'){
            fputc(c2, resFile);
        }

        if(c2 == '\n'){
            break;
        }
    }

    fclose(srcFile);
    fclose(tempFile);
    fclose(resFile);

    remove("temp.txt");
    rename("new.txt", "temp.txt");

    system("flex projet.l");
    system("bison -d projetb.y");
    system("gcc projetb.tab.c lex.yy.c -o test");
    system("./test");
    remove("temp.txt");
    remove("temp");
    remove("test");

    return 0;

}