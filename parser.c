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

void getNextToken();

void program();
void compoundStatement();
  void statementList();
    bool expressionStatement();
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
                bool addOP();
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

char token[20];
char nextToken[20];

int main (){
    program();

    return 0;
}

void program(){
    if(!isProgram()){
        error("Expected 'Program'");
    }
    if(!isMain()){
        error("Expected 'main'");
    }
    if(!openParenthesis()){
        error("Expected '('");
    }
    if(!closedParenthesis()){
        error("Expected ')'");
    }
    compoundStatement();
}
void compoundStatement(){
    if(!begin()){
        error("Expected 'begin'");
    }
    declarationList();
    statementList();
    if(!end()){
        error("Expected 'end'");
    }
}

void declarationList(){
    while(declaration());
}

bool declaration(){
    if(typeSpecifier()){
        return initalizedDeclaratorList();
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
    while(identifier() && (strcmp(nextToken, "<COMMA>") == 0)){
        getNextToken(); 
        getNextToken(); 
    }
    return (identifier() && (strcmp(nextToken, "<STMT-END>") == 0));
}










bool intType(){
    if(strcmp(nextToken, "<INT>") == 0){
        return true;
    }
    return false;
}
bool floatType(){
    if(strcmp(nextToken, "<FLOAT>") == 0){
        return true;
    }
    return false;
}
bool identifier(){
    if(strcmp(nextToken, "<ID>") == 0){
        return true;
    }
    return false;
}
bool begin(){
    if(strcmp(nextToken, "<BEGIN>") == 0){
        return true;
    }
    return false;
}
bool end(){
    if(strcmp(nextToken, "<END>") == 0){
        return true;
    }
    return false;
}
bool openParenthesis(){
    if(strcmp(nextToken, "<OPEN-PAREN>") == 0){
        return true;
    }
    return false;
}
bool closedParenthesis(){
    if(strcmp(nextToken, "<CLOSED-PAREN>") == 0){
        return true;
    }
    return false;
}