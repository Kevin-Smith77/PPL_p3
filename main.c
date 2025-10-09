/*       
NAME: Ben McIntire/Kevin Smith
FILE: main.c
FUNC: 
*/

#include <stdio.h>


int main(int argc, char **argv) {
    float number;
    FILE *fp;

    fp = fopen (argv[1], "r");
    if (fp) {
        while (fscanf (fp, "%f", &number) != EOF) {
            printf ("%8.2f\n", number);
        }
        fclose (fp);
    } else
        printf ("ERROR - File not found\n");

    return 0;
}
