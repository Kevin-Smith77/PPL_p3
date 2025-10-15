/*       
NAME: Ben McIntire/Kevin Smith
FILE: main.c
FUNC: 
*/

#include <stdio.h>


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

int main(int argc, char **argv) {
    FILE *fp;
    fp = fopen (argv[1], "r");
    if (fp) {
        while ((token = fgetc(fp)) != EOF) {
            printf ("%c\n", token);
        }
        fclose (fp);
    } else
        printf ("ERROR - File not found\n");

    return 0;
}
