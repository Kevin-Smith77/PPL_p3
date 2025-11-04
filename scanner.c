/*       
NAME: Ben McIntire/Kevin Smith
FILE: main.c
FUNC: program scanner for tokenizing C- code
INPUT: [file] containing C- code
OUTPUT: displayed token values below outputed code
STATUS: DONE
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
struct token {
    char* tokenID;
    char* code;
};

/* Global initialization for character values */
char c;

/* Default Constructors */
bool isLetter(char);
void printTokens(struct token**, int);
void operator(FILE*, struct token**, int*);
void character(FILE*, struct token**, int*);
void digit(FILE*, struct token**, int*);


int main(int argc, char **argv) {
    FILE *fp;
    struct token** tokenArray = malloc(100 * sizeof(struct token*));
    int i, numTokens = 0;
    for (i = 0; i < 100; i++) {
        tokenArray[i] = malloc(sizeof(struct token));
        tokenArray[i]->tokenID = NULL;
        tokenArray[i]->code = NULL;
    }
    fp = fopen (argv[1], "r");
    if (fp) {
        
        c = fgetc(fp);
        while (c != EOF) {
            
            while(c != '\n' && c != EOF){
                
                while(c == ' ' || c == '\t') { c = fgetc(fp); }
                if(isdigit(c)) { digit(fp, tokenArray, &numTokens); }
                else if(isLetter(c)) { character(fp, tokenArray, &numTokens); }
                else { operator(fp, tokenArray, &numTokens); }
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
FUNC: Check ascii value of a character to determine if it is a letter (upper or lower)
INPUT: [char] to check
OUTPUT: boolean value expressing if the character is a letter
*/
bool isLetter(char c){
    int val = (int)c;
    if(( val >= 65 && val <= 90 )||( val >= 97 && val <= 122)) { return 1; }
    else { return 0; }
}



/* 
FUNC: Print tokens from token array
INPUT: Array of token strings, [int] of number of token's in array
OUTPUT: Displayed token strings
*/
void printTokens(struct token** tokenArray, int numTokens) {
    int i;
    for (i = 0; i < numTokens; i++) {
        printf("%s", tokenArray[i]->tokenID);
    }
    printf("\n");
}



/* 
FUNC: Compare characters from program file to determine it's token value, add it's token value to the token array
INPUT: [file] input pointer, token array, [int] of number of tokens in array
OUTPUT: NULL
*/
void operator(FILE *fp, struct token** tokenArray, int* numTokens){
    if (c == '+' || c == '-'){
        tokenArray[*numTokens]->tokenID = "<ADD-OP>";
        tokenArray[*numTokens]->code = strdup(c);
        (*numTokens)++;
        c = fgetc(fp);
    }
    else if (c == '=' ){
        tokenArray[*numTokens]->tokenID = "<EQ-OP>";
        tokenArray[*numTokens]->code = strdup(c);
        (*numTokens)++;
        c = fgetc(fp);
    }
    else if(c==':'){
        
        c = fgetc(fp);
        if(c == '='){
            tokenArray[*numTokens]->tokenID = "<ASSIGN>";
            tokenArray[*numTokens]->code = strdup(":=");
            (*numTokens)++;
            c = fgetc(fp);
        }
        else { 
            tokenArray[*numTokens]->tokenID = "<OTHER>";
            tokenArray[*numTokens]->code = strdup(":" + c);
            (*numTokens)++;
        }
    }
    else if(c=='&'){
        
        c = fgetc(fp);
        if(c == '&'){
            tokenArray[*numTokens]->tokenID = "<AND>";
            tokenArray[*numTokens]->code = strdup("&&");
            (*numTokens)++;
            c = fgetc(fp);
        }
        else { 
            tokenArray[*numTokens]->tokenID = "<OTHER>";
            tokenArray[*numTokens]->code = strdup("&" + c);
            (*numTokens)++;
        }
    }
    else if(c=='|'){
        
        c = fgetc(fp);
        if(c == '|'){
            tokenArray[*numTokens]->tokenID = "<OR>";
            tokenArray[*numTokens]->code = strdup("||");
            (*numTokens)++;
            c = fgetc(fp);
        }
        else { 
            tokenArray[*numTokens]->tokenID = "<OTHER>";
            tokenArray[*numTokens]->code = strdup("|" + c);
            (*numTokens)++;
        }
    }

    else if (c == '!'){
        
        c = fgetc(fp);
        if(c == '=') { 
            tokenArray[(*numTokens)]->tokenID = "<EQ-OP>";
            tokenArray[(*numTokens)]->code = strdup("!=");
            (*numTokens)++;
        }
        else { 
            tokenArray[(*numTokens)++]->tokenID = "<OTHER>";
            tokenArray[(*numTokens)]->code = strdup("!" + c);
            (*numTokens)++;
        }
        c = fgetc(fp);
    }
    else if (c == '>' || c == '<'){
        
        tokenArray[(*numTokens)]->tokenID = "<COMP-OP>";
        tokenArray[(*numTokens)]->code = strdup(c);
        c = fgetc(fp);
        if(c == '='){
            //NEEDS TO BE FIXED
            printf("%c", c);
            c = fgetc(fp);
        }    
    }
    else if (c == '*' || c == '/' || c == '%'){ 
        
        tokenArray[(*numTokens)]->tokenID = "<MULT-OP>"; 
        tokenArray[(*numTokens)]->code = strdup(c);
        (*numTokens)++;
        c = fgetc(fp);
    }
    else if (c == ';'){
        
        tokenArray[(*numTokens)]->tokenID = "<STMT-END>";
        tokenArray[(*numTokens)]->code = strdup(c);
        (*numTokens)++;
        c = fgetc(fp);
    }
    else if ( c == '('){
        
        tokenArray[(*numTokens)]->tokenID = "<OPEN-PAREN>";
        tokenArray[(*numTokens)]->code = strdup(c);
        (*numTokens)++;
        c = fgetc(fp);
    }
    else if (c == ')'){
        
        tokenArray[(*numTokens)]->tokenID = "<CLOSED-PAREN>";
        tokenArray[(*numTokens)]->code = strdup(c);
        (*numTokens)++;
        c = fgetc(fp);
    }
    else if (c == ','){
        
        tokenArray[(*numTokens)]->tokenID = "<COMMA>";
        tokenArray[(*numTokens)]->code = strdup(c);
        (*numTokens)++;
        c = fgetc(fp);
    }
    else{ 
        
        tokenArray[(*numTokens)]->tokenID = "<OTHER>";
        tokenArray[(*numTokens)]->code = strdup(c);
        (*numTokens)++;
        c = fgetc(fp);
    }
}



/* 
FUNC: Compare characters from program file to determine it's token value, add it's token value to the token array
INPUT: [file] input pointer, token array, [int] of number of tokens in array
OUTPUT: NULL
*/
void character(FILE* fp, struct token** tokenArray, int* numTokens){
    char str[50];
    int index = 0;
    char* tokenType;
    while (isLetter(c)){
        str[index++] = c;
        c = fgetc(fp);
    }
    str[index] = '\0';
    if(strcmp(str, "while") == 0){ tokenType = "<WHILE>"; }
    else if(strcmp(str, "if") == 0){ tokenType = "<IF>"; }
    else if(strcmp(str, "int") == 0){ tokenType = "<INT>"; }
    else if(strcmp(str, "else") == 0){ tokenType = "<ELSE>"; }
    else if(strcmp(str, "float") == 0){ tokenType = "<FLOAT>"; }
    else if(strcmp(str, "program") == 0){ tokenType = "<PROGRAM>"; }
    else if(strcmp(str, "begin") == 0){ tokenType = "<BEGIN>"; }
    else if(strcmp(str, "end") == 0){ tokenType = "<END>"; }
    else { tokenType = "<ID>"; }
    tokenArray[*numTokens]->tokenID = tokenType;
    tokenArray[*numTokens]->code = strdup(str);
    (*numTokens)++;
}



/* 
FUNC: Compare characters from program file to determine it's token value, add it's token value to the token array
INPUT: [file] input pointer, token array, [int] of number of tokens in array
OUTPUT: NULL
*/
void digit(FILE* fp, struct token** tokenArray, int* numTokens){
    char str[50];
    int index = 0;

    while (isdigit(c)) {
        str[index++] = c;
        printf("%c", c);
        c = fgetc(fp);
    }

    if (c == '.') {
        str[index++] = c;
        printf("%c", c);
        c = fgetc(fp);
        while (isdigit(c)) {
            str[index++] = c;
            printf("%c", c);
            c = fgetc(fp);
        }
        str[index] = '\0';
        tokenArray[*numTokens]->tokenID = "<FLOAT-CONST>";
    } else {
        str[index] = '\0';
        tokenArray[*numTokens]->tokenID = "<INT-CONST>";
    }

    tokenArray[*numTokens]->code = strdup(str);
    (*numTokens)++;
}
