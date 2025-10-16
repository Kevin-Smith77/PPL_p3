/*       
NAME: Ben McIntire/Kevin Smith
FILE: main.c
FUNC: 
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
/* Global initialization of token values */
char c;
/* Default Constructors */
void operator(FILE*, char**, int*);
void digit(FILE*, char**, int*);
void character(FILE*, char**, int*);
bool isLetter(char);

void printTokens(char**,int);

int main(int argc, char **argv) {
    FILE *fp;
    char** tokenArray = (char**) malloc(100 * sizeof(char*));
    for(int i = 0; i < 100; i++){
        tokenArray[i] = (char*) malloc(10 * sizeof(char));
    }
    int numTokens = 0;
    fp = fopen (argv[1], "r");
    if (fp) {
        while ((c = fgetc(fp)) != EOF) {
            while(c != '\n'){
              if(isDigit(c)){}
              else if(isLetter(c)){}
              else{}
            }
            printf ("\n");
            printTokens(tokenArray, numTokens);
            numTokens = 0;
        }
        fclose (fp);
    } else
        printf ("ERROR - File not found\n");

    return 0;
}


/* 
FUNC:
INPUT:
OUTPUT:
*/
bool isLetter(char c){
    int val = (int)c;
    if(( val >= 65 && val <= 90 )||( val >= 97 && val <= 122)){ return 1; }
    else{ return 0;}
}


/* 
FUNC:
INPUT:
OUTPUT:
*/
void printTokens(char** tokenArray, int numTokens){
    for(int i = 0; i < numTokens; i++){
        printf("%s  ", tokenArray[i]);
    }
    printf("\n");
}




/* 
FUNC:
INPUT:
OUTPUT:
*/
void operator(FILE *fp, char** tokenArray, int i){
    printf(%c, c);
    if (c == '+' || c == '-'){ 
        tokenArray[*i++] = "<ADD-OP>"; 
        c = fgetc(fp);
    }

    else if (c == '=' ){
        tokenArray[*i++] = "<EQ-OP>";
        c = fgetc(fp);
    }

    else if (c == '!'){
        c = fgetc(fp);
        printf("%c", c);
        if((c == '='){ tokenArray[*i++] = "<EQ-OP>"; }
        else{ tokenArray[*i++] = "<OTHER>"; }
        c = fgetc(fp);
    }
    else if (c == '>' || c == '<'{
        tokenArray[*i++] = "<COMP-OP>";
        c = fgetc(fp);
        if(c == '='){
            printf("%c", c);
            c = fgetc(fp);
        }    
    }
    else if (c == '*' || c == '/' || c == '%'){ 
        tokenArray[*i++] = "<MULT-OP>"; 
        c = fgetc(fp);
    }
    else if (c == ';'){
        tokenArray[*i++] = "<NULL-STMT>"; 
        c = fgetc(fp);
    }
    else{ 
        tokenArray[*i++] = "<OTHER>"; 
        c = fgetc(fp);
    }
}



