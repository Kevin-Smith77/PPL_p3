/*
NAME: Ben McIntire/Kevin Smith
FILE: parser.c
FUNC: Parse through C- code and display any issues encountered
INPUT: [FILE] with C- code
OUTPUT: errors within C- code. Errors displayed with line number, the line of code with 
    the error up to the error, and a carrot pointing to the position of the error
STATUS: DONE
*/


/* INCLUDE STATEMENTS */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "scanner.c"


/* DEFAULT CONSTRUCTORS*/
void getNextToken();
void error(char* msg);
void program();
void compoundStatement();
void statementList();
void statement();
bool expressionStatement();
bool expression();
bool logicalOrExpression();
bool logicalAndExpression();
bool equalityExpression();
bool relationalExpression();
bool additiveExpression();
bool multiplicativeExpression();
bool primaryExpression();
bool constant();
bool integerConstant();
bool floatingConstant();
bool parenthesizedExpression();
bool openParenthesis();
bool closedParenthesis();
bool multOp();
bool addOp();
bool relationalOp();
bool equalityOp();
bool identifier();
bool unaryExpression();
bool conditionalStatement();
bool whileStatement();
bool nullStatement();
void declarationList();
bool declaration();
bool typeSpecifier();
bool intType();
bool floatType();
bool initalizedDeclaratorList();
bool begin();
bool end();
bool isProgram();
bool isMain();
bool isWhile();
bool isIf();
bool isElse();
bool isAnd();
bool isOr();



/* STRUCTURES */
struct errorData{
    int position, line;
    char* code;
};



/* GLOBALS */
struct token* curToken;
struct token* nextToken;
struct errorData errorStatement;
FILE *fp;


int main (int argc, char **argv){
    errorStatement.line = 1;
    errorStatement.position = 0;
    errorStatement.code = malloc(100 * sizeof(char));
    curToken = malloc(sizeof(struct token));
    nextToken = malloc(sizeof(struct token));
    curToken->tokenID = NULL;
    curToken->code = NULL;
    nextToken->tokenID = NULL;
    nextToken->code = NULL;
    fp = fopen (argv[1], "r");
    if (fp == NULL) {
        printf ("ERROR - File not found\n");
        return 1;
    }
    c = fgetc(fp);
    getNextToken();
    getNextToken();
    program();

    fclose (fp);
    return 0;
}

