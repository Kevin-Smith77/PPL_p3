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
void program();
  void statementList();
    bool expressionStatement();
      bool logicalOrExpression();
        bool logicalAndExpression();
          bool equalityExpression();
            bool relationalExpression();
              bool additiveExpression();
                bool multiplicativeExpression();
                  bool unaryExpression();
                    bool primaryExpression();
                      bool identifier();
                      bool constant();
                        bool integerConstant();
                        bool floatingConstant();
                      bool parenthesizedExpression();
                        bool openParenthesis();
                        bool closeParenthesis();
                  bool multOp();
                bool addOP();
              bool relationalOp();
            bool equalityOp();
      bool identifier();
      bool unaryExpression();
    bool compoundStatement();
    bool conditionalStatement();
    bool whileStatement();
    bool nullStatement();
  void declarationList();
    

int main (){
  
}


