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
void operator();
void digit();
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

void printTokens(char** tokenArray, int numTokens){
    for(int i = 0; i < numTokens; i++){
        printf("%s  ", tokenArray[i]);
    }
    printf("\n");
}
/*
void character(FILE* fp, char** tokenArray, int* numTokens){
    bool done = false;
    printf("%c", c);
    if(c=='w'){
        c = fgetc(fp);
        if(c=='h'){
            printf("%c", c);
            c = fgetc(fp);
            if(c=='i'){
                printf("%c", c);
                c = fgetc(fp);
                if(c=='l'){
                    printf("%c", c);
                    c = fgetc(fp);
                    if(c=='e'){
                        printf("%c", c);
                        c = fgetc(fp);
                        if(!isLetter(c)){
                            done = true;
                            tokenArray[*numTokens] = "WHILE";
                            (*numTokens)++;
                        }
                    }
                }
            }
        }
    }
    else if(c=='i'){
        c = fgetc(fp);
        if(c=='f'){
            printf("%c", c);
            c = fgetc(fp);
            if(!isLetter(c)){
                done = true;
                tokenArray[*numTokens] = "IF";
                (*numTokens)++;
            }
        }
        else if(c=='n'){
            printf("%c", c);
            c = fgetc(fp);
            if(c=='t'){
                printf("%c", c);
                c = fgetc(fp);
                if(!isLetter(c)){
                    done = true;
                    tokenArray[*numTokens] = "INT";
                    (*numTokens)++;
                }
            }
        }
    }
    else if(c=='e'){
        c = fgetc(fp);
        if(c=='l'){
            printf("%c", c);
            c = fgetc(fp);
            if(c=='s'){
                printf("%c", c);
                c = fgetc(fp);
                if(c=='e'){
                    printf("%c", c);
                    c = fgetc(fp);
                    if(!isLetter(c)){
                        done = true;
                        tokenArray[*numTokens] = "ELSE";
                        (*numTokens)++;
                    }
                }
            }
        }
        
    }
    else if(c=='f'){
        printf("%c", c);
        c = fgetc(fp);
        if(c=='l'){
            printf("%c", c);
            c = fgetc(fp);
            if(c=='o'){
                printf("%c", c);
                c = fgetc(fp);
                if(c=='a'){
                    printf("%c", c);
                    c = fgetc(fp);
                    if(c=='t'){
                        printf("%c", c);
                        c = fgetc(fp);
                        if(!isLetter(c)){
                            done = true;
                            tokenArray[*numTokens] = "FLOAT";
                            (*numTokens)++;
                        }
                    }
                }
            }
        }
    }
    if(!done){
        while(isLetter(c)){
            printf("%c", c);
            c = fgetc(fp);
        }
        tokenArray[*numTokens] = "ID";
        (*numTokens)++;
    }
}
*/
void character(FILE* fp, char** tokenArray, int* numTokens){
    char* str = (char*) malloc(20 * sizeof(char));
    int index = 0;
    while(isLetter(c)){
        printf("%c", c);
        c = fgetc(fp);
        str[index++] = c;
    }
    str[index] = '\0';
    if(strcmp(str, "while") == 0){ tokenArray[(*numTokens)++] = "WHILE"; }
    else if(strcmp(str, "if") == 0){ tokenArray[(*numTokens)++] = "IF"; }
    else if(strcmp(str, "int") == 0){ tokenArray[(*numTokens)++] = "INT"; }
    else if(strcmp(str, "else") == 0){ tokenArray[(*numTokens)++] = "ELSE"; }
    else if(strcmp(str, "float") == 0){ tokenArray[(*numTokens)++] = "FLOAT"; }
    else{ tokenArray[(*numTokens)++] = "ID"; }
}