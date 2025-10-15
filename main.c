/*       
NAME: Ben McIntire/Kevin Smith
FILE: main.c
FUNC: 
*/

#include <stdio.h>
#include <string.h>


/* Global initialization of token values */
char token, nextToken;
/* Error struct prototype for when we need it */
struct error{
    int line, space, errorCode;
};
/* Default Constructors */
void operator();
void digit();
void character();
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
        while ((token = fgetc(fp)) != EOF) {
            while(token != '\n'){

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

void printTokens(char** tokenArray, int numTokens){
    for(int i = 0; i < numTokens; i++){
        printf("%s  ", tokenArray[i]);
    }
    printf("\n");
}
