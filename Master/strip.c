#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000

/* Function declarations */
int  isEmpty(const char *str);
void removeEmptyLines(FILE *srcFile, FILE *tempFile);
void printFile(FILE *fptr);


int main()
{
    FILE *srcFile;
    FILE *tempFile;


    /* Try to open file */
    srcFile  = fopen("tp.txt", "r");
    tempFile = fopen("temp.txt", "w");


    // Move src file pointer to beginning
    rewind(srcFile);

    // Remove empty lines from file.
    removeEmptyLines(srcFile, tempFile);


    /* Close all open files */
    fclose(srcFile);
    fclose(tempFile);

    return 0;
}


int isEmpty(const char *str)
{
    char ch;

    do
    {
        ch = *(str++);

        // Check non whitespace character
        if(ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            return 0;

    } while (ch != '\0');

    return 1;
}



/**
 * Function to remove empty lines from a file.
 */
void removeEmptyLines(FILE *srcFile, FILE *tempFile)
{
    char buffer[BUFFER_SIZE];

    while ((fgets(buffer, BUFFER_SIZE, srcFile)) != NULL)
    {
        /* If current line is not empty then write to temporary file */
        if(!isEmpty(buffer))
            fputs(buffer, tempFile);
    }
}