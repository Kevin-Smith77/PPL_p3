/*       
NAME: Ben McIntire/Kevin Smith
FILE: main.c
FUNC: 
*/

#include <stdio.h>
#include <stdbool.h>

/* Global initialization of token values */
char c;
/* Default Constructors */
void operator();
void digit();
void character();
bool isLetter(char);

int main(int argc, char **argv) {
    FILE *fp;
    fp = fopen (argv[1], "r");
    if (fp) {
        while ((c = fgetc(fp)) != EOF) {
            if(isDigit(c)){}
            else if(isLetter(c)){}
            else{}
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
