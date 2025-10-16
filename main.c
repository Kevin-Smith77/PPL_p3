/*       
NAME: Ben McIntire/Kevin Smith
FILE: main.c
FUNC: 
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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
    int i;
    for(i = 0; i < 100; i++){
        tokenArray[i] = (char*) malloc(10 * sizeof(char));
    }
    int numTokens = 0;
    fp = fopen (argv[1], "r");
    if (fp) {
        c = fgetc(fp);
        while (c != EOF) {
            while(c != '\n' && c != EOF){
                while(c == ' ' || c == '\t'){ c = fgetc(fp); }
                if(isdigit(c)){digit(fp, tokenArray, &numTokens);}
                else if(isLetter(c)){character(fp, tokenArray, &numTokens);}
                else{operator(fp, tokenArray, &numTokens); }
                printf("  ");
            }
            printf ("\n");
            printTokens(tokenArray, numTokens);
            numTokens = 0;
            while(c == '\n'){ c = fgetc(fp); }
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
    int i;
    /*printf("%d tokens: \n", numTokens);*/
    for(i = 0; i < numTokens; i++){
        printf("%s  ", tokenArray[i]);
    }
    printf("\n");
}

/* 
FUNC:
INPUT:
OUTPUT:
*/
void operator(FILE *fp, char** tokenArray, int* i){
    printf("%c", c);
    if (c == '+' || c == '-'){ 
        tokenArray[(*i)++] = "<ADD-OP>"; 
        c = fgetc(fp);
    }

    else if (c == '=' ){
        tokenArray[(*i)++] = "<EQ-OP>";
        c = fgetc(fp);
    }
    else if(c==':'){
        c = fgetc(fp);
        if(c == '='){ 
            tokenArray[(*i)++] = "<ASSIGN>"; 
            printf("%c", c);
            c = fgetc(fp);
        }
        else{ tokenArray[(*i)++] = "<OTHER>"; }
    }

    else if (c == '!'){
        c = fgetc(fp);
        printf("%c", c);
        if(c == '='){ tokenArray[(*i)++] = "<EQ-OP>"; }
        else{ tokenArray[(*i)++] = "<OTHER>"; }
        c = fgetc(fp);
    }
    else if (c == '>' || c == '<'){
        tokenArray[(*i)++] = "<COMP-OP>";
        c = fgetc(fp);
        if(c == '='){
            printf("%c", c);
            c = fgetc(fp);
        }    
    }
    else if (c == '*' || c == '/' || c == '%'){ 
        tokenArray[(*i)++] = "<MULT-OP>"; 
        c = fgetc(fp);
    }
    else if (c == ';'){
        tokenArray[(*i)++] = "<NULL-STMT>"; 
        c = fgetc(fp);
    }
    else if ( c == '('){
        tokenArray[(*i)++] = "<OPEN-PAREN>";
        c = fgetc(fp);
    }
    else if (c == ')'){
        tokenArray[(*i)++] = "<CLOSED-PAREN>";
        c = fgetc(fp);
    }
    else{ 
        tokenArray[(*i)++] = "<OTHER>"; 
        c = fgetc(fp);
    }
}

void character(FILE* fp, char** tokenArray, int* numTokens){
    char* str = (char*) malloc(20 * sizeof(char));
    int index = 0;
    while(isLetter(c)){
        str[index++] = c;
        printf("%c", c);
        c = fgetc(fp);
    }
    str[index] = '\0';
    if(strcmp(str, "while") == 0){ tokenArray[(*numTokens)++] = "WHILE"; }
    else if(strcmp(str, "if") == 0){ tokenArray[(*numTokens)++] = "IF"; }
    else if(strcmp(str, "int") == 0){ tokenArray[(*numTokens)++] = "INT"; }
    else if(strcmp(str, "else") == 0){ tokenArray[(*numTokens)++] = "ELSE"; }
    else if(strcmp(str, "float") == 0){ tokenArray[(*numTokens)++] = "FLOAT"; }
    else{ tokenArray[(*numTokens)++] = "ID"; }
}
void digit(FILE* fp, char** tokenArray, int* numTokens){
    while (isdigit(c)) {
        printf ("%c", c);
        c = fgetc(fp);
    }
    if (c == '.') {
        printf ("%c", c);
        c = fgetc(fp);
        while (isdigit(c)) {
            printf ("%c", c);
            c = fgetc(fp);
        }
        tokenArray[(*numTokens)++] = "FLOAT";
    }
    else { tokenArray[(*numTokens)++] = "INT"; }
}

