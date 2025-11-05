#include "scanner.c"

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