/*
FUNC: Identifies token value of code segment, updates error statement values to print error statements
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
void getNextToken(){
    *curToken = *nextToken;
    int dummyNum = 0;
    char str[2];
    str[1] = '\0';
    while(c == ' ' || c == '\t' || c == '\n') {
        if(c == ' '){ 
            errorStatement.position++; 
            str[0] = c;
            strcat(errorStatement.code, str);
        }
        else if(c == '\t'){ 
            errorStatement.position+=4;
            str[0] = c;
            strcat(errorStatement.code, str);
        }
        else{
            errorStatement.position = 0;
            errorStatement.line++;
            errorStatement.code[0] = '\0';
        }
        c = fgetc(fp);
    }
    if(c != EOF){
        if(isdigit(c)) { digit(fp, &nextToken, &dummyNum); }
        else if(isLetter(c)) { character(fp,  &nextToken,  &dummyNum); }
        else { operator(fp,  &nextToken,  &dummyNum); }
    }
    else {
        nextToken->tokenID = "<EOF>";
        nextToken->code = "EOF";
    }
    errorStatement.position += strlen(nextToken->code);
    strcat(errorStatement.code, nextToken->code);
}

/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
void program(){
    if(!isProgram()){
        error("Expected 'Program'");
    }
    else{
        getNextToken();
    }
    if(!isMain()){
        error("Expected 'main'");
    }
    else{
        getNextToken();
    }
    if(!openParenthesis()){
        error("Expected '(' 169");
    }
    else{
        getNextToken();
    }
    if(!closedParenthesis()){
        error("Expected ')' 175");
    }
    else{
        getNextToken();
    }
    compoundStatement();
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
void compoundStatement(){
    if(!begin()){
        error("Expected 'begin' 200");
    }
    else{
        getNextToken();
    }
    declarationList();
    statementList();
    /*getNextToken();*/
    if(!end()){
        error("Expected 'end' 200");
    }
    else{
        getNextToken();
    }
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
void declarationList(){
    while(declaration());
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
void statementList(){
    while(!end() && strcmp(nextToken->tokenID, "<EOF>") !=0){
        statement();
        /*getNextToken();*/
    }
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
void statement(){
    if(!expressionStatement() && !conditionalStatement() && !whileStatement() && !nullStatement()){
        if(begin()){
            compoundStatement();
        }
        else {
            error("Invalid statement 240");
            while(!nullStatement() && !begin() && strcmp(nextToken->tokenID, "<END>") !=0 && strcmp(nextToken->tokenID, "<EOF>") !=0){
                getNextToken();
            }
        }
    }
    
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool declaration(){
    if(typeSpecifier()){
        getNextToken();
        if(initalizedDeclaratorList()){
            getNextToken();
            getNextToken();
        }
        else {
            error("Invalid declaration 261");
            while(!nullStatement() && !typeSpecifier()){
                getNextToken();
            }
            if(nullStatement()){
                getNextToken();
            }
        }
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool typeSpecifier(){
    if(intType() || floatType()){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool initalizedDeclaratorList(){
    while(identifier() && (strcmp(nextToken->tokenID, "<COMMA>") == 0)){
        getNextToken(); 
        getNextToken(); 
    }
    return (identifier() && (strcmp(nextToken->tokenID, "<STMT-END>") == 0));
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool expressionStatement(){
    if(expression()){
        /*getNextToken();*/
        if(nullStatement()){
            getNextToken();
        }
        else{
            error("Expected ';' 339");
        }
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool unaryExpression(){ 
    if(addOp() && strcmp(curToken->code, "-") == 0 ){
        getNextToken();
    }
    if(primaryExpression()){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool primaryExpression(){
    if(constant()){
        getNextToken();
        return true;
    }
    else if(identifier()){
        getNextToken();
        return true;
    }
    else if(parenthesizedExpression()){
        /*getNextToken();*/
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool parenthesizedExpression(){
    if(openParenthesis()){
        getNextToken();
        if(!expression()){
            error("Invalid expression 388");
            while(!closedParenthesis() && !nullStatement() && strcmp(curToken->tokenID, "<EOF>") != 0){
                getNextToken();
            }
        }
        if(closedParenthesis()){
            getNextToken();
        } else {
            error("Expected ')' 397");
        }
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool expression(){
    if(identifier() && (strcmp(nextToken->tokenID, "<ASSIGN>") == 0)){
        getNextToken(); 
        getNextToken(); 
        if(!unaryExpression()){
            error("Invalid expression 309");
            while(!nullStatement()){
                getNextToken();
            }
        }
        return true;
    }
    else if(logicalOrExpression()){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool whileStatement(){
    if(isWhile()){
        getNextToken();
        if(!openParenthesis()){
            error("Expected '(' 427");
        }
        else{
            getNextToken();
        }
        if(!expression()){
            error("Invalid expression 433");
        }
        if(!closedParenthesis()){
            error("Expected ')' 436");
        }
        getNextToken();
        statement();
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool conditionalStatement(){
    if(isIf()){
        getNextToken();
        if(!openParenthesis()){
            error("Expected '('");
        }
        else{
            getNextToken();
        }
        if(!expression()){
            error("Invalid expression");
        }
        if(!closedParenthesis()){
            error("Expected ')'");
        }
        getNextToken();
        statement();
        if(isElse()){
            getNextToken();
            statement();
        }
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool additiveExpression(){
    if(multiplicativeExpression()){
        while(addOp()){
            getNextToken();
            if(!multiplicativeExpression()){
                error("Invalid expression 491");
                return false;
            }
        } 
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool multiplicativeExpression(){
    if(unaryExpression()){
        while(multOp()){
            getNextToken();
            if(!unaryExpression()){
                error("Invalid expression");
                return false;
            }
        }
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool relationalExpression(){
    if(additiveExpression()){
        while(relationalOp()){
            getNextToken();
            if(!additiveExpression()){
                error("Invalid expression 541");
                return false;
            }
        }
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool equalityExpression(){
    if(relationalExpression()){
        while(equalityOp()){
            getNextToken();
            if(!relationalExpression()){
                error("Invalid expression");
                return false;
            }
        }
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool logicalAndExpression(){
    if(equalityExpression()){
        while(isAnd()){
            getNextToken();
            if(!equalityExpression()){
                error("Invalid expression");
                return false;
            }
        }
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool logicalOrExpression(){
    if(logicalAndExpression()){
        while(isOr()){
            getNextToken();
            if(!logicalAndExpression()){
                error("Invalid expression");
                return false;
            }
        }
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool isProgram(){
    if(strcmp(curToken->tokenID, "<PROGRAM>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool isMain(){
    if(strcmp(curToken->tokenID, "<ID>") == 0 && strcmp(curToken->code, "main") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool intType(){
    if(strcmp(curToken->tokenID, "<INT>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool floatType(){
    if(strcmp(curToken->tokenID, "<FLOAT>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool identifier(){
    if(strcmp(curToken->tokenID, "<ID>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool begin(){
    if(strcmp(curToken->tokenID, "<BEGIN>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool end(){
    if(strcmp(curToken->tokenID, "<END>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool openParenthesis(){
    if(strcmp(curToken->tokenID, "<OPEN-PAREN>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool closedParenthesis(){
    if(strcmp(curToken->tokenID, "<CLOSED-PAREN>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool nullStatement(){
    if(strcmp(curToken->tokenID, "<STMT-END>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool isWhile(){
    if(strcmp(curToken->tokenID, "<WHILE>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool isIf(){
    if(strcmp(curToken->tokenID, "<IF>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool isElse(){
    if(strcmp(curToken->tokenID, "<ELSE>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool addOp(){
    if(strcmp(curToken->tokenID, "<ADD-OP>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool multOp(){
    if(strcmp(curToken->tokenID, "<MULT-OP>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool equalityOp(){
    if(strcmp(curToken->tokenID, "<EQ-OP>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool relationalOp(){
    if(strcmp(curToken->tokenID, "<COMP-OP>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool constant(){
    if(integerConstant() || floatingConstant()){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool integerConstant(){
    if(strcmp(curToken->tokenID, "<INT-CONST>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool floatingConstant(){
    if(strcmp(curToken->tokenID, "<FLOAT-CONST>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool isAnd(){
    if(strcmp(curToken->tokenID, "<AND>") == 0){
        return true;
    }
    return false;
}


/*
FUNC: Parser logic function to check C- syntax
INPUT: global [FILE] pointer, global [token] structues curToken/nextToken, global [errorData] structure
OUTPUT: N/A
*/
bool isOr(){
    if(strcmp(curToken->tokenID, "<OR>") == 0){
        return true;
    }
    return false;
}



/*
FUNC: Display error message 
INPUT: global errorData, [char*] error message
OUTPUT: structured error statement
*/
void error(char* msg){
    int i;
    int lineNumWidth = (int)floor(log10(errorStatement.line)) + 1;
    int caret = errorStatement.position;
    printf("%d: %s\n", errorStatement.line, errorStatement.code);
    printf("%s\n", curToken->tokenID);
    char padding[128];
    for(i = 0; i < lineNumWidth + 2 + caret; i++){
        padding[i]=' ';
    }
    padding[i] = '\0';
    printf("  %s^\n", padding);
    printf("Error: %s\n", msg);
}



