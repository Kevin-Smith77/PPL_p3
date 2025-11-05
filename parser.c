/*
NAME: Ben McIntire/Kevin Smith
FILE: parser.c
FUNC: 
INPUT:
OUTPUT:
STATUS:
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "scanner.c"

void getNextToken();

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

struct errorData{
    int position, line;
    char* code;
}

struct token curToken;
struct token nextToken;
struct errorData errorStatement;
FILE *fp;


int main (int argc, char **argv){
    errorStatement.line = 1;
    errorStatement.position = 0;
    fp = fopen (argv[1], "r");
    if (fp == NULL) {
        printf ("ERROR - File not found\n");
        return 1;
    }

    program();

    fclose (fp);
    return 0;
}

void getNextToken(){
    curToken = nextToken;
    int dummyNum = 0;
    while(c == ' ' || c == '\t' || c == '\n') {
        if(c == ' '){ 
            errorStatement.position++; 
            strcat(errorStatement.code, c);
        }
        else if(c == '\t'){ 
            errorStatement.position+=4;
            strcat(errorStatement.code, c);
        }
        else{
            errorStatement.position = 0;
            errorStatement.line++;
            errorStatement.code = " "; 
        }
        c = fgetc(fp);
    }
    
    if(c != EOF){
        if(isdigit(c)) { digit(fp, &nextToken, &dummyNum); }
        else if(isLetter(c)) { character(fp,  &nextToken,  &dummyNum); }
        else { operator(fp,  &nextToken,  &dummyNum); }
    }
    else {
        nextToken.tokenID = "<EOF>";
        nextToken.code = "EOF";
    }
    errorStatement.position += strlen(nextToken.code);
    strcat(errorStatement.code, nextToken.code);
}


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
        error("Expected '('");
    }
    else{
        getNextToken();
    }
    if(!closedParenthesis()){
        error("Expected ')'");
    }
    else{
        getNextToken();
    }
    compoundStatement();
}
void compoundStatement(){
    if(!begin()){
        error("Expected 'begin'");
    }
    else{
        getNextToken();
    }
    declarationList();
    statementList();
    getNextToken();
    if(!end()){
        error("Expected 'end'");
    }
}

void declarationList(){
    while(declaration());
}
void statementList(){
    while(nextToken.tokenID != "<END>" && nextToken.tokenID != "<EOF>"){
        statement();
        getNextToken();
    }
}
void statement(){
    if(!expressionStatement() && !conditionalStatement() && !whileStatement() && !nullStatement()){
        if(begin()){
            compoundStatement();
        }
    }
    else {
        error("Invalid statement");
        while(!nullStatement() && !begin() && nextToken.tokenID != "<END>" && nextToken.tokenID != "<EOF>"){
            getNextToken();
        }
    }
}

bool declaration(){
    if(typeSpecifier()){
        getNextToken();
        if(initalizedDeclaratorList()){
            getNextToken();
            getNextToken();
        }
        else {
            error("Invalid declaration");
            while(!nullStatement() || !typeSpecifier()){
                getNextToken();
            }
        }
        return true;
    }
    return false;
}

bool typeSpecifier(){
    if(intType() || floatType()){
        return true;
    }
    return false;
}

bool initalizedDeclaratorList(){
    while(identifier() && (strcmp(nextToken.tokenID, "<COMMA>") == 0)){
        getNextToken(); 
        getNextToken(); 
    }
    return (identifier() && (strcmp(nextToken.tokenID, "<STMT-END>") == 0));
}

bool expressionStatement(){
    if(identifier() && (strcmp(nextToken.tokenID, "<ASSIGN>") == 0)){
        getNextToken(); 
        getNextToken(); 
        if(!unaryExpression()){
            error("Invalid expression");
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
bool unaryExpression(){ 
    if(addOp() && strcmp(curToken.code, "-") == 0 ){
        getNextToken();
    }
    if(primaryExpression()){
        return true;
    }
    return false;
}
bool primaryExpression(){
    if(constant()){
        getNextToken();
        if(!nullStatement()){
            error("Expected ';'");
            while(!nullStatement() && curToken.tokenID != "<EOF>"){
                getNextToken();
            }
        }
        return true;
    }
    else if(identifier()){
        getNextToken();
        if(!nullStatement()){
            error("Expected ';'");
            while(!nullStatement() && curToken.tokenID != "<EOF>"){
                getNextToken();
            }
        }
        return true;
    }
    else if(parenthesizedExpression()){
        getNextToken();
        if(!nullStatement()){
            error("Expected ';'");
            while(!nullStatement() && curToken.tokenID != "<EOF>"){
                getNextToken();
            }
        }
        return true;
    }
    return false;
}

bool parenthesizedExpression(){
    if(openParenthesis()){
        getNextToken();
        if(!expression()){
            error("Invalid expression");
            while(!closedParenthesis() && !nullStatement() && curToken.tokenID != "<EOF>"){
                getNextToken();
            }
        }
        else{
            getNextToken();
        }
        if(!closedParenthesis()){
            error("Expected ')'");
        }
        return true;
    }
    return false;
}
bool expression(){
    if(additiveExpression() || expressionStatement()){
        return true;
    }
    return false;
}
bool whileStatement(){
    if(isWhile()){
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
        return true;
    }
    return false;
}
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
bool additiveExpression(){
    if(multiplicativeExpression()){
        getNextToken();
        while(addOp()){
            getNextToken();
            if(!multiplicativeExpression()){
                error("Invalid expression");
                return false;
            }
            getNextToken();
        }
        return true;
    }
    return false;
}
bool multiplicativeExpression(){
    if(unaryExpression()){
        getNextToken();
        while(multOp()){
            getNextToken();
            if(!unaryExpression()){
                error("Invalid expression");
                return false;
            }
            getNextToken();
        }
        return true;
    }
    return false;
}
bool relationalExpression(){
    if(additiveExpression()){
        getNextToken();
        while(relationalOp()){
            getNextToken();
            if(!additiveExpression()){
                error("Invalid expression");
                return false;
            }
            getNextToken();
        }
        return true;
    }
    return false;
}
bool equalityExpression(){
    if(relationalExpression()){
        getNextToken();
        while(equalityOp()){
            getNextToken();
            if(!relationalExpression()){
                error("Invalid expression");
                return false;
            }
            getNextToken();
        }
        return true;
    }
    return false;
}
bool logicalAndExpression(){
    if(equalityExpression()){
        getNextToken();
        while(isAnd()){
            getNextToken();
            if(!equalityExpression()){
                error("Invalid expression");
                return false;
            }
            getNextToken();
        }
        return true;
    }
    return false;
}
bool logicalOrExpression(){
    if(logicalAndExpression()){
        getNextToken();
        while(isOr()){
            getNextToken();
            if(!logicalAndExpression()){
                error("Invalid expression");
                return false;
            }
            getNextToken();
        }
        return true;
    }
    return false;
}



bool isProgram(){
    if(strcmp(curToken.tokenID, "<PROGRAM>") == 0){
        return true;
    }
    return false;
}
bool isMain(){
    if(strcmp(curToken.tokenID, "<ID>") == 0 && strcmp(curToken.code, "main") == 0){
        return true;
    }
    return false;
}

bool intType(){
    if(strcmp(curToken.tokenID, "<INT>") == 0){
        return true;
    }
    return false;
}
bool floatType(){
    if(strcmp(curToken.tokenID, "<FLOAT>") == 0){
        return true;
    }
    return false;
}
bool identifier(){
    if(strcmp(curToken.tokenID, "<ID>") == 0){
        return true;
    }
    return false;
}
bool begin(){
    if(strcmp(curToken.tokenID, "<BEGIN>") == 0){
        return true;
    }
    return false;
}
bool end(){
    if(strcmp(curToken.tokenID, "<END>") == 0){
        return true;
    }
    return false;
}
bool openParenthesis(){
    if(strcmp(curToken.tokenID, "<OPEN-PAREN>") == 0){
        return true;
    }
    return false;
}
bool closedParenthesis(){
    if(strcmp(curToken.tokenID, "<CLOSED-PAREN>") == 0){
        return true;
    }
    return false;
}
bool nullStatement(){
    if(strcmp(curToken.tokenID, "<STMT-END>") == 0){
        return true;
    }
    return false;
}
bool isWhile(){
    if(strcmp(curToken.tokenID, "<WHILE>") == 0){
        return true;
    }
    return false;
}
bool isIf(){
    if(strcmp(curToken.tokenID, "<IF>") == 0){
        return true;
    }
    return false;
}
bool isElse(){
    if(strcmp(curToken.tokenID, "<ELSE>") == 0){
        return true;
    }
    return false;
}
bool addOp(){
    if(strcmp(curToken.tokenID, "<ADD-OP>") == 0){
        return true;
    }
    return false;
}
bool multOp(){
    if(strcmp(curToken.tokenID, "<MULT-OP>") == 0){
        return true;
    }
    return false;
}
bool equalityOp(){
    if(strcmp(curToken.tokenID, "<EQ-OP>") == 0){
        return true;
    }
    return false;
}
bool relationalOp(){
    if(strcmp(curToken.tokenID, "<COMP-OP>") == 0){
        return true;
    }
    return false;
}
bool constant(){
    if(integerConstant() || floatingConstant()){
        return true;
    }
    return false;
}
bool integerConstant(){
    if(strcmp(curToken.tokenID, "<INT-CONST>") == 0){
        return true;
    }
    return false;
}
bool floatingConstant(){
    if(strcmp(curToken.tokenID, "<FLOAT-CONST>") == 0){
        return true;
    }
    return false;
}
bool isAnd(){
    if(strcmp(curToken.tokenID, "<AND>") == 0){
        return true;
    }
    return false;
}
bool isOr(){
    if(strcmp(curToken.tokenID, "<OR>") == 0){
        return true;
    }
    return false;

}